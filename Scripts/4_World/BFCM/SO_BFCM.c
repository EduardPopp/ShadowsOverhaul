class SO_BFCM_Base extends ItemBase
{
    // =========================================================
    // SUB-ATTACHMENT AENDERUNGEN AN DAS AUTO WEITERLEITEN
    // =========================================================
    override void EEItemAttached(EntityAI item, string slot_name)
    {
        super.EEItemAttached(item, slot_name);

        // Wenn ein neues Mainboard eingebaut wird -> Auto informieren
        if (slot_name == "SOMainboard")
        {
            NotifyParentVehicle();
        }
    }

    override void EEItemDetached(EntityAI item, string slot_name)
    {
        super.EEItemDetached(item, slot_name);

        if (slot_name == "SOMainboard")
        {
            NotifyParentVehicle();
        }
    }

    protected void NotifyParentVehicle()
    {
        EntityAI vehicle = EntityAI.Cast(GetHierarchyParent());
        if (!vehicle)
            return;

        CarScript car = CarScript.Cast(vehicle);
        if (car)
        {
            car.OnAttachmentChanged();
        }
    }
}