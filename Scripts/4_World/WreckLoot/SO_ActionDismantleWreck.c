/**
 *  SO_ActionDismantleWreck
 *  Spieler steht neben einem Autowrack und zerlegt es.
 *  Action am Spieler registriert (Wracks sind statische Objekte).
 *  Spawnt Items aus der konfigurierbaren Loot-Tabelle.
 */

class SO_ActionDismantleWreckCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float duration = SO_Config.GetWreckSettings().dismantleTimeSeconds;
        m_ActionData.m_ActionComponent = new CAContinuousTime(duration);
    }
}

class SO_ActionDismantleWreck : ActionContinuousBase
{
    void SO_ActionDismantleWreck()
    {
        m_CallbackClass     = SO_ActionDismantleWreckCB;
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_CRAFTING;
        m_FullBody          = false;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text              = "#Dismantle_00";
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
        return "#Dismantle_00";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!SO_Config.IsWreckDismantleEnabled())
            return false;

        //Spieler muss Schraubenschlüssel in der Hand halten
        ItemBase inHands = player.GetItemInHands();


        if(!inHands)
            return false;

        if (!inHands.IsKindOf("Wrench") && !inHands.IsKindOf("PipeWrench"))
        return false;

        // Prüfe ob ein Wrack in der Nähe ist
        Object wreck = FindNearestWreck(player);

        if (!wreck)
            return false;

        // Prüfe ob noch lootbar
        if (!SO_WreckManager.Get().CanLoot(wreck.GetPosition()))
            return false;

        return true;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        if (!player)
            return;

        Object wreck = FindNearestWreck(player);
        if (!wreck)
            return;

        if (!SO_WreckManager.Get().CanLoot(wreck.GetPosition()))
            return;

        // Loot spawnen
        SpawnLoot(player, wreck);

        // Als gelooted markieren
        SO_WreckManager.Get().MarkLooted(wreck.GetPosition());
    }

    private void SpawnLoot(PlayerBase player, Object wreck)
    {
        SO_WreckSettings cfg = SO_Config.GetWreckSettings();
        vector playerPos = player.GetPosition();
        int itemsToSpawn = cfg.maxLootPerWreck;
        int spawned = 0;
        
        for (int attempt = 0; attempt < 20 && spawned < itemsToSpawn; attempt++)
        {
            int idx = Math.RandomInt(0, cfg.lootTable.Count());
            SO_WreckLootEntry entry = cfg.lootTable.Get(idx);

            // Chance prüfen
            if (Math.RandomFloat01() > entry.chance)
                continue;

            int qty = Math.RandomIntInclusive(entry.minQty, entry.maxQty);

            for (int i = 0; i < qty && spawned < itemsToSpawn; i++)
            {
                vector offset = Vector(Math.RandomFloat(-0.5, 0.5), 0, Math.RandomFloat(-0.5, 0.5));
                vector itemPos = playerPos + offset;
                itemPos[1] = GetGame().SurfaceY(itemPos[0], itemPos[2]);

                GetGame().CreateObjectEx(entry.item, itemPos, ECE_PLACE_ON_SURFACE);
                spawned++;
            }
        }
    }

    // Sucht das nächste Autowrack in 3m Radius
    static Object FindNearestWreck(PlayerBase player)
    {
        float radius = 3.0;
        vector playerPos = player.GetPosition();

        array<Object> objects = new array<Object>();
        array<CargoBase> proxyCargos = new array<CargoBase>();
        GetGame().GetObjectsAtPosition(playerPos, radius, objects, proxyCargos);

        Object nearest = null;
        float nearestDist = radius + 1;

        foreach (Object obj : objects)
        {
            if (IsWreck(obj))
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

    // Prüft ob ein Objekt ein Autowrack ist
    static bool IsWreck(Object obj)
    {
        if (!obj)
            return false;

        string type = obj.GetType();

        if (type.IndexOf("Wreck") != -1)
            return true;

        if (type.IndexOf("wreck") != -1)
            return true;

        return false;
    }
}