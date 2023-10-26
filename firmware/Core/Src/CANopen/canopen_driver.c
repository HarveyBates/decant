#include "canopen_driver.h"

CAN_HandleTypeDef* hcan;
#ifdef CANOPEN_DEBUG
UART_HandleTypeDef* huart;
#endif

const uint8_t QUEUE_SIZE = 16;

/// CAN Message
typedef struct {
  CAN_RxHeaderTypeDef header;
  uint8_t data[8]; // CAD-FD has 64 bytes (handle later on)
} CANopenMessage;

/// Queue of received messages
osMessageQueueId_t rxQueue;

CANopenStatus CANopen_apply_filters();

uint32_t create_cob_id(CANopenFunctionCode FC, uint32_t CAN_ID) {
  return (uint32_t)FC | CAN_ID;
}

#ifdef CANOPEN_DEBUG
CANopenStatus CANopen_init(CAN_HandleTypeDef* _hcan, UART_HandleTypeDef* _huart) {
  if (!_huart) return CANOPEN_ERR;
  huart = _huart;
#else
  CANopenStatus CANopen_init(CAN_HandleTypeDef* _hcan) {
#endif

  CANopenStatus can_status;
  HAL_StatusTypeDef hal_status;

  if (!_hcan) return CANOPEN_ERR;
  hcan = _hcan;

  can_status = CANopen_apply_filters();
  if (can_status != CANOPEN_OK) {
    return can_status;
  }

  hal_status = HAL_CAN_Start(hcan);
  if (hal_status != HAL_OK) {
    return CANOPEN_START_ERR;
  }

  hal_status = HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
  if (hal_status != HAL_OK) {
    return CANOPEN_RX_CB_SETUP_ERR;
  }

  // Setup FreeRTOS message queue
  rxQueue = osMessageQueueNew(QUEUE_SIZE, sizeof(CANopenMessage), NULL);
  if (!rxQueue) {
    return CANOPEN_QUEUE_INIT_ERR;
  }

  return CANOPEN_OK;
}

CANopenStatus CANopen_apply_filters() {
  CAN_FilterTypeDef filter;

  // Default filter, accepts all messages
  filter.FilterBank = 0;
  filter.FilterMode = CAN_FILTERMODE_IDMASK;
  filter.FilterFIFOAssignment = CAN_RX_FIFO0;
  filter.FilterIdHigh = 0;
  filter.FilterIdLow = 0;
  filter.FilterMaskIdHigh = 0;
  filter.FilterMaskIdLow = 0;
  filter.FilterScale = CAN_FILTERSCALE_32BIT;
  filter.FilterActivation = ENABLE;
  filter.SlaveStartFilterBank = 14;

  if (HAL_CAN_ConfigFilter(hcan, &filter) != HAL_OK) {
    return CANOPEN_FILTER_ERR;
  }

  return CANOPEN_OK;
}

// Transmit
CANopenStatus CANopen_Transmit(const CANopenFunctionCode FC,
                               const uint32_t NODE_ID,
                               const uint8_t* data,
                               uint8_t data_size) {
  CAN_TxHeaderTypeDef   tx_header;
  uint8_t               tx_data[8];
  uint32_t              tx_mailbox;

  tx_header.IDE = CAN_ID_STD;
  tx_header.StdId = create_cob_id(FC, NODE_ID);
  tx_header.RTR = CAN_RTR_DATA;
  tx_header.DLC = data_size;

  // Constrain data size to <= 8
  data_size = (data_size > 8) ? 8 : data_size;

  for (uint8_t i = 0; i < data_size; i++) {
    tx_data[i] = data[i];
  }

  if (HAL_CAN_AddTxMessage(hcan, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
    return CANOPEN_TX_ERR;
  }

  return CANOPEN_OK;
}

// Receive via interrupt
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* _hcan) {
  CANopenMessage msg;

  if (HAL_CAN_GetRxMessage(_hcan, CAN_RX_FIFO0, &msg.header, msg.data) != HAL_OK) {
    return;
  }

  // Put message in queue
  osMessageQueuePut(rxQueue, &msg, 0U, 0U);
}

void CANopen_Queue_Process() {
  CANopenMessage msg;
  osStatus_t status;

  // Get a message off the queue if available
  status = osMessageQueueGet(rxQueue, &msg, NULL, 0U);

  if (status !=  osOK) {
    return;
  }

  HAL_UART_Transmit(huart, (uint8_t*)"TX\n", sizeof("TX\n"), 100);

  // Transmit this message over USB
  CDC_Transmit_FS((uint8_t *)&msg, sizeof(msg));
}
