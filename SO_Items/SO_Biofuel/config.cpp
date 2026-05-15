class CfgPatches
{
    class SO_StaleOil
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
    class Bottle_Base;
    class CanisterGasoline: Bottle_Base {};

	class SO_BioFuel_Base: CanisterGasoline
    {
        scope = 0;
        displayName = "BioFuel";
        descriptionShort = "Baseclass do not Spawn";
        model = "\Shadows_Overhaul\SO_Items\SO_Biofuel\data\oil_bottle.p3d";
        weight = 2000;
        varQuantityInit = 20000;
		varQuantityMin = 0;
		varQuantityMax = 20000;
		itemSize[] = {4, 4};
        isMeleeWeapon = 1;
        hiddenSelections[] = {"zbytek"};

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


    class SO_BioFuel : SO_BioFuel_Base
    {
        scope = 2;
        displayName = "#SO_BioFuel_00";
        descriptionShort= "#SO_BioFuel_01";
        hiddenSelectionsTextures[] = {"\Shadows_Overhaul\SO_Items\SO_BioFuel\data\air_oil_bottle_piston_co.paa"};
        hiddenSelectionsMaterials[] = {"\Shadows_Overhaul\SO_Items\SO_BioFuel\data\air_oil_bottle_piston.rvmat"};
    };
};
class CfgSlots
{
	
};
class CfgNonAIVehicles
{
	
};