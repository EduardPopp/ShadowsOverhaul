class CfgPatches
{
    class SO_Tubes
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
	class SO_Tubes_Base: Inventory_Base
    {
        scope = 0;
        displayName = "Tubes";
        descriptionShort = "Baseclass do not Spawn";
        model = "\Shadows_Overhaul\SO_Items\SO_Tubes\data\hydraulic_hoses.p3d";
        weight = 1000;
		itemSize[] = {2, 2};
        isMeleeWeapon = 1;

        class DamageSystem {
			class GlobalHealth {
				class Health {
					hitpoints = 100000;
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


    class SO_Tubes : SO_Tubes_Base
    {
        scope = 2;
        displayName = "#SO_Tubes_00";
        descriptionShort= "#SO_Tubes_01";
        hiddenSelectionTextures[] = {"\Shadows_Overhaul\SO_Items\SO_Tubes\data\hydraulic_hoses_co.paa"};
        hiddenSelectionMaterials[] = {"\Shadows_Overhaul\SO_Items\SO_Tubes\data\hydraulic_hoses.rvmat"};
    };
};
class CfgSlots
{
	
};
class CfgNonAIVehicles
{
	
};