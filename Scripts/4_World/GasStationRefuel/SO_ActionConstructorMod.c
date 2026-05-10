/**
 *  Registriert alle SO Refuel-Actions in der globalen Action-Registry.
 */

modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

        actions.Insert(SO_ActionRefuelAtPump);
        actions.Insert(SO_ActionCarFlip);
        actions.Insert(SO_ActionSiphonFuel);
        actions.Insert(SO_ActionDismantleWreck);
        Print("[Shadows_Overhaul] Actions registered!");
    }
}