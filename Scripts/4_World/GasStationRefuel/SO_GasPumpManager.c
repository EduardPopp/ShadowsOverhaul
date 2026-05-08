/**
 *  SO_GasPumpManager
 *  Verwaltet den Spritvorrat jeder Zapfsäule.
 *  Persistenz über JSON, 24h-Reset mit zufälliger Menge.
 */

class SO_GasPumpData
{
    float fuelRemaining;
    float lastResetTime;  // Server-Zeit des letzten Resets
}

class SO_GasPumpStorage
{
    ref map<string, ref SO_GasPumpData> pumps = new map<string, ref SO_GasPumpData>();
}

class SO_GasPumpManager
{
    private static ref SO_GasPumpManager m_Instance;
    private ref SO_GasPumpStorage m_Storage;
    private string m_SavePath = "$profile:Shadows_Overhaul/gaspumps.json";

    static SO_GasPumpManager Get()
    {
        if (!m_Instance)
        {
            m_Instance = new SO_GasPumpManager();
            m_Instance.Load();
        }
        return m_Instance;
    }

    // Erzeugt einen eindeutigen Key aus der Weltposition der Zapfsäule
    static string GetPumpKey(vector pos)
    {
        // Auf ganze Zahlen runden, damit kleine Floating-Point-Abweichungen keinen neuen Key erzeugen
        int x = Math.Round(pos[0]);
        int y = Math.Round(pos[1]);
        int z = Math.Round(pos[2]);
        return x.ToString() + "_" + y.ToString() + "_" + z.ToString();
    }

    // Holt den aktuellen Spritvorrat einer Zapfsäule
    float GetFuel(vector pumpPos)
    {
        string key = GetPumpKey(pumpPos);
        SO_GasPumpData data;

        if (m_Storage.pumps.Find(key, data))
        {
            // Prüfe ob Reset fällig ist
            if (ShouldReset(data))
            {
                ResetPump(key, data);
                Save();
            }
            return data.fuelRemaining;
        }
        else
        {
            // Neue Zapfsäule – initialisieren
            data = new SO_GasPumpData();
            ResetPump(key, data);
            m_Storage.pumps.Set(key, data);
            Save();
            return data.fuelRemaining;
        }
    }

    // Zieht Sprit von der Zapfsäule ab, gibt tatsächlich verfügbare Menge zurück
    float ConsumeFuel(vector pumpPos, float amount)
    {
        string key = GetPumpKey(pumpPos);
        SO_GasPumpData data;

        if (!m_Storage.pumps.Find(key, data))
        {
            return 0;
        }

        float actual = Math.Min(amount, data.fuelRemaining);
        data.fuelRemaining -= actual;

        if (data.fuelRemaining < 0)
            data.fuelRemaining = 0;

        Save();
        return actual;
    }

    bool HasFuel(vector pumpPos)
    {
        return GetFuel(pumpPos) > 0.1;
    }

    private bool ShouldReset(SO_GasPumpData data)
    {
        float now = GetGame().GetTime() / 1000.0;  // ms → Sekunden
        float intervalSec = SO_Config.GetGasPumpSettings().resetIntervalHours * 3600.0;
        return (now - data.lastResetTime) >= intervalSec;
    }

    private void ResetPump(string key, SO_GasPumpData data)
    {
        SO_GasPumpSettings cfg = SO_Config.GetGasPumpSettings();
        data.fuelRemaining = Math.RandomFloat(cfg.minFuelLiters, cfg.maxFuelLiters);
        data.lastResetTime = GetGame().GetTime() / 1000.0;
        Print("[Shadows_Overhaul] Gaspump " + key + " refilled " + data.fuelRemaining.ToString() + "L");
    }

    private void Load()
    {
        m_Storage = new SO_GasPumpStorage();

        if (FileExist(m_SavePath))
        {
            JsonFileLoader<SO_GasPumpStorage>.JsonLoadFile(m_SavePath, m_Storage);
            Print("[Shadows_Overhaul] GasPump-Data loaded: " + m_Storage.pumps.Count().ToString() + " Gaspumps");
        }
        else
        {
            Print("[Shadows_Overhaul] No GasPump-Data – fresh start.");
        }
    }

    private void Save()
    {
        string dir = "$profile:Shadows_Overhaul";

        if (!FileExist(dir))
        {
            MakeDirectory(dir);
        }

        JsonFileLoader<SO_GasPumpStorage>.JsonSaveFile(m_SavePath, m_Storage);
    }
}