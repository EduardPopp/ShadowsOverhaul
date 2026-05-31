class CfgPatches
{
    class SO_BFR
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
    class SO_BFR_Base : Inventory_Base
    {
        scope = 0;
        displayName = "Bio Raffinery";
        descriptionShort = "Baseclass do not Spawn";
        model = "\Shadows_Overhaul\SO_Items\SO_BFR\data\SO_BFR.p3d";
        weight = 5000;

        attachments[] =
        {
            "CookingTripod",
            "CookingEquipment",
            "CanisterGasoline",
            "Firewood"
        };
        class AnimationSources
        {
            class CanisterGasoline
            {
                source = "user";
                animPeriod = 0.01;
                initPhase = 1;
            };
            class CookingEquipment
            {
                source = "user";
                animPeriod = 0.01;
                initPhase = 1;
            };
            class Firewood
            {
                source = "user";
                animPeriod = 0.01;
                initPhase = 1;
            };
            class CookingTripod
            {
                source = "user";
                animPeriod = 0.01;
                initPhase = 1;
            };
        };
        class TransportConditions
        {
            class CargoConditions
            {
                allowOwnedCargo = 1;
                allowedCargo[] = {};
                excludedCargo[] = {};
            };
        };
        //TODO später eigenen Sound nachliefern
        // class CfgSoundSets
        // {
        //     class SO_BFR_Complete_SoundSet
        //     {
        //         soundShaders[] = {"SO_BFR_Complete_SoundShader"};
        //         volumeFactor = 1.0;
        //         frequencyFactor = 1.0;
        //         spatial = 1;
        //     };
        // };

        // class CfgSoundShaders
        // {
        //     class SO_BFR_Complete_SoundShader
        //     {
        //         samples[] = {
        //             {"Shadows_Overhaul\SO_Items\SO_BFR\data\sound\craft_complete.ogg", 1}
        //         };
        //         volume = 0.8;
        //         range = 30;
        //     };
        // };

        // Inventar: 5 breit x 4 hoch = 20 Slots
        itemsCargoSize[] = {10, 7};
    };

    class SO_BFR : SO_BFR_Base
    {
        scope = 2;
        displayName = "#SO_BFR_00";
        descriptionShort = "#SO_BFR_01";
    };
};

class CfgSlots
{
    // class CanisterGasoline
    // {
    //     name = "CanisterGasoline";
    //     displayName = "#att_BFR_Canister_00";
    //     ghostIcon = "canister";
    // };
    // class Pot
    // {
    //     name = "Pot";
    //     displayName = "#att_BFR_Pot_00";
    //     ghostIcon = "cooking";
    // };
    // class Firewood
    // {
    //     name = "Firewood";
    //     displayName = "#att_BFR_Firewood_00";
    //     ghostIcon = "cooking";
    // };
};

class CfgNonAIVehicles
{
};