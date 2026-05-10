/**
 *  SO_NoDamagePlayer
 *  Spieler können nicht überfahren werden.
 *  Toggle: noVehicleDamage in config.json
 */

modded class PlayerBase
{
    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
    {
        if (SO_Config.IsNoVehicleDamage() && source)
        {
            // Schaden von Fahrzeug → blocken
            if (source.IsInherited(Transport))
            {
                return;
            }
        }

        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
    }
}