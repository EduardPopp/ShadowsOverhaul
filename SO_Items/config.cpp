/**
 *  SG Overhaul – Stamina Module
 *  Konfigurierbares Staminasystem als Alternative zu Expansion.
 *  Serveradmin wählt per JSON-Config welches System aktiv ist.
 */

class CfgPatches
{
    class SO_Items
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