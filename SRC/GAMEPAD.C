#include "GAMEPAD.H"

char padbuff[2][34]; 

void initGamePad() {

    // init and start polling
    InitPAD(padbuff[0], 34, padbuff[1], 34);
    StartPAD();
    
    // To avoid VSync Timeout error
    ChangeClearPAD(1);
}

BUTTONS_PRESSED getControllerInput() {
    
  	PADTYPE *pad;
    BUTTONS_PRESSED buttons_pressed;

    pad = (PADTYPE*)padbuff[0];

    if (pad->stat == PAD_STATUS_CONNECTED) 
    {
        if ((pad->type == PAD_TYPE_DIGITAL) || (pad->type == PAD_TYPE_DUAL_ANALOG) || (pad->type == PAD_TYPE_DUAL_SHOCK)) 
        {
            buttons_pressed.left_pressed  = !(pad->btn & PAD_LEFT);
            buttons_pressed.right_pressed = !(pad->btn & PAD_RIGHT);
            buttons_pressed.cross_pressed = !(pad->btn & PAD_CROSS);
        }
    }

    return buttons_pressed;
}