class CfgPatches
{
    class SO_Mainboard
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
    class Inventory_Base;
	class SO_Mainboard_Base: Inventory_Base
    {
        scope = 0;
        displayName = "Mainboard";
        descriptionShort = "Baseclass do not Spawn";
        model = "\Shadows_Overhaul\SO_Items\SO_Mainboard\data\tire_repair_kit.p3d";
        weight = 500;
        inventorySlot[] = {"SOMainboard"};
		itemSize[] = {2, 2};
        rotationFlags = 17;
        isMeleeWeapon = 1;
        hiddenSelections[] = {"zbytek"};

        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints = 100;
                    healthLevels[] =
                    {
                        {1.0, {}},
                        {0.7, {}},
                        {0.5, {}},
                        {0.3, {}},
                        {0.0, {}}
                    };
                };
            };
        };

        class MeleeModes {
			class Default {
				ammo = "MeleeLightBlunt";
				range = 1.0;
			};
			
			class Heavy {
				ammo = "MeleeLightBlunt_Heavy";
				range = 1.0;
			};
			
			class Sprint {
				ammo = "MeleeLightBlunt_Heavy";
				range = 2.8;
			};
		};
    };


    class SO_Mainboard : SO_Mainboard_Base
    {
        scope = 2;
        displayName = "#SO_Mainboard_00";
        descriptionShort= "#SO_Mainboard_01";
        hiddenSelectionsTextures[] = {"\Shadows_Overhaul\SO_Items\SO_Mainboard\data\tire_repair_kit_co.paa"};
        hiddenSelectionsMaterials[] = {"\Shadows_Overhaul\SO_Items\SO_Mainboard\data\tire_repair_kit.rvmat"};
    };
};
class CfgSlots
{
	
};
class CfgNonAIVehicles
{
	
};