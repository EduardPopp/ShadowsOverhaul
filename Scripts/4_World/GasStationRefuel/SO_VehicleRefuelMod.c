modded class CarScript
{
    override void InitializeActions()
    {
        super.InitializeActions();
        if (SO_Config.IsGasPumpEnabled())
        {
            AddAction(SO_ActionRefuelAtPump);
        }
    }
}