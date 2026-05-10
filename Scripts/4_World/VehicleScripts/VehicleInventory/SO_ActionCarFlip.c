/**
 *  SO_ActionCarFlip
 *  Stellt umgekippte Fahrzeuge wieder auf.
 *  Spieler schaut Auto an → "Fahrzeug aufrichten" → gedrückt halten → Auto steht.
 */

class SO_ActionCarFlipCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float duration = SO_Config.GetCarFlipSeconds();
        m_ActionData.m_ActionComponent = new CAContinuousTime(duration);
    }
}

class SO_ActionCarFlip : ActionContinuousBase
{
    void SO_ActionCarFlip()
    {
        m_CallbackClass     = SO_ActionCarFlipCB;
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_CRAFTING;
        m_FullBody          = false;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text              = "#Flip_00";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem   = new CCINone();
        m_ConditionTarget = new CCTNone();
    }

    override typename GetInputType()
    {
        return ContinuousInteractActionInput;
    }

    override bool HasTarget()
    {
        return true;
    }

    override string GetText()
    {
        return "#Flip_00";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!SO_Config.IsCarFlipEnabled())
            return false;

        CarScript car = CarScript.Cast(target.GetObject());
        if (!car)
            return false;

        // Ist das Auto umgekippt?
        if (!IsFlipped(car))
            return false;

        // Spieler darf nicht im Auto sitzen
        if (player.GetCommand_Vehicle())
            return false;

        return true;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        if (!player)
            return;

        CarScript car = CarScript.Cast(action_data.m_Target.GetObject());
        if (!car)
            return;

        // Auto aufrichten
        vector ori = car.GetOrientation();

        // Rotation zurücksetzen (nur Yaw behalten, Pitch und Roll auf 0)
        ori[1] = 0;  // Pitch
        ori[2] = 0;  // Roll
        car.SetOrientation(ori);

        vector pos = car.GetPosition();
        pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 0.10;
        
        car.SetPosition(pos);
        car.PlaceOnSurface();
    }

    // Prüft ob das Fahrzeug auf der Seite oder dem Dach liegt
    static bool IsFlipped(CarScript car)
    {
        vector up = car.GetDirectionUp();
        // Dot product mit Welt-Up: 1.0 = aufrecht, 0 = auf der Seite, -1 = auf dem Dach
        float dot = vector.Dot(up, "0 1 0");
        // Unter 0.5 = mehr als ~60 Grad gekippt
        return dot < 0.5;
    }
}