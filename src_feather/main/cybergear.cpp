/**
  ****************************(C)SWJTU_ROBOTCON****************************
  * @file       cybergear.c/h
  * @brief      Xiaomi motor library
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     1-10-2023       ZDYukino        1. done
  *
  @verbatim
  =========================================================================
  =========================================================================
  @endverbatim
  ****************************(C)SWJTU_ROBOTCON****************************
  **/
#include "CANSAME5x.h"
#include "cybergear.h"
#include <string.h>


CAN_RxHeaderTypeDef rxMsg;    // RX structure
CAN_TxHeaderTypeDef txMsg;    // TX configuration structure
uint8_t rx_data[8];           // RX data
uint32_t Motor_Can_ID;        // Motor ID for received data
uint8_t byte[4];              // Temporary data for conversion
uint32_t send_mail_box = {0}; // NONE

CANSAME5x CAN;

// #define can_txd() HAL_CAN_AddTxMessage(&hcan, &txMsg, tx_data, &send_mail_box) // CAN send macro


MI_Motor mi_motor[2]; // Predefine 2 Xiaomi motors

static void init_can() {
  // start the CAN bus at 250 kbps
  if (!CAN.begin(250000)) {
    // TODO This may require Arduino.h
    Serial.println("Starting CAN failed!");
    while (1) delay(10);
  }
}

static void sendCANPacket(long id, const u_int8_t data) {
  // Remote transmission request
  bool rtr = false; 
  CAN.beginExtendedPacket(id, len, rtr);

  CAN.write(&data, sizeof(data));
  CAN.endPacket(); 
}

/**
 * @brief          Convert float to 4-byte array
 * @param[in]      f: The float number
 * @retval         4-byte array
 * @description    IEEE 754 protocol
 */
static uint8_t *Float_to_Byte(float f)
{
  unsigned long longdata = 0;
  longdata = *(unsigned long *)&f;
  byte[0] = (longdata & 0xFF000000) >> 24;
  byte[1] = (longdata & 0x00FF0000) >> 16;
  byte[2] = (longdata & 0x0000FF00) >> 8;
  byte[3] = (longdata & 0x000000FF);
  return byte;
}

/**
 * @brief          Convert 16-bit data from Xiaomi motor to float
 * @param[in]      x: 16-bit data
 * @param[in]      x_min: Minimum parameter value
 * @param[in]      x_max: Maximum parameter value
 * @param[in]      bits: Number of bits of the parameter
 * @retval         The float value
 */
static float uint16_to_float(uint16_t x, float x_min, float x_max, int bits)
{
  uint32_t span = (1 << bits) - 1;
  float offset = x_max - x_min;
  return offset * x / span + x_min;
}

/**
 * @brief          Convert float to 16-bit data for sending to Xiaomi motor
 * @param[in]      x: Float value
 * @param[in]      x_min: Minimum parameter value
 * @param[in]      x_max: Maximum parameter value
 * @param[in]      bits: Number of bits of the parameter
 * @retval         The 16-bit data
 */
static int float_to_uint(float x, float x_min, float x_max, int bits)
{
  float span = x_max - x_min;
  float offset = x_min;
  if (x > x_max)
    x = x_max;
  else if (x < x_min)
    x = x_min;
  return (int)((x - offset) * ((float)((1 << bits) - 1)) / span);
}

/**
 * @brief          Write motor parameter
 * @param[in]      Motor: Motor control structure
 * @param[in]      Index: Address for the parameter
 * @param[in]      Value: Parameter value
 * @param[in]      Value_type: Data type of the parameter
 * @retval         None
 */
static void Set_Motor_Parameter(MI_Motor *Motor, uint16_t Index, float Value, char Value_type)
{
  uint8_t tx_data[8];
  uint32_t tx_msg_id = Communication_Type_SetSingleParameter << 24 | Master_CAN_ID << 8 | Motor->CAN_ID;
  tx_data[0] = Index;
  tx_data[1] = Index >> 8;
  tx_data[2] = 0x00;
  tx_data[3] = 0x00;
  if (Value_type == 'f')
  {
    Float_to_Byte(Value);
    tx_data[4] = byte[3];
    tx_data[5] = byte[2];
    tx_data[6] = byte[1];
    tx_data[7] = byte[0];
  }
  else if (Value_type == 's')
  {
    tx_data[4] = (uint8_t)Value;
    tx_data[5] = 0x00;
    tx_data[6] = 0x00;
    tx_data[7] = 0x00;
  }
  sendCANPacket(tx_msg_id, tx_data);
}

/**
 * @brief          Extract motor CANID from reply frame extended ID
 * @param[in]      CAN_ID_Frame: Extended CANID in the reply frame
 * @retval         Motor CANID
 */
static uint32_t Get_Motor_ID(uint32_t CAN_ID_Frame)
{
  return (CAN_ID_Frame & 0xFFFF) >> 8;
}

