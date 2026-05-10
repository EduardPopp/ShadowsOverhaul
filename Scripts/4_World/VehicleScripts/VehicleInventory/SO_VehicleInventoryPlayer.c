/**
 *  SO_VehicleInventory – PlayerBase (4_World)
 *  Erlaubt Inventar-Manipulation im Fahrzeug.
 */

modded class PlayerBase
{
    private int m_SOLockCount = 0;
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

        m_SOLockCount = 0; 
        if (SO_Config.IsVehicleInventory() && GetInventory() && GetCommand_Vehicle())
        {
            // GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(TryUnlockVehicleInventory, 500, false);
        
            if (GetCommand_Vehicle().GetVehicleSeat() != 0)
            {
                GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(TryUnlockVehicleInventory, 500, false);
            }
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

        m_SOLockCount = 0;
        while (GetInventory().IsInventoryLocked())
            {
                GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
                m_SOLockCount++;
                if (m_SOLockCount > 10)
                    break;
            }
        // GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
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
            for (int i = 0; i < m_SOLockCount; i++)
            {
                GetInventory().LockInventory(LOCK_FROM_SCRIPT);
            }
            m_SOLockCount = 0;
        }

        super.OnCommandVehicleFinish();
    }


    override void SetActions()
    {
        super.SetActions();

        if (SO_Config.IsWreckDismantleEnabled())
            AddAction(SO_ActionDismantleWreck);

        if (SO_Config.IsSiphonEnabled())
            AddAction(SO_ActionSiphonFuel);
    }
}