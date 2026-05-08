modded class CarScript
{
    override void EEInit()
    {
        super.EEInit();

        if (SO_Config.IsNoVehicleDamage())
            SetAllowDamage(false);
    }

    override void DamageCrew(float dmg)
    {
        if (SO_Config.IsNoVehicleDamage())
            return;

        super.DamageCrew(dmg);
    }

    override void OnVehicleJumpOutServer(GetOutTransportActionData gotActionData)
    {
        if (SO_Config.IsNoVehicleDamage())
            return;

        super.OnVehicleJumpOutServer(gotActionData);
    }
}