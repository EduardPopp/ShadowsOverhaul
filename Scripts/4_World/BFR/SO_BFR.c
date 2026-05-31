/**
 *  SO_BFR — Bio Fuel Refinery
 *
 *  Slots:  CanisterGasoline (Benzinkanister)
 *          CookingEquipment      (Topf)
 *          Firewood  (Feuerholz) — wird beim Craft verbraucht
 *
 *  Cargo:  Zutaten laut SO_BFRSettings (Brew, Produce, Solvent, Catalyst, Fat)
 *
 *  Ablauf: Alle 3 Slots bestücken → Zutaten ins Cargo → Craft startet automatisch
 *          → Timer läuft → Zutaten + Holz verbraucht → SO_BioFuel spawnt nach eingestellter Zeit in der Config Datei
 */

class SO_BFR extends ItemBase
{
    // =========================================================
    //  Klassenweite Variablen
    // =========================================================
    protected bool       m_IsCrafting;
    protected float      m_CraftTimer;
    protected ref Timer  m_CraftTickTimer;

    void SO_BFR()
    {
        m_IsCrafting = false;
        m_CraftTimer = 0;
    }

    // =========================================================
    //  Initialisierung des Raffineriemoduls
    // =========================================================
    override void EEInit()
    {
        super.EEInit();

        // Nach Server-Restart weitercaften
        if (m_IsCrafting && GetGame().IsServer())
            StartTimer();
    }

    override void SetActions()
    {
        super.SetActions();
        AddAction(SO_ActionCheckBFR);
    }

