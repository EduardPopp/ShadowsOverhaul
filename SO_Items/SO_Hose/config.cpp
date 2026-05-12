class CfgPatches
{
    class SO_Hose
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
	class SO_Hose_Base: Inventory_Base
    {
        scope = 0;
        displayName = "Hose";
        descriptionShort = "Baseclass do not Spawn";
        model = "\Shadows_Overhaul\SO_Items\SO_Hose\data\SO_Hose.p3d";
        weight = 2000;
		itemSize[] = {2, 2};
        isMeleeWeapon = 1;
        hiddenSelections[] = {"Zbytek"};

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


    class SO_Hose : SO_Hose_Base
    {
        scope = 2;
        displayName = "#SO_Hose_00";
        descriptionShort= "#SO_Hose_01";
        hiddenSelectionTextures[] = {"\Shadows_Overhaul\SO_Items\SO_Hose\data\SO_Hose_co.paa"};
        hiddenSelectionMaterials[] = {"\Shadows_Overhaul\SO_Items\SO_Hose\data\SO_Hose.rvmat"};
    };
};
class CfgSlots
{
	
};
class CfgNonAIVehicles
{
	
};