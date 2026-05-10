/**
 *  SO_ActionSiphonFuel
 *  Spieler hält Kanister in der Hand, schaut Auto an,
 *  hält gedrückt → Sprit wird vom Auto in den Kanister gesaugt.
 */

class SO_ActionSiphonFuelCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float duration = SO_Config.GetSiphonSettings().siphonTimeSeconds;
        m_ActionData.m_ActionComponent = new CAContinuousTime(duration);
    }
}

class SO_ActionSiphonFuel : ActionContinuousBase
{
    void SO_ActionSiphonFuel()
    {
        m_CallbackClass     = SO_ActionSiphonFuelCB;
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_CRAFTING;
        m_FullBody          = false;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text              = "#Siphoning_00";
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

    override string GetText()
    {
        return "#Siphoning_00";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!SO_Config.IsSiphonEnabled())
            return false;
        
        // Spieler muss Kanister in der Hand halten
        ItemBase inHands = player.GetItemInHands();
        Print("[Shadows_Overhaul] Siphon: InHands: " + (inHands != null));
        if (!inHands || !inHands.IsKindOf("CanisterGasoline"))
            return false;

        // Kanister muss noch Platz haben
        if (inHands.GetQuantity() >= inHands.GetQuantityMax() - 1)
            return false;

        // Ziel muss ein Fahrzeug sein
        Car car = Car.Cast(target.GetObject());
        if (!car)
            return false;


        // Fahrzeug muss Sprit haben
        if (car.GetFluidFraction(CarFluid.FUEL) <= 0.01)
            return false;

        // Spieler darf nicht im Auto sitzen
        if (player.GetCommand_Vehicle())
            return false;

        return true;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        if (!player)
            return;

        Car car = Car.Cast(action_data.m_Target.GetObject());
        if (!car)
            return;

        ItemBase canister = player.GetItemInHands();
        if (!canister)
            return;

        SO_SiphonSettings cfg = SO_Config.GetSiphonSettings();
        float fuelMl = canister.GetQuantityMax() * (cfg.fuelPerAction / 100.0);

        // Nicht mehr als der Kanister aufnehmen kann
        float spaceLeft = canister.GetQuantityMax() - canister.GetQuantity();
        fuelMl = Math.Min(fuelMl, spaceLeft);

        // Nicht mehr als das Auto hat (in ml umgerechnet)
        float carFuelCapacity = car.GetFluidCapacity(CarFluid.FUEL);
        float carFuelCurrentMl = car.GetFluidFraction(CarFluid.FUEL) * carFuelCapacity * 1000.0;
        fuelMl = cfg.fuelPerAction;

        if (fuelMl <= 0)
            return;

        // Ein Wert, zwei Einheiten
        canister.SetLiquidType(LIQUID_GASOLINE);
        float fuelLiters = fuelMl / 1000.0;

        car.Leak(CarFluid.FUEL, fuelLiters);

        canister.AddQuantity(fuelMl);
    }
}