    // =========================================================
    //  Persistenz um laufende Craftvorgänge nicht zu verlieren wenn der Server neustartet
    // =========================================================
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(m_IsCrafting);
        ctx.Write(m_CraftTimer);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version))
            return false;

        if (!ctx.Read(m_IsCrafting))
            m_IsCrafting = false;

        if (!ctx.Read(m_CraftTimer))
            m_CraftTimer = 0;

        return true;
    }

    // =========================================================
    //  die Proxies + Inventar sperren
    // =========================================================
    override void EEItemAttached(EntityAI item, string slot_name)
    {
        super.EEItemAttached(item, slot_name);

        // Inventar des Attachments sperren (Topf, Kanister)
        if (slot_name == "CookingEquipment") {
            SetAnimationPhase("CookingEquipment", 0);
            item.GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
        }
        if (slot_name == "CanisterGasoline") {
            SetAnimationPhase("CanisterGasoline", 0);
        }
        if (slot_name == "Firewood") {
            SetAnimationPhase("Firewood", 0);
        }
        if (slot_name == "CookingTripod") {
            SetAnimationPhase("CookingTripod", 0);
            item.GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
        }
        

        // Auto-Start prüfen
        if (!GetGame().IsServer())
            return;

        if (!m_IsCrafting && CanStartCrafting())
            StartCrafting();
    }

    override void EEItemDetached(EntityAI item, string slot_name)
    {
        super.EEItemDetached(item, slot_name);

        if (slot_name == "CookingEquipment") {
            SetAnimationPhase("CookingEquipment", 1);
            item.GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
        }
        if (slot_name == "CanisterGasoline") {
            SetAnimationPhase("CanisterGasoline", 1);
        }
        if (slot_name == "Firewood") {
            SetAnimationPhase("Firewood", 1);
        }
        if (slot_name == "CookingTripod") {
            SetAnimationPhase("CookingTripod", 0);
            item.GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
        }
    }

    // =========================================================
    //  der checkt ab ob er den Craft starten kann. also alle Items dran und alle Benötigten Gegenstände im Inventar
    // =========================================================
    override void EECargoIn(EntityAI item)
    {
        super.EECargoIn(item);

        if (!GetGame().IsServer())
            return;

        if (!m_IsCrafting && CanStartCrafting())
            StartCrafting();
    }

    // =========================================================
    //  Die Bedingungen das Canister, Topf und Feuerholz vorhanden sein müssen
    // =========================================================
    bool HasRequiredAttachments()
    {
        EntityAI canister = FindAttachmentBySlotName("CanisterGasoline");
        EntityAI pot      = FindAttachmentBySlotName("CookingEquipment");
        EntityAI wood     = FindAttachmentBySlotName("Firewood");

        return (canister != null && pot != null && wood != null);
    }

    bool CanStartCrafting()
    {
        if (!HasRequiredAttachments())
            return false;

        return CheckRecipe();
    }

    // =========================================================
    //  ein kleiner check durch alle Items um zu sehen ob die zu den Kategorien zählen könnten, damit keiner mit Steinen das gleiche erzielt
    // =========================================================
    static bool IsBrew(EntityAI item)
    {
        if (item.IsKindOf("SO_Brew")) return true;
        return false;
    }

    static bool IsProduce(EntityAI item)
    {
        if (item.IsKindOf("Apple"))           return true;
        if (item.IsKindOf("GreenBellPepper")) return true;
        if (item.IsKindOf("Pear"))            return true;
        if (item.IsKindOf("Plum"))            return true;
        if (item.IsKindOf("Tomato"))          return true;
        if (item.IsKindOf("Potato"))          return true;
        if (item.IsKindOf("Zucchini"))        return true;
        if (item.IsKindOf("Pumpkin"))         return true;
        if (item.IsKindOf("SambucusBerry"))   return true;
        if (item.IsKindOf("CaninaBerry"))     return true;
        return false;
    }

    static bool IsSolvent(EntityAI item)
    {
        if (item.IsKindOf("GlasBottles"))            return true;
        if (item.IsKindOf("CanisterGasoline")) return true;
        if (item.IsKindOf("SO_StaleOil"))      return true;
        return false;
    }

    static bool IsCatalyst(EntityAI item)
    {
        if (item.IsKindOf("GardenLime"))  return true;
        if (item.IsKindOf("Fertilizer"))  return true;
        return false;
    }

    static bool IsFat(EntityAI item)
    {
        if (item.IsKindOf("Lard")) return true;
        return false;
    }

    // =========================================================
    //  check ob alle benötigten Items im Inventar liegen um es dann zu sperren
    // =========================================================
    bool CheckRecipe()
    {
        SO_BFRSettings cfg = SO_Config.GetBFRSettings();

        int countBrew     = 0;
        int countProduce  = 0;
        int countSolvent  = 0;
        int countCatalyst = 0;
        int countFat      = 0;

        CargoBase cargo = GetInventory().GetCargo();
        if (!cargo)
            return false;

        int count = cargo.GetItemCount();
        for (int i = 0; i < count; i++)
        {
            EntityAI item = cargo.GetItem(i);
            if (!item)
                continue;

            if (IsBrew(item))          countBrew++;
            else if (IsProduce(item))  countProduce++;
            else if (IsSolvent(item))  countSolvent++;
            else if (IsCatalyst(item)) countCatalyst++;
            else if (IsFat(item))      countFat++;
        }

        if ( countBrew >= cfg.reqBrew && countProduce >= cfg.reqProduce && countSolvent >= cfg.reqSolvent && countCatalyst >= cfg.reqCatalyst && countFat >= cfg.reqFat) {
            return true;
        }
        return false;
    }

    // =========================================================
    //  Der Craftingprozess an sich, mit start/Stop Timer funktionen und Craftbeginn und Craftende
    // =========================================================
    void StartCrafting()
    {
        if (m_IsCrafting)
            return;

        if (!CanStartCrafting())
            return;

        m_IsCrafting = true;
        m_CraftTimer = 0;

        // Holz verbrauchen
        EntityAI wood = FindAttachmentBySlotName("Firewood");
        if (wood)
            GetGame().ObjectDelete(wood);

        // Alles sperren
        GetInventory().LockInventory(LOCK_FROM_SCRIPT);
        StartTimer();
    }

    void StartTimer()
    {
        if (!m_CraftTickTimer)
            m_CraftTickTimer = new Timer();

        m_CraftTickTimer.Run(10.0, this, "OnCraftTick", null, true);
    }

    void StopTimer()
    {
        if (m_CraftTickTimer)
            m_CraftTickTimer.Stop();
    }

    void OnCraftTick()
    {
        if (!m_IsCrafting)
        {
            StopTimer();
            return;
        }

        m_CraftTimer += 10.0;

        if (m_CraftTimer >= SO_Config.GetBFRSettings().craftingDuration)
            FinishCrafting();
    }

    void FinishCrafting()
    {
        m_IsCrafting = false;
        m_CraftTimer = 0;

        StopTimer();
        GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);

        ConsumeIngredients();
        SpawnResult();

        // Fertig-Sound
        SEffectManager.PlaySound("pour_Water_SoundSet", GetPosition());
    }

    // =========================================================
    //  durch die Zutatenliste iterieren und dabei die Konsumierten Items löschen
    // =========================================================
    void ConsumeIngredients()
    {
        SO_BFRSettings cfg = SO_Config.GetBFRSettings();

        int usedBrew     = 0;
        int usedProduce  = 0;
        int usedSolvent  = 0;
        int usedCatalyst = 0;
        int usedFat      = 0;

        CargoBase cargo = GetInventory().GetCargo();
        if (!cargo)
            return;

        for (int i = cargo.GetItemCount() - 1; i >= 0; i--)
        {
            EntityAI item = cargo.GetItem(i);
            if (!item)
                continue;

            bool consume = false;

            if (IsBrew(item) && usedBrew < cfg.reqBrew)
            {   usedBrew++;     consume = true; }
            else if (IsProduce(item) && usedProduce < cfg.reqProduce)
            {   usedProduce++;  consume = true; }
            else if (IsSolvent(item) && usedSolvent < cfg.reqSolvent)
            {   usedSolvent++;  consume = true; }
            else if (IsCatalyst(item) && usedCatalyst < cfg.reqCatalyst)
            {   usedCatalyst++; consume = true; }
            else if (IsFat(item) && usedFat < cfg.reqFat)
            {   usedFat++;      consume = true; }

            if (consume)
                GetGame().ObjectDelete(item);
        }
    }

    // =========================================================
    //  Craftergebnis erzeugen, wenn in der Raffinerie kein Platz mehr ist, dann auf dem Boden erzeugen
    // =========================================================
    void SpawnResult()
    {
        SO_BFRSettings cfg = SO_Config.GetBFRSettings();

        for (int i = 0; i < cfg.resultAmount; i++)
        {
            EntityAI result = GetInventory().CreateInInventory("SO_BioFuel");

            if (!result)
            {
                vector pos = GetPosition();
                GetGame().CreateObjectEx("SO_BioFuel", pos, ECE_PLACE_ON_SURFACE);
            }
        }
    }

    // =========================================================
    //  STATUS-ABFRAGEN
    // =========================================================
    bool IsCrafting()
    {
        return m_IsCrafting;
    }

    float GetCraftProgress()
    {
        if (!m_IsCrafting)
            return 0;

        return Math.Clamp(m_CraftTimer / SO_Config.GetBFRSettings().craftingDuration, 0, 1);
    }

    float GetRemainingMinutes()
    {
        if (!m_IsCrafting)
            return 0;

        float remaining = SO_Config.GetBFRSettings().craftingDuration - m_CraftTimer;
        return Math.Max(0, remaining / 60.0);
    }
}