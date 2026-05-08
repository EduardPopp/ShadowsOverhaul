/**
 *  Registriert alle SO Refuel-Actions in der globalen Action-Registry.
 */

modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

        actions.Insert(SO_ActionRefuelAtPump);
        Print("[Shadows_Overhaul] Refuel Action registriert!");
    }
}