/**
 *  SO_Config
 *  Zentrale Config für Shadows Overhaul.
 *  Config: $profile:Shadows_Overhaul/config.json
 */

class SO_GasPumpSettings
{
    int   enabled             = 1;
    int   requirePumpCheck    = 1;     // 1 = Spieler muss erst prüfen, 0 = sofort Menge sichtbar
    float pumpCheckSeconds    = 3.0;   // Sekunden für das Prüfen
    float vehicleSearchRadius = 5.0;
    float refuelTimeSeconds   = 12.0;
    float fuelPerSecond       = 5.0;
    float minFuelLiters       = 200.0;
    float maxFuelLiters       = 500.0;
    float resetIntervalHours  = 24.0;
}

class SO_CarFlipSettings
{
    int   enabled             = 1;
    float flipTimeSeconds     = 5.0;
}

class SO_Config
{
    int infiniteStamina = 1;
    int noVehicleDamage = 1;
    int vehicleInventory = 1;
    ref SO_GasPumpSettings gasPumpRefuel = new SO_GasPumpSettings();
    ref SO_CarFlipSettings carFlip = new SO_CarFlipSettings();

    private static ref SO_Config m_Instance;

    static SO_Config Get()
    {
        if (!m_Instance)
        {
            m_Instance = new SO_Config();
            m_Instance.Load();
        }
        return m_Instance;
    }

    static bool IsInfiniteStamina()
    {
        return Get().infiniteStamina == 1;
    }

    static bool IsNoVehicleDamage()
    {
        return Get().noVehicleDamage == 1;
    }
    
    static bool IsVehicleInventory()
    {
        return Get().vehicleInventory == 1;
    }

    static bool IsGasPumpEnabled()
    {
        return Get().gasPumpRefuel.enabled == 1;
    }

    static bool RequiresPumpCheck()
    {
        return Get().gasPumpRefuel.requirePumpCheck == 1;
    }

    static SO_GasPumpSettings GetGasPumpSettings()
    {
        return Get().gasPumpRefuel;
    }

    static float GetCarFlipSeconds()
    {
        return Get().carFlip.flipTimeSeconds;
    }

    static bool IsCarFlipEnabled()
    {
        return Get().carFlip.enabled == 1;
    }

    private void Load()
    {
        string path = "$profile:Shadows_Overhaul/config.json";

        if (FileExist(path))
        {
            JsonFileLoader<SO_Config>.JsonLoadFile(path, this);
            Print("[Shadows_Overhaul] Config loaded.");
        }
        else
        {
            Save();
            Print("[Shadows_Overhaul] Defult Config created.");
        }
    }

    private void Save()
    {
        string dir = "$profile:Shadows_Overhaul";

        if (!FileExist(dir))
        {
            MakeDirectory(dir);
        }

        JsonFileLoader<SO_Config>.JsonSaveFile("$profile:Shadows_Overhaul/config.json", this);
    }
}