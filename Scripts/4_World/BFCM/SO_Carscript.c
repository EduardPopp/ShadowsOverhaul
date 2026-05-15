modded class CarScript
{
    protected bool m_SO_EngineBlocked;

    void CarScript()
    {
        m_SO_EngineBlocked = false;
    }

    // =========================================================
    // GETTER / SETTER
    // =========================================================
    void SetSO_EngineBlocked(bool blocked)
    {
        m_SO_EngineBlocked = blocked;
    }

    bool IsSO_EngineBlocked()
    {
        return m_SO_EngineBlocked;
    }

    // =========================================================
    // PERSISTENZ
    // =========================================================
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(m_SO_EngineBlocked);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version))
            return false;

        if (!ctx.Read(m_SO_EngineBlocked))
            m_SO_EngineBlocked = false;

        return true;
    }

    // =========================================================
    // MAINBOARD-STATUS PRUEFEN
    // =========================================================
    bool HasWorkingMainboard()
    {
        // BFCM im Fahrzeug finden
        EntityAI bfcm = FindAttachmentBySlotName("BFCM");
        if (!bfcm)
            return false;

        // Mainboard im BFCM finden
        EntityAI mainboard = bfcm.FindAttachmentBySlotName("SOMainboard");
        if (!mainboard)
            return false;

        // Mainboard darf nicht zerstoert sein
        if (mainboard.IsRuined())
            return false;

        return true;
    }

    // =========================================================
    // ATTACHMENT-EVENTS: Block aufheben wenn neues Mainboard
    // =========================================================
    override void EEItemAttached(EntityAI item, string slot_name)
    {
        super.EEItemAttached(item, slot_name);

        // Wenn ein BFCM eingebaut wird, pruefen ob es ein gutes Mainboard hat
        if (slot_name == "BFCM")
        {
            CheckAndUnblockEngine();
        }
    }

    // Wird auch getriggert wenn Sub-Attachments sich aendern
    void OnAttachmentChanged()
    {
        CheckAndUnblockEngine();
    }

    void CheckAndUnblockEngine()
    {
        if (HasWorkingMainboard())
        {
            m_SO_EngineBlocked = false;
        }
    }
}