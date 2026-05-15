modded class ActionStartEngine
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!super.ActionCondition(player, target, item))
            return false;

        CarScript car = CarScript.Cast(target.GetObject());
        if (car && !car.HasWorkingMainboard())
            return false;

        if (car && car.IsSO_EngineBlocked())
            return false;

        return true;
    }
}