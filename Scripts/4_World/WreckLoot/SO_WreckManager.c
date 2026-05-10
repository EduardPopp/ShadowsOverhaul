/**
 *  SO_WreckManager
 *  Verwaltet welche Wracks bereits zerlegt wurden.
 *  Persistenz über JSON, konfigurierbarer Reset.
 */

class SO_WreckData
{
    int timesLooted;
    float lastLootTime;
}

class SO_WreckStorage
{
    ref map<string, ref SO_WreckData> wrecks = new map<string, ref SO_WreckData>();
}

class SO_WreckManager
{
    private static ref SO_WreckManager m_Instance;
    private ref SO_WreckStorage m_Storage;
    private string m_SavePath = "$profile:Shadows_Overhaul/wrecks.json";

    static SO_WreckManager Get()
    {
        if (!m_Instance)
        {
            m_Instance = new SO_WreckManager();
            m_Instance.Load();
        }
        return m_Instance;
    }

    static string GetWreckKey(vector pos)
    {
        int x = Math.Round(pos[0]);
        int y = Math.Round(pos[1]);
        int z = Math.Round(pos[2]);
        return x.ToString() + "_" + y.ToString() + "_" + z.ToString();
    }

    bool CanLoot(vector wreckPos)
    {
        string key = GetWreckKey(wreckPos);
        SO_WreckData data;

        if (!m_Storage.wrecks.Find(key, data))
            return true;

        // Reset-Check
        float now = GetGame().GetTime() / 1000.0;
        float intervalSec = SO_Config.GetWreckSettings().resetIntervalHours * 3600.0;

        if ((now - data.lastLootTime) >= intervalSec)
            return true;

        // Max Loot erreicht?
        return false;
    }

    void MarkLooted(vector wreckPos)
    {
        string key = GetWreckKey(wreckPos);
        SO_WreckData data = new SO_WreckData();
        data.timesLooted = 1;
        data.lastLootTime = GetGame().GetTime() / 1000.0;
        m_Storage.wrecks.Set(key, data);
        Save();
    }

    private void Load()
    {
        m_Storage = new SO_WreckStorage();

        if (FileExist(m_SavePath))
        {
            JsonFileLoader<SO_WreckStorage>.JsonLoadFile(m_SavePath, m_Storage);
            Print("[Shadows_Overhaul] Wrack-Daten geladen: " + m_Storage.wrecks.Count().ToString() + " Wracks");
        }
    }

    private void Save()
    {
        string dir = "$profile:Shadows_Overhaul";
        if (!FileExist(dir))
            MakeDirectory(dir);

        JsonFileLoader<SO_WreckStorage>.JsonSaveFile(m_SavePath, m_Storage);
    }
}