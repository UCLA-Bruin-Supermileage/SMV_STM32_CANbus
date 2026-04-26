#ifndef __SMV_BOARD_ENUMS_H
#define __SMV_BOARD_ENUMS_H

// enumerators for SMV boards.
// pick the appropriate one for your board
// every time you send a message, pick the appropriate data type for that message.

enum devices {
    Safety,
    UI,
    FC,
    RC,
    Motor_Controller,
    Joule_H,
    Joule_L,
    DAQ_Board
};

enum motorMessage {
    Hall_Velocity,
    Motor_Torque,
    Motor_Current,
    Board_Temp,
    Motor_Temp,
};

enum UIMessage {
    Emergency_Stop,
    Supercap_Discharge,
    Blink_Left,
    Blink_Right,
    Reverse,
    Headlights,
    Wipers,
    Horn,
    Hazard,
    Spare_Button,
    Spare_Switch,
    Regen,
    DAQ_Button
};

enum FCMessage {
    Gas,
    Brake,
    FC_Pressure
};

enum RCMessage{
    RC_Pressure,
    RC_Torque
};

enum JouleMessage {
    Power
};

enum DAQMessage {
    Longitude,
    Latitude,
    Altitude
};

#endif
