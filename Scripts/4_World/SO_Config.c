/**
 *  SO_Config
 *  Zentrale Config für Shadows Overhaul.
 *  Config: $profile:Shadows_Overhaul/config.json
 */

 //class definitions of every Module - add new ones below
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

class SO_SiphonSettings
{
    int enabled = 1;
    float siphonTimeSeconds = 10.0;
    float fuelPerAction = 5000; //ml pro Durchgang
}

class SO_WreckLootEntry
{
    string item;
    float chance;
    int minQty;
    int maxQty;
}

class SO_WreckSettings
{
    int enabled = 1;
    float dismantleTimeSeconds = 15.0;
    int maxLootPerWreck = 3;
    float resetIntervalHours = 48.0;
    ref array<ref SO_WreckLootEntry> lootTable = new array<ref SO_WreckLootEntry>();
}


// Config Class def
class SO_Config
{
    int infiniteStamina = 1;
    int noVehicleDamage = 1;
    int vehicleInventory = 1;
    ref SO_GasPumpSettings gasPumpRefuel = new SO_GasPumpSettings();
    ref SO_CarFlipSettings carFlip = new SO_CarFlipSettings();
    ref SO_SiphonSettings  siphonFuel = new SO_SiphonSettings();
    ref SO_WreckSettings   wreckDismantle = new SO_WreckSettings();
    
    
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

    // Getter methods and check if Module is activated in the config
    // - add new ones below if new modules will be added
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

    static bool IsSiphonEnabled()
    {
        return Get().siphonFuel.enabled == 1;
    }

    static SO_SiphonSettings GetSiphonSettings()
    {
        return Get().siphonFuel;
    }

    static bool IsWreckDismantleEnabled()
    {
        return Get().wreckDismantle.enabled == 1;
    }

    static SO_WreckSettings GetWreckSettings()
    {
        return Get().wreckDismantle;
    }


    // Config Loader
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
            InitDefaultLootTable();
            Save();
            Print("[Shadows_Overhaul] Defult Config created.");
        }

        if (wreckDismantle.lootTable.Count() == 0)
            InitDefaultLootTable();
    }

    // Init Method for Loottable for Wrecks
    private void InitDefaultLootTable()
    {
        //AddLoot("itemname", chance, minQty, maxQty)
        wreckDismantle.lootTable.Clear();
        AddLoot("MetalWire", 0.6, 1, 3);
        AddLoot("MetalSheet", 0.6, 1, 2);
        AddLoot("SparkPlug", 0.1, 1, 1);
        AddLoot("HeadlightH7", 0.1, 1, 2);
        // Add materials for Biofueladdon
        AddLoot("StaleOil", 0.1, 1, 1);
        AddLoot("Tubes", 0.2, 1, 3);
        // very small chance to get the complete assembled module: BioFuelCarburetorModule
        AddLoot("BFCM", 0.001, 1, 3);

    }
    

    private void AddLoot(string item, float chance, int minQty, int maxQty)
    {
        SO_WreckLootEntry entry = new SO_WreckLootEntry();
        entry.item    = item;
        entry.chance  = chance;
        entry.minQty  = minQty;
        entry.maxQty  = maxQty;
        wreckDismantle.lootTable.Insert(entry);
    }

    //Config Saver
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