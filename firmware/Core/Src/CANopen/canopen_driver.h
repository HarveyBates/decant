#ifndef CANOPEN_DRIVER_H
#define CANOPEN_DRIVER_H

#include "stm32f4xx_hal.h"
#include "usbd_cdc_if.h"
#include "cmsis_os2.h"

#define CANOPEN_DEBUG

typedef enum {
  CANOPEN_OK,
  CANOPEN_ERR,
  CANOPEN_FILTER_ERR,
  CANOPEN_START_ERR,
  CANOPEN_RX_CB_SETUP_ERR,
  CANOPEN_QUEUE_INIT_ERR,
  CANOPEN_TX_ERR,
  CANOPEN_RX_ERR,
  CANOPEN_QUEUE_FULL
} CANopenStatus;

typedef enum {
  CANOPEN_NMT           = 0x000,
  CANOPEN_SYNC          = 0x080,
  CANOPEN_EMERGENCY     = 0x080,
  CANOPEN_TIMESTAMP     = 0x100,
  CANOPEN_PDO1_TRANSMIT = 0x180,
  CANOPEN_PDO1_RECEIVE  = 0x200,
  CANOPEN_PDO2_TRANSMIT = 0x280,
  CANOPEN_PDO2_RECEIVE  = 0x300,
  CANOPEN_PDO3_TRANSMIT = 0x380,
  CANOPEN_PDO3_RECEIVE  = 0x400,
  CANOPEN_PDO4_TRANSMIT = 0x480,
  CANOPEN_PDO4_RECEIVE  = 0x500,
  CANOPEN_SDO_TRANSMIT  = 0x580,
  CANOPEN_SDO_RECEIVE   = 0x600
} CANopenFunctionCode;

#ifdef CANOPEN_DEBUG
CANopenStatus CANopen_init(CAN_HandleTypeDef* _hcan, UART_HandleTypeDef* _huart);
#else
  CANopenStatus CANopen_init(CAN_HandleTypeDef* _hcan);
#endif

CANopenStatus CANopen_Transmit(CANopenFunctionCode FC,
                               uint32_t NODE_ID,
                               const uint8_t* data,
                               uint8_t data_size);

void CANopen_Queue_Process();

#endif // CANOPEN_DRIVER_H
