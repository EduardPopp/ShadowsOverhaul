class ActionRepairMainboardCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        // Reparaturdauer: 5 Sekunden
        m_ActionData.m_ActionComponent = new CAContinuousTime(5.0);
    }
}

class ActionRepairMainboard extends ActionContinuousBase
{
    void ActionRepairMainboard()
    {
        m_CallbackClass      = ActionRepairMainboardCB;
        m_CommandUID          = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
        m_FullBody            = true;
        m_StanceMask          = DayZPlayerConstants.STANCEMASK_CROUCH;
        m_SpecialtyWeight     = UASoftSkillsWeight.PRECISE_LOW;
        m_Text                = "#repair";  // Lokalisierung anpassen
    }

    override void CreateConditionComponents()
    {
        // Item in Hand: muss intakt sein (Reparaturkit)
        m_ConditionItem   = new CCINonRuined();
        // Ziel: muss existieren und nicht komplett zerstoert
        m_ConditionTarget = new CCTNonRuined();
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        // Ziel muss ein SO_Mainboard sein
        SO_Mainboard board = SO_Mainboard.Cast(target.GetObject());
        if (!board)
            return false;

        // Item in Hand muss das Elektronik-Reparaturkit sein
        if (!item.IsKindOf("ElectronicRepairKit"))  // Klassenname anpassen!
            return false;

        // Board muss reparierbar sein
        if (!board.CanBeRepaired())
            return false;

        return true;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        SO_Mainboard board = SO_Mainboard.Cast(action_data.m_Target.GetObject());
        if (!board)
            return;

        // Reparatur ausfuehren
        board.DoRepair();

        // Reparaturkit abnutzen
        if (action_data.m_MainItem)
        {
            action_data.m_MainItem.DecreaseHealth("", "Health", 25);
        }

        // Auto-Blockade aufheben wenn Board jetzt funktioniert
        EntityAI bfcm = EntityAI.Cast(board.GetHierarchyParent());
        if (bfcm)
        {
            EntityAI vehicle = EntityAI.Cast(bfcm.GetHierarchyParent());
            CarScript car = CarScript.Cast(vehicle);
            if (car)
            {
                car.CheckAndUnblockEngine();
            }
        }
    }
}