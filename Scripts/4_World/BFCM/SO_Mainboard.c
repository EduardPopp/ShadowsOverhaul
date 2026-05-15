class SO_Mainboard extends ItemBase
{
    protected int m_RepairCount;

    void SO_Mainboard()
    {
        m_RepairCount = 0;
    }

    // =========================================================
    // PERSISTENZ
    // =========================================================
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(m_RepairCount);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version))
            return false;

        if (!ctx.Read(m_RepairCount))
            m_RepairCount = 0;

        return true;
    }

    // =========================================================
    // REPARATUR-LOGIK
    // =========================================================
    int GetRepairCount()
    {
        return m_RepairCount;
    }

    bool CanBeRepaired()
    {
        // Nur 2 Reparaturen moeglich (0 -> 1, 1 -> 2)
        return (m_RepairCount < 2) && !IsRuined();
    }

    float GetMaxHealthForRepairCount()
    {
        switch (m_RepairCount)
        {
            case 0: return 0.75;  // 1. Reparatur: max 75%
            case 1: return 0.25;  // 2. Reparatur: max 25%
        }
        return 0.0;              // ab 2: nicht mehr reparierbar
    }

    void DoRepair()
    {
        if (!CanBeRepaired())
            return;

        float maxHP = GetMaxHealth("", "Health");
        float newHealth = maxHP * GetMaxHealthForRepairCount();

        SetHealth("", "Health", newHealth);
        m_RepairCount++;
    }

    // =========================================================
    // ZERSTOERUNG -> MOTOR STOPPEN + BLOCKIEREN
    // =========================================================
    override void EEKilled(Object killer)
    {
        super.EEKilled(killer);

        // BFCM ist der direkte Parent
        EntityAI bfcm = EntityAI.Cast(GetHierarchyParent());
        if (!bfcm)
            return;

        // Das Auto ist der Parent vom BFCM
        EntityAI vehicle = EntityAI.Cast(bfcm.GetHierarchyParent());
        if (!vehicle)
            return;

        CarScript car = CarScript.Cast(vehicle);
        if (car)
        {
            // Motor sofort stoppen
            car.EngineStop();

            // Motorstart blockieren via Synced-Variable
            car.SetSO_EngineBlocked(true);
        }
    }
}