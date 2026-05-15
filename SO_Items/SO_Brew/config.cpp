class CfgPatches
{
    class SO_Brew
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

	class SO_Brew_Base : Bottle_Base
    {
        scope = 0;
        displayName = "Brew";
        descriptionShort = "Baseclass do not Spawn";
        model = "\Shadows_Overhaul\SO_Items\SO_Brew\data\brake_fluid.p3d";
        weight = 2500;
		itemSize[] = {1, 2};
        isMeleeWeapon = 1;
        hiddenSelections[] = {"zbytek"};
        varLiquidTypeInit = 512;       // eigener Liquid-Type (freie Zahl die nicht vanilla belegt ist)
        varQuantityInit = 5000;
        varQuantityMin = 0;
        varQuantityMax = 5000;
        varQuantityDestroyOnMin = 0;   // Flasche bleibt wenn leer

        class AnimationPhases
        {
            class Filling
            {
                source = "Filling";
                animPeriod = 0.01;
                initPhase = 0;
            };
        };

    };

    class SO_Brew : SO_Brew_Base
    {
        scope = 2;
        displayName = "#SO_Brew_00";
        descriptionShort= "#SO_Brew_01";
        hiddenSelectionsTextures[] = {"\Shadows_Overhaul\SO_Items\SO_Brew\data\SO_Brew_co.paa"};
        hiddenSelectionsMaterials[] = {"\Shadows_Overhaul\SO_Items\SO_Brew\data\brake_fluid.rvmat"};
    };
};

class CfgSlots
{
	
};
class CfgNonAIVehicles
{
	
};