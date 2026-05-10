/**
 *  Modded ActionFillBottleBase
 *  Intercept: Wenn an einer Zapfsäule getankt wird,
 *  wird der Verbrauch vom GasPumpManager-Pool abgezogen.
 *  Alle anderen Quellen (Brunnen, Teiche etc.) bleiben vanilla.
 */

modded class ActionFillBottleBase
{
    private float m_SOStartQuantity = -1;
    private vector m_SOPumpPos;
    private bool m_SOAtPump = false;

    override void OnStartServer(ActionData action_data)
    {
        super.OnStartServer(action_data);

        m_SOAtPump = false;
        m_SOStartQuantity = -1;

        PlayerBase player = action_data.m_Player;
        if (!player || !SO_Config.IsGasPumpEnabled())
            return;

        // Prüfe ob wir an einer Zapfsäule stehen
        Object pump = SO_ActionRefuelAtPump.FindNearestPump(player);
        if (!pump)
            return;

        // Prüfe ob die Quelle Benzin ist
        Object targetObj = action_data.m_Target.GetObject();
        if (!targetObj)
            return;

        if (targetObj.GetLiquidSourceType() != LIQUID_GASOLINE)
            return;

        m_SOAtPump = true;
        m_SOPumpPos = pump.GetPosition();
        m_SOStartQuantity = action_data.m_MainItem.GetQuantity();
    }

    override bool ActionConditionContinue(ActionData action_data)
    {
        // Vanilla-Check
        if (!super.ActionConditionContinue(action_data))
            return false;

        // Wenn an Zapfsäule: prüfe ob noch Sprit da ist
        if (m_SOAtPump)
        {
            if (!SO_GasPumpManager.Get().HasFuel(m_SOPumpPos))
                return false;

            // Prüfe ob bereits mehr getankt als verfügbar
            float currentQty = action_data.m_MainItem.GetQuantity();
            float filled = currentQty - m_SOStartQuantity;
            float filledLiters = filled / 1000.0;
            float available = SO_GasPumpManager.Get().GetFuel(m_SOPumpPos);

            if (filledLiters >= available)
                return false;
        }

        return true;
    }

    override void OnEndServer(ActionData action_data)
    {
        // Verbrauch vom Pool abziehen
        if (m_SOAtPump && m_SOStartQuantity >= 0 && action_data.m_MainItem)
        {
            float endQuantity = action_data.m_MainItem.GetQuantity();
            float totalFilled = endQuantity - m_SOStartQuantity;

            if (totalFilled > 0)
            {
                float totalFilledLiters = totalFilled / 1000.0;

                float consumed = SO_GasPumpManager.Get().ConsumeFuel(m_SOPumpPos, totalFilledLiters);

                // Falls weniger verfügbar als getankt: Überschuss entfernen
                if (consumed < totalFilledLiters)
                {
                    float excess = totalFilledLiters - consumed;
                    action_data.m_MainItem.AddQuantity(-excess);
                }

                Print("[Shadows_Overhaul] Canister filled: +" + consumed.ToString() + "ml from pool.");
            }
        }

        m_SOAtPump = false;
        m_SOStartQuantity = -1;

        super.OnEndServer(action_data);
    }

    // Blockiere die Action komplett wenn Zapfsäule leer ist
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!super.ActionCondition(player, target, item))
            return false;

        // Nur bei Zapfsäulen eingreifen
        if (SO_Config.IsGasPumpEnabled() && target.GetObject())
        {
            if (target.GetObject().GetLiquidSourceType() == LIQUID_GASOLINE)
            {
                Object pump = SO_ActionRefuelAtPump.FindNearestPump(player);
                if (pump && !SO_GasPumpManager.Get().HasFuel(pump.GetPosition()))
                    return false;
            }
        }

        return true;
    }
}