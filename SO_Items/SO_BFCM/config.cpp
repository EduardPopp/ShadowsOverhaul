class CfgPatches
{

    class SO_BFCM
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = 
        {
            "DZ_Data",
            "DZ_Scripts"
        };
    };
};

class CfgVehicles
{
    class Bottle_Base;
    class CanisterGasoline: Bottle_Base
    {
        inventorySlot[] = {
            "CanisterGasoline"
        };
    };

    class Inventory_Base;
	class SO_BFCM_Base: Inventory_Base
    {
        scope = 0;
        displayName = "The BFCM";
        descriptionShort = "Baseclass do not Spawn";
        model = "\Shadows_Overhaul\SO_Items\SO_BFCM\data\helicopter_battery.p3d";
        weight = 15000;
		itemSize[] = {3, 3};
        attachments[] = {
            "SOMainboard", 
            "BatteryD", 
            "GasMaskFilter",
            "CanisterGasoline"
        };
        inventorySlot[] = {"BFCM"};
        // hiddenSelections[] = {"zbytek"};

    }

    class SO_BFCM : SO_BFCM_Base
    {
        scope = 2;
        displayName = "#SO_BFCM_00";
        descriptionShort= "#SO_BFCM_01";
        // hiddenSelectionsTextures[] = {"\Shadows_Overhaul\SO_Items\SO_BFCM\data\helicopter_battery_co.paa"};
        // hiddenSelectionsMaterials[] = {"\Shadows_Overhaul\SO_Items\SO_BFCM\data\helicopter_battery.rvmat"};
    };
};

class CfgSlots
{
    class SOMainboard
    {
        name = "SOMainboard";
        displayName = "#att_SOMainboard_00";
        ghostIcon = "gorkavisor";
    };
    class CanisterGasoline
    {
        name = "CanisterGasoline";
        displayName = "#att_SOGasoline_00";
        ghostIcon = "gascanister";
    };

	
};
class CfgNonAIVehicles
{
	
};