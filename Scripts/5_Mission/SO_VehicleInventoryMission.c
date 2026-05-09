/**
 *  SO_VehicleInventory – MissionGameplay (5_Mission)
 *  Umgeht den CanOpenInventory-Check im Fahrzeug.
 */

modded class MissionGameplay
{
    override void ShowInventory()
    {
        if (SO_Config.IsVehicleInventory())
        {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            if (player && player.GetCommand_Vehicle())
            {
                if (player.GetCommand_Vehicle().GetVehicleSeat() == 0)
                {
                    // Fahrer → normales Verhalten (gesperrt)
                    super.ShowInventory();
                    return;
                }

                UIScriptedMenu menu = GetUIManager().GetMenu();
                if (!menu)
                {
                    if (!m_InventoryMenu)
                    {
                        InitInventory();
                    }

                    if (!GetUIManager().FindMenu(MENU_INVENTORY))
                    {
                        GetUIManager().ShowScriptedMenu(m_InventoryMenu, null);
                        player.OnInventoryMenuOpen();
                    }

                    AddActiveInputExcludes({"inventory"});
                    AddActiveInputRestriction(EInputRestrictors.INVENTORY);
                    return;
                }
            }
        }

        super.ShowInventory();
    }
}