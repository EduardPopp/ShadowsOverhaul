class SO_ActionCheckBFR extends ActionSingleUseBase
{
    void SO_ActionCheckBFR()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_Text       = "#SO_BFR_Check_00";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem   = new CCINone();
        m_ConditionTarget = new CCTNonRuined();
    }

    override typename GetInputType()
    {
        return InteractActionInput;
    }

    override bool HasTarget()
    {
        return true;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        SO_BFR refinery = SO_BFR.Cast(target.GetObject());
        if (!refinery)
            return false;

        return refinery.IsCrafting();
    }

    override void OnExecuteServer(ActionData action_data)
    {
        SO_BFR refinery = SO_BFR.Cast(action_data.m_Target.GetObject());
        if (!refinery)
            return;

        float minutes = refinery.GetRemainingMinutes();
        string msg;

        if (minutes > 1)
            msg = "Noch etwa " + Math.Round(minutes).ToString() + " Minuten...";
        else
            msg = "Fast fertig...";

        GetGame().GetMission().OnEvent(
            ChatMessageEventTypeID,
            new ChatMessageEventParams(CCDirect, "", msg, "")
        );
    }
}