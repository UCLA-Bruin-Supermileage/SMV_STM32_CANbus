#ifndef __SMV_CAN_ERROR_H
#define __SMV_CAN_ERROR_H

#include <stdint.h>

typedef enum {
    /* ---- no fault ---- */
    CAN_ERR_NONE = 0,

    /* ---- init-time faults (hardware / config, must fix before run) ---- */
    CAN_ERR_INIT_FAILED,   // HAL_CAN_Init() returned HAL_ERROR
                           //   cause: wrong GPIO/AF mapping, bad clock,
                           //          RX pin floating without transceiver
    CAN_ERR_FILTER_FAILED, // HAL_CAN_ConfigFilter() returned HAL_ERROR
                           //   cause: filter_bank > 13, or CAN not yet init
    CAN_ERR_START_FAILED,  // HAL_CAN_Start() returned HAL_ERROR
                           //   cause: INAK bit never cleared; RX pin float,
                           //          transceiver unpowered, wrong GPIO
    CAN_ERR_NOTIFY_FAILED, // HAL_CAN_ActivateNotification() failed
                           //   cause: called before HAL_CAN_Start

    /* ---- runtime TX faults (recoverable, RX stays alive) ---- */
    CAN_ERR_TX_MAILBOX_TIMEOUT, // all 3 TX mailboxes stayed full past deadline
                                //   cause: bus dead, no ACK, frames piling up
    CAN_ERR_TX_ADD_FAILED,      // HAL_CAN_AddTxMessage() returned HAL_ERROR
                                //   cause: mailboxes full, or wrong CAN state
    CAN_ERR_TX_BUS_OFF,         // TEC > 255, hardware entered Bus-Off
                                //   in this state bxCAN cannot TX or RX;
                                //   library blocks TX and attempts recovery
    CAN_ERR_TX_MAX_RETRIES,     // consecutive TX failures exceeded retry limit

    /* ---- runtime RX faults ---- */
    CAN_ERR_RX_FAILED, // HAL_CAN_GetRxMessage() returned HAL_ERROR
                       //   cause: FIFO overrun, wrong FIFO selected,
                       //          or CAN state error

    /* ---- logic / programmer errors ---- */
    CAN_ERR_FILTER_BANK_FULL, // addFilterDevice* called more than 14 times
                              //   bxCAN only has filter banks 0-13 for CAN1

} CAN_ErrorCode;

typedef struct {
    CAN_ErrorCode code;       // most recent error (CAN_ERR_NONE = healthy)
    CAN_ErrorCode last_fatal; // last init-time fault (never cleared)
    uint8_t tx_retries;       // consecutive TX failures since last success
    uint8_t is_bus_off;       // 1 while hardware is in Bus-Off recovery
    uint32_t last_error_tick; // HAL_GetTick() value when code was set
    uint32_t error_count;     // total lifetime errors (never wraps, saturates)
} CAN_Error;

void CAN_Error_Record(CAN_Error *err, CAN_ErrorCode code);

void CAN_Error_Clear(CAN_Error *err);

const char *CAN_Error_GetString(CAN_ErrorCode code);

#endif /* __SMV_CAN_ERROR_H */