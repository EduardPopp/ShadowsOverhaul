/**
 *  SO_ActionRefuelAtPump – Release
 *
 *  requirePumpCheck = 1:
 *    Phase 1: "Zapfsäule prüfen" → 3s → Meldung mit Menge
 *    Phase 2: "Tanken (XL übrig)" → 12s → Auto betanken
 *
 *  requirePumpCheck = 0:
 *    Direkt "Tanken (XL übrig)" → 12s → Auto betanken
 */

class SO_ActionRefuelAtPumpCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        SO_GasPumpSettings cfg = SO_Config.GetGasPumpSettings();
        float duration;

        // Prüfen oder Tanken?
        if (SO_ActionRefuelAtPump.IsCheckMode())
            duration = cfg.pumpCheckSeconds;
        else
            duration = cfg.refuelTimeSeconds;

        m_ActionData.m_ActionComponent = new CAContinuousTime(duration);
    }
}

class SO_ActionRefuelAtPump : ActionContinuousBase
{
    private static float s_CachedFuel = 0;
    private static bool  s_IsCheckMode = false;

    // Speichert welche Pumps der Spieler bereits geprüft hat (Session-basiert)
    private static ref map<string, ref set<string>> s_CheckedPumps = new map<string, ref set<string>>();

    void SO_ActionRefuelAtPump()
    {
        m_CallbackClass     = SO_ActionRefuelAtPumpCB;
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_CRAFTING;
        m_FullBody          = false;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text              = "Tanken";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem   = new CCINone();
        m_ConditionTarget = new CCTNone();
    }

    override typename GetInputType()
    {
        return ContinuousInteractActionInput;
    }

    override bool HasTarget()
    {
        return true;
    }

    // Getter für den Callback
    static float GetCachedFuel()
    {
        return s_CachedFuel;
    }

    static bool IsCheckMode()
    {
        return s_IsCheckMode;
    }

    override string GetText()
    {
        if (s_IsCheckMode)
            return "Zapfsäule prüfen";

        if (s_CachedFuel <= 0.1)
            return "Zapfsäule prüfen";

        return "Tanken (" + Math.Round(s_CachedFuel).ToString() + "L übrig)";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!SO_Config.IsGasPumpEnabled())
            return false;

        CarScript vehicle = CarScript.Cast(target.GetObject());
        if (!vehicle)
            return false;

        Object pump = FindNearestPump(player);
        if (!pump)
            return false;

        string pumpKey = SO_GasPumpManager.GetPumpKey(pump.GetPosition());
        string playerUID = "";

        if (player.GetIdentity())
            playerUID = player.GetIdentity().GetId();
        else
            playerUID = "offline";

        // Muss der Spieler erst prüfen?
        bool needsCheck = SO_Config.RequiresPumpCheck() && !HasPlayerCheckedPump(playerUID, pumpKey);
        s_IsCheckMode = needsCheck;

        if (needsCheck)
        {
            // Im Check-Modus: immer anzeigen
            return true;
        }

        // Bereits geprüft: normaler Tankmodus
        float fuel = SO_GasPumpManager.Get().GetFuel(pump.GetPosition());
        s_CachedFuel = fuel;

        // Säule leer und Tank voll → nichts zu tun
        if (fuel <= 0.1 && vehicle.GetFluidFraction(CarFluid.FUEL) >= 0.98)
            return false;

        // Säule leer → Prüfen anbieten (auch wenn schon geprüft, Info aktualisieren)
        if (fuel <= 0.1)
        {
            s_IsCheckMode = true;
            return true;
        }

        // Tank voll → kein Tanken nötig
        if (vehicle.GetFluidFraction(CarFluid.FUEL) >= 0.98)
            return false;

        return true;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        if (!player)
            return;

        Object pump = FindNearestPump(player);
        if (!pump)
            return;

        string pumpKey = SO_GasPumpManager.GetPumpKey(pump.GetPosition());
        string playerUID = "";

        if (player.GetIdentity())
            playerUID = player.GetIdentity().GetId();
        else
            playerUID = "offline";

        vector pumpPos = pump.GetPosition();
        float fuel = SO_GasPumpManager.Get().GetFuel(pumpPos);

        // Check-Modus: Spieler hat geprüft → Ergebnis mitteilen
        bool wasCheckMode = SO_Config.RequiresPumpCheck() && !HasPlayerCheckedPump(playerUID, pumpKey);

        if (wasCheckMode)
        {
            MarkPumpChecked(playerUID, pumpKey);

            if (fuel <= 0.1)
                GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Die Säule scheint leer zu sein...", ""));
            else
                GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Etwa " + Math.Round(fuel).ToString() + "L verfügbar.", ""));

            s_CachedFuel = fuel;
            return;
        }

        // Säule leer → nur Meldung
        if (fuel <= 0.1)
        {
            GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Die Säule scheint leer zu sein...", ""));
            return;
        }

        // Tank-Modus: Auto betanken
        CarScript vehicle = CarScript.Cast(action_data.m_Target.GetObject());
        if (!vehicle)
            return;

        SO_GasPumpSettings cfg = SO_Config.GetGasPumpSettings();
        float wantedFuel = cfg.fuelPerSecond * cfg.refuelTimeSeconds;

        float actualFuel = SO_GasPumpManager.Get().ConsumeFuel(pumpPos, wantedFuel);
        if (actualFuel <= 0)
            return;

        vehicle.Fill(CarFluid.FUEL, actualFuel);

        float fuelLeft = SO_GasPumpManager.Get().GetFuel(pumpPos);
        s_CachedFuel = fuelLeft;

        if (fuelLeft <= 0.1)
        {
            GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Die Säule scheint leer zu sein...", ""));
        }
    }

    // =============================================
    //  Pump-Check Tracking (Session-basiert)
    // =============================================

    static bool HasPlayerCheckedPump(string playerUID, string pumpKey)
    {
        if (!s_CheckedPumps.Contains(playerUID))
            return false;

        return s_CheckedPumps.Get(playerUID).Find(pumpKey) != -1;
    }

    static void MarkPumpChecked(string playerUID, string pumpKey)
    {
        if (!s_CheckedPumps.Contains(playerUID))
        {
            s_CheckedPumps.Set(playerUID, new set<string>());
        }

        s_CheckedPumps.Get(playerUID).Insert(pumpKey);
    }

    // =============================================
    //  Pump Suche
    // =============================================

    static Object FindNearestPump(PlayerBase player)
    {
        float radius = SO_Config.GetGasPumpSettings().vehicleSearchRadius;
        vector playerPos = player.GetPosition();

        array<Object> objects = new array<Object>();
        array<CargoBase> proxyCargos = new array<CargoBase>();
        GetGame().GetObjectsAtPosition(playerPos, radius * 2, objects, proxyCargos);

        Object nearest = null;
        float nearestDist = radius * 2 + 1;

        foreach (Object obj : objects)
        {
            if (obj.IsKindOf("Land_FuelStation_Feed"))
            {
                float dist = vector.Distance(playerPos, obj.GetPosition());
                if (dist < nearestDist)
                {
                    nearestDist = dist;
                    nearest = obj;
                }
            }
        }

        return nearest;
    }
}