/**
 * @brief          Process data frame from motor reply
 * @param[in]      Motor: Motor control structure
 * @param[in]      DataFrame: Data frame
 * @param[in]      IDFrame: Extended ID frame
 * @retval         None
 */
static void Motor_Data_Handler(MI_Motor *Motor, uint8_t DataFrame[8], uint32_t IDFrame)
{
  Motor->Angle = uint16_to_float(DataFrame[0] << 8 | DataFrame[1], MIN_P, MAX_P, 16);
  Motor->Speed = uint16_to_float(DataFrame[2] << 8 | DataFrame[3], V_MIN, V_MAX, 16);
  Motor->Torque = uint16_to_float(DataFrame[4] << 8 | DataFrame[5], T_MIN, T_MAX, 16);
  Motor->Temp = (DataFrame[6] << 8 | DataFrame[7]) * Temp_Gain;
  Motor->error_code = (IDFrame & 0x1F0000) >> 16;
}

/**
 * @brief          Check Xiaomi motor ID
 * @param[in]      id: Motor CAN_ID [factory default 0x7F]
 * @retval         None
 */
void check_cybergear(uint8_t ID)
{
  uint8_t tx_data[8] = {0};
  uint32_t tx_msg_id = Communication_Type_GetID << 24 | Master_CAN_ID << 8 | ID;
  sendCANPacket(tx_msg_id, tx_data);
}

/**
 * @brief          Enable Xiaomi motor
 * @param[in]      Motor: Motor control structure
 * @retval         None
 */
void start_cybergear(MI_Motor *Motor)
{
  uint8_t tx_data[8] = {0};
  uint32_t tx_msg_id = Communication_Type_MotorEnable << 24 | Master_CAN_ID << 8 | Motor->CAN_ID;
  sendCANPacket(tx_msg_id, tx_data);
}

/**
 * @brief          Stop motor
 * @param[in]      Motor: Motor control structure
 * @param[in]      clear_error: Clear error bit (0 don't clear, 1 clear)
 * @retval         None
 */
void stop_cybergear(MI_Motor *Motor, uint8_t clear_error)
{
  uint8_t tx_data[8] = {0};
  tx_data[0] = clear_error; // Set clear error bit
  uint32_t tx_msg_id = Communication_Type_MotorStop << 24 | Master_CAN_ID << 8 | Motor->CAN_ID;
  sendCANPacket(tx_msg_id, tx_data);
}

/**
 * @brief          Set motor mode (Must be adjusted when stopped!)
 * @param[in]      Motor: Motor structure
 * @param[in]      Mode: Motor operating mode (1. Motion mode 2. Position mode 3. Speed mode 4. Current mode)
 * @retval         None
 */
void set_mode_cybergear(MI_Motor *Motor, uint8_t Mode)
{
  Set_Motor_Parameter(Motor, Run_mode, Mode, 's');
}

/**
 * @brief          Set current in current control mode
 * @param[in]      Motor: Motor structure
 * @param[in]      Current: Current setting
 * @retval         None
 */
void set_current_cybergear(MI_Motor *Motor, float Current)
{
  Set_Motor_Parameter(Motor, Iq_Ref, Current, 'f');
}

/**
 * @brief          Set motor zero point
 * @param[in]      Motor: Motor structure
 * @retval         None
 */
void set_zeropos_cybergear(MI_Motor *Motor)
{
  uint8_t tx_data[8] = {0};
  tx_data[0] = 1;
  uint32_t tx_msg_id = Communication_Type_SetPosZero << 24 | Master_CAN_ID << 8 | Motor->CAN_ID;
  sendCANPacket(tx_msg_id, tx_data);
}

/**
 * @brief          Set motor CANID
 * @param[in]      Motor: Motor structure
 * @param[in]      CAN_ID: New ID to set
 * @retval         None
 */
void set_CANID_cybergear(MI_Motor *Motor, uint8_t CAN_ID)
{
  uint8_t tx_data[8] = {0};
  uint32_t tx_msg_id = Communication_Type_CanID << 24 | CAN_ID << 16 | Master_CAN_ID << 8 | Motor->CAN_ID;
  Motor->CAN_ID = CAN_ID; // Load the new ID into the motor structure
  sendCANPacket(tx_msg_id, tx_data);
}

/**
 * @brief          Initialize Xiaomi motor
 * @param[in]      Motor: Motor structure
 * @param[in]      Can_Id: Xiaomi motor ID (default 0x7F)
 * @param[in]      Motor_Num: Motor number
 * @param[in]      mode: Motor operating mode (0. Motion mode 1. Position mode 2. Speed mode 3. Current mode)
 * @retval         None
 */
void init_cybergear(MI_Motor *Motor, uint8_t Can_Id, uint8_t mode)
{
  txMsg.StdId = 0;          // Configure CAN send: clear standard frame
  uint32_t tx_msg_id = 0;          // Configure CAN send: clear extended frame
  txMsg.IDE = CAN_ID_EXT;   // Configure CAN send: extended frame
  txMsg.RTR = CAN_RTR_DATA; // Configure CAN send: data frame
  txMsg.DLC = 0x08;         // Configure CAN send: data length

  Motor->CAN_ID = Can_Id;          // Set ID
  set_mode_cybergear(Motor, mode); // Set motor mode
  start_cybergear(Motor);          // Enable motor
}

/**
 * @brief          Mi Motor motion control
 * @param[in]      Motor: Target motor structure
 * @param[in]      torque: Desired torque [-12,12] N*m
 * @param[in]      MechPosition: Desired position [-12.5,12.5] rad
 * @param[in]      speed: Desired speed [-30,30] rpm
 * @param[in]      kp: Proportional gain
 * @param[in]      kd: Derivative gain
 * @retval         None
 */
void motor_controlmode(MI_Motor *Motor, float torque, float MechPosition, float speed, float kp, float kd)
{
  uint8_t tx_data[8]; // Initialize send data
  // Load send data
  tx_data[0] = float_to_uint(MechPosition, P_MIN, P_MAX, 16) >> 8;
  tx_data[1] = float_to_uint(MechPosition, P_MIN, P_MAX, 16);
  tx_data[2] = float_to_uint(speed, V_MIN, V_MAX, 16) >> 8;
  tx_data[3] = float_to_uint(speed, V_MIN, V_MAX, 16);
  tx_data[4] = float_to_uint(kp, KP_MIN, KP_MAX, 16) >> 8;
  tx_data[5] = float_to_uint(kp, KP_MIN, KP_MAX, 16);
  tx_data[6] = float_to_uint(kd, KD_MIN, KD_MAX, 16) >> 8;
  tx_data[7] = float_to_uint(kd, KD_MIN, KD_MAX, 16);

  uint32_t tx_msg_id = Communication_Type_MotionControl << 24 | float_to_uint(torque, T_MIN, T_MAX, 16) << 8 | Motor->CAN_ID; // Load extended frame data
  sendCANPacket(tx_msg_id, tx_data);
}

void motor_position_control(MI_Motor *Motor, float desiredPosition)
{
  uint8_t tx_data[8];
  tx_data[0] = 0x16;
  tx_data[1] = 0x70;
  // tx_data[4] = float_to_uint(desiredPosition, -3.14f, 3.14f, 32) >> 24;
  // tx_data[5] = float_to_uint(desiredPosition, -3.14f, 3.14f, 32) >> 16;
  // tx_data[6] = float_to_uint(desiredPosition, -3.14f, 3.14f, 32) >> 8;
  // tx_data[7] = float_to_uint(desiredPosition, -3.14f, 3.14f, 32);
  memcpy(&tx_data[4], &desiredPosition, 4);
  uint32_t tx_msg_id = Communication_Type_SetSingleParameter << 24 | Master_CAN_ID << 8 | Motor->CAN_ID;
  sendCANPacket(tx_msg_id, tx_data);
}

extern void motor_get_angle(MI_Motor *Motor)
{
  uint8_t tx_data[8];
  tx_data[0] = 0x15;
  tx_data[1] = 0x30;
  uint32_t tx_msg_id = Communication_Type_GetSingleParameter << 24 | Master_CAN_ID << 8 | Motor->CAN_ID;
  sendCANPacket(tx_msg_id, tx_data);
}

/***************************** Callback function responsible for receiving returned information Can be moved elsewhere *****************************/
/**
 * @brief          HAL library CAN callback function, receives motor data
 * @param[in]      hcan: CAN handle pointer
 * @retval         none
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);                // Toggle LED to indicate activity
  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxMsg, rx_data); // Receive data
  Motor_Can_ID = Get_Motor_ID(rxMsg.ExtId);                  // First, get the returned motor ID information
  switch (Motor_Can_ID)                                      // Extract corresponding motor information to its structure
  {
  case 0X01: // Motor 0
    if (rxMsg.ExtId >> 24 != 0)
    { // Check if it is broadcast mode
      Motor_Data_Handler(&mi_motor[0], rx_data, rxMsg.ExtId);
      mi_motor[0].Angle = mi_motor[0].Angle - MOTOR0_ANGLE_OFFSET;
    }
    else
    {
      mi_motor[0].MCU_ID = rx_data[0];
    }
    break;
  case 0X02: // Motor 1
    if (rxMsg.ExtId >> 24 != 0)
    { // Check if it is broadcast mode
      Motor_Data_Handler(&mi_motor[1], rx_data, rxMsg.ExtId);
      mi_motor[1].Angle = mi_motor[1].Angle - MOTOR1_ANGLE_OFFSET;
    }
    else
    {
      mi_motor[1].MCU_ID = rx_data[1];
    }
    break;
  default:
    break;
  }
}
