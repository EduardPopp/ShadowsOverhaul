/**
 *  SO_StaminaOverride
 *  Near-Infinite Stamina: Drain wird auf ~0 gesetzt.
 *  Schalter in config.json → infiniteStamina: 1/0
 */

modded class StaminaHandler
{
    private bool m_SOInfinite = false;

    void StaminaHandler(PlayerBase player)
    {
        m_SOInfinite = SO_Config.IsInfiniteStamina();
        Print("[Shadows_Overhaul] Infinite Stamina: " + m_SOInfinite.ToString());
    }

    override void DepleteStaminaEx(EStaminaModifiers modifier, float dT = -1, float coef = 1.0)
    {
        if (m_SOInfinite)
        {
            // Drain quasi eliminieren
            super.DepleteStaminaEx(modifier, dT, coef * 0.005);
        }
        else
        {
            super.DepleteStaminaEx(modifier, dT, coef);
        }
    }
}