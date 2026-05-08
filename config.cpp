/**
 *  SG Overhaul – Stamina Module
 *  Konfigurierbares Staminasystem als Alternative zu Expansion.
 *  Serveradmin wählt per JSON-Config welches System aktiv ist.
 */

class CfgPatches
{
    class Shadows_Overhaul
    {
        units[]           = {};
        weapons[]         = {};
        requiredVersion   = 0.1;
        requiredAddons[]  =
        {
            "DZ_Data",
            "DZ_Scripts"
        };
    };
};

class CfgMods
{
    class Shadows_Overhaul
    {
        type       = "mod";
        name       = "Shadows_Overhaul";
        dir        = "Shadows_Overhaul";
        author     = "EddeShadow";

        dependencies[] = { "World", "Mission" };

        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = { "Shadows_Overhaul/Scripts/4_World" };
            };
        };
    };
};