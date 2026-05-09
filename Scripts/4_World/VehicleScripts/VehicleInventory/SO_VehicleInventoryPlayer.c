/**
 *  SO_VehicleInventory – PlayerBase (4_World)
 *  Erlaubt Inventar-Manipulation im Fahrzeug.
 */

modded class PlayerBase
{
    override bool CanManipulateInventory()
    {
        if (SO_Config.IsVehicleInventory() && GetCommand_Vehicle())
        {
            if (GetCommand_Vehicle().GetVehicleSeat() == 0)
                return false;  // Fahrer → gesperrt


            return true;
        }    

        return super.CanManipulateInventory();
    }

    override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
    {
        if (SO_Config.IsVehicleInventory() && IsInVehicle())
        {
            if (GetCommand_Vehicle().GetVehicleSeat() == 0)
                return super.CanReceiveItemIntoHands(item_to_hands);

            return true;
        }

        return super.CanReceiveItemIntoHands(item_to_hands);
    }

    override void OnCommandVehicleStart()
    {
        super.OnCommandVehicleStart();

        // Print("[Shadows_Overhaul] OnCommandVehicleStart! Seat: " + GetCommand_Vehicle().GetVehicleSeat());

        if (SO_Config.IsVehicleInventory() && GetInventory())
        {
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(TryUnlockVehicleInventory, 500, false);
            // if (GetCommand_Vehicle().GetVehicleSeat() != 0)
            //     GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
                // Print("[Shadows_Overhaul] Inventar entsperrt!");
                // Print("[Shadows_Overhaul] IsInventoryLocked: " + GetInventory().IsInventoryLocked());
                // Print("[Shadows_Overhaul] IsInventorySoftLocked: " + IsInventorySoftLocked());
        
        }
    }

    void TryUnlockVehicleInventory()
    {
        if (!GetCommand_Vehicle())
            return;

        if (GetCommand_Vehicle().GetVehicleSeat() == 0)
            return;

        while (GetInventory().IsInventoryLocked())
            {
                GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
            }
        // GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);

        Print("[Shadows_Overhaul] Delayed Unlock. IsLocked: " + GetInventory().IsInventoryLocked());
    }

    
    override bool IsInventorySoftLocked()
    {
        if (SO_Config.IsVehicleInventory() && GetCommand_Vehicle())
        {
            if (GetCommand_Vehicle().GetVehicleSeat() != 0)
                return false;
        }

        return super.IsInventorySoftLocked();
    }

    override void OnCommandVehicleFinish()
    {
        if (SO_Config.IsVehicleInventory() && GetInventory())
        {
            GetInventory().LockInventory(LOCK_FROM_SCRIPT);
        }

        super.OnCommandVehicleFinish();
    }
}