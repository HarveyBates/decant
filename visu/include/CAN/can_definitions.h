#ifndef CAN_VISUALISER_CAN_DEFINITIONS_H
#define CAN_VISUALISER_CAN_DEFINITIONS_H

#include <cstdint>

/// Function codes for CANopen frames
enum CANopenFC : uint16_t {
  FC_NMT           = 0x0000,
  FC_SYNC          = 0x0080,
  FC_EMERGENCY     = 0x0080,
  FC_TIMESTAMP     = 0x0100,
  FC_PDO1_TRANSMIT = 0x0180,
  FC_PDO1_RECEIVE  = 0x0200,
  FC_PDO2_TRANSMIT = 0x0280,
  FC_PDO2_RECEIVE  = 0x0300,
  FC_PDO3_TRANSMIT = 0x0380,
  FC_PDO3_RECEIVE  = 0x0400,
  FC_PDO4_TRANSMIT = 0x0480,
  FC_PDO4_RECEIVE  = 0x0500,
  FC_SDO_TRANSMIT  = 0x0580,
  FC_SDO_RECEIVE   = 0x0600
};

/// Network management (NMT) codes
enum CANopenNMT {
  NMT_BOOT                = 0x00,
  NMT_OPERATIONAL         = 0x01,
  NMT_STOPPED             = 0x02,
  NMT_PRE_OPERATIONAL     = 0x80,
  NMT_RESET_APPLICATION   = 0x81,
  NMT_RESET_COMMUNICATION = 0x82
};

/// SDO Client Command Specifier (CSS) codes
enum CANopenCSS {
  CSS_SEGMENT_DOWNLOAD = 0x00,
  CSS_DOWNLOAD         = 0x01,
  CSS_SEGMENT_UPLOAD   = 0x02,
  CSS_UPLOAD           = 0x03
};

/// SDO abort codes
enum CANopenSDOAbort : uint32_t {
  ABORT_TOGGLE_NOT_ALTERNATED             = ((uint32_t)0x0503 << 16) | 0x0000,
  ABORT_SDO_PROTOCOL_TIMEOUT              = ((uint32_t)0x0504 << 16) | 0x0000,
  ABORT_COMMAND_SPECIFIER_NOT_VALID       = ((uint32_t)0x0504 << 16) | 0x0001,
  ABORT_OUT_OF_MEMORY                     = ((uint32_t)0x0504 << 16) | 0x0005,
  ABORT_UNSUPPORTED_ACCESS                = ((uint32_t)0x0601 << 16) | 0x0000,
  ABORT_WRITE_ONLY                        = ((uint32_t)0x0601 << 16) | 0x0001,
  ABORT_READ_ONLY                         = ((uint32_t)0x0601 << 16) | 0x0002,
  ABORT_OBJECT_DOES_NOT_EXIST             = ((uint32_t)0x0602 << 16) | 0x0000,
  ABORT_OBJECT_CANNOT_BE_MAPPED           = ((uint32_t)0x0604 << 16) | 0x0041,
  ABORT_MAPPED_OBJECTS_EXCEED_PDO         = ((uint32_t)0x0604 << 16) | 0x0042,
  ABORT_PARAMETER_INCOMPATIBILITY_REASON  = ((uint32_t)0x0604 << 16) | 0x0043,
  ABORT_INTERNAL_DEVICE_ERROR             = ((uint32_t)0x0606 << 16) | 0x0000,
  ABORT_HARDWARE_ERROR                    = ((uint32_t)0x0606 << 16) | 0x0001,
  ABORT_PARAMETER_LENGTH_ERROR            = ((uint32_t)0x0607 << 16) | 0x0010,
  ABORT_PARAMETER_LENGTH_TOO_LONG         = ((uint32_t)0x0607 << 16) | 0x0012,
  ABORT_PARAMETER_LENGTH_TOO_SHORT        = ((uint32_t)0x0607 << 16) | 0x0013,
  ABORT_SUB_INDEX_DOES_NOT_EXIST          = ((uint32_t)0x0609 << 16) | 0x0011,
  ABORT_INVALID_VALUE                     = ((uint32_t)0x0609 << 16) | 0x0030,
  ABORT_VALUE_TOO_HIGH                    = ((uint32_t)0x0609 << 16) | 0x0031,
  ABORT_VALUE_TOO_LOW                     = ((uint32_t)0x0609 << 16) | 0x0032,
  ABORT_MAX_LESS_THAN_MIN                 = ((uint32_t)0x0609 << 16) | 0x0036,
  ABORT_RESOURCE_NOT_AVAILABLE            = ((uint32_t)0x060A << 16) | 0x0023,
  ABORT_GENERAL_ERROR                     = ((uint32_t)0x0800 << 16) | 0x0000,
  ABORT_DATA_CANNOT_BE_TRANSFERRED_STORED = ((uint32_t)0x0800 << 16) | 0x0020,
  ABORT_DATA_CANNOT_BE_TRANSFERRED_LOCAL  = ((uint32_t)0x0800 << 16) | 0x0021,
  ABORT_INVALID_DEVICE_STATE              = ((uint32_t)0x0800 << 16) | 0x0022,
};

typedef struct {
  uint32_t StdId; /*!< Specifies the standard identifier.
                    This parameter must be a number between Min_Data = 0 and
                    Max_Data = 0x7FF. */

  uint32_t ExtId; /*!< Specifies the extended identifier.
                    This parameter must be a number between Min_Data = 0 and
                    Max_Data = 0x1FFFFFFF. */

  uint32_t IDE; /*!< Specifies the type of identifier for the message that will
                be transmitted. This parameter can be a value of @ref
                CAN_identifier_type */

  uint32_t RTR; /*!< Specifies the type of frame for the message that will be
                transmitted. This parameter can be a value of @ref
                CAN_remote_transmission_request */

  uint32_t DLC; /*!< Specifies the length of the frame that will be transmitted.
                This parameter must be a number between Min_Data = 0 and
                Max_Data = 8. */

  uint32_t Timestamp; /*!< Specifies the timestamp counter value captured on
                         start of frame reception.
                          @note: Time Triggered Communication Mode must be
                         enabled. This parameter must be a number between
                         Min_Data = 0 and Max_Data = 0xFFFF. */

  uint32_t FilterMatchIndex; /*!< Specifies the index of matching acceptance
                          filter element. This parameter must be a number
                          between Min_Data = 0 and Max_Data = 0xFF. */

} CAN_RxHeaderTypeDef;

#endif  // CAN_VISUALISER_CAN_DEFINITIONS_H
