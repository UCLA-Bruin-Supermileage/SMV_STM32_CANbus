#ifndef __SMVCANBUS_H
#define __SMVCANBUS_H

#include "stm32f4xx_hal.h"
#include "smv_board_enums.h"
#include "smv_can_error.h"

#define CAN_2_FILTER_BANK_INDEX 14
#define CAN_1_MAX_FILTER_BANK_INDEX 13
#define CAN_MAX_FILTER_BANK_INDEX 27
#define CAN_2_MAX_FILTER_BANK_INDEX (CAN_MAX_FILTER_BANK_INDEX)


typedef struct CANBUS CANBUS;
struct CANBUS {

    uint32_t              TxMailbox;       /* The number of the mail box that transmitted the Tx message */
    CAN_TxHeaderTypeDef   TxHeader;        /* Header containing the information of the transmitted frame */
    uint8_t               TxData[8]; /* Buffer of the data to send */
    CAN_RxHeaderTypeDef   RxHeaderFIFO0;   /* Header containing the information of the received frame */
    uint8_t               RxDataFIFO0[8];  /* Buffer of the received data */
    CAN_RxHeaderTypeDef   RxHeaderFIFO1;   /* Header containing the information of the received frame */
    uint8_t               RxDataFIFO1[8];  /* Buffer of the received data */
    CAN_HandleTypeDef *hcan;
    CAN_FilterTypeDef  sFilterConfig;
    uint8_t instance;


    int device_id;
    int rec_hardware;
    int rec_dataType;

    /* MESSAGE DATA */
    char hardware[20]; // hardware type from the incoming message
    char dataType[20]; // datatype from the incoming
    double data; // data from incoming message
    uint8_t filter_bank; //keep track of which filter bank to fill next; we want to keep it between 0 and 14
    uint8_t max_filter_bank;

    /* ERROR HANDLING */
    CAN_Error *err; // pointer to error struct, defined in smv_can_error.h

    /* METHODS */
    void (*init)(CANBUS*, int, CAN_HandleTypeDef*, CAN_Error*); /* initialize the CAN bus driver @param CANBUS* pointer to your CANBUS object @param int your board's ID. reference the enums. @param CAN_HandleTypeDef* pointer to your STM32 generated CAN handler @param CAN_Error* pointer to your CAN error object */
    void (*begin)(CANBUS*); /* CAN bus begins running in normal mode @param CANBUS* pointer to your CANBUS object */
    double (*getData)(CANBUS*); /* get incoming message from CAN bus line @param CANBUS* pointer to your CANBUS object */
    int (*getDataTypeRaw)(CANBUS*); /*get raw integer value of data type received*/
    int (*getHardwareRaw)(CANBUS*); /*get raw integer value of hardware received*/
    char* (*getDataType)(CANBUS*); /* get data type of incoming message from CAN bus line @param CANBUS* pointer to your CANBUS object */
    char* (*getHardware)(CANBUS*); /* get data type of incoming message from CAN bus line @param CANBUS* pointer to your CANBUS object */
    void (*addFilterDevice)(CANBUS*, int, uint32_t); /* receive data only from the device specified, filters everything else @param CANBUS* pointer to your CANBUS object @param int the board ID to listen to. reference the enums. */
    void (*addFilterDeviceData)(CANBUS*, int, int, uint32_t); /* receive only the specified type data only from the device specified, filters everything else @param CANBUS* pointer to your CANBUS object @param int the board ID to listen to. reference the enums. @param int the datatype ID to listen to. reference the enums. */
    void (*send)(CANBUS*, double, uint8_t); /* send data to the CAN bus @param CANBUS* pointer to your CANBUS object @param double the data you want to send @param uint8_t the type of data you are sending. reference the enums. */

};

/*
Constructor workaround in C. Call this when creating your CAN object, and then proceed with the rest of the example.
*/
CANBUS CAN_new();
CANBUS CAN_new_dual (int can_index);

/*
- CubeMX defines a CAN interrupt handler when the programmer enables the interrupt in NVIC settings in ioc
- Any definition of the Fifo0PendingCallback function will override the default definition, if any
- The programmer will define the Fifo0PendingCallback function with GetRxMessage and then call this helper function inside

Purpose:
- Assign the new message to our RxData array
- Use DoubleCaster union to assign the byte array value to the data variable
- Assign the incoming header to our RxHeader object
- Extract the integer values of device_id and data_type encoded in the RxHeader.StdId
    - Look up which strings they are both associated with and assign the appropriate values to hardware[] and dataType[]

*/
void CAN_Interrupt_Helper(CANBUS *can);

#endif
