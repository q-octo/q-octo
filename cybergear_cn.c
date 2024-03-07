/**
  ****************************(C)SWJTU_ROBOTCON****************************
  * @file       cybergear.c/h
  * @brief      小米电机函数库
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
#include "main.h"
#include "can.h"
#include "cybergear.h"
#include <string.h>

CAN_RxHeaderTypeDef rxMsg;//发送接收结构体
CAN_TxHeaderTypeDef txMsg;//发送配置结构体
uint8_t rx_data[8];       //接收数据
uint32_t Motor_Can_ID;    //接收数据电机ID
uint8_t byte[4];          //转换临时数据
uint32_t send_mail_box = {0};//NONE

#define can_txd() HAL_CAN_AddTxMessage(&hcan, &txMsg, tx_data, &send_mail_box)//CAN发送宏定义

MI_Motor mi_motor[2];//预先定义2个小米电机

/**
  * @brief          浮点数转4字节函数
  * @param[in]      f:浮点数
  * @retval         4字节数组
  * @description  : IEEE 754 协议
  */
static uint8_t* Float_to_Byte(float f)
{
    unsigned long longdata = 0;
    longdata = *(unsigned long*)&f;
    byte[0] = (longdata & 0xFF000000) >> 24;
    byte[1] = (longdata & 0x00FF0000) >> 16;
    byte[2] = (longdata & 0x0000FF00) >> 8;
    byte[3] = (longdata & 0x000000FF);
    return byte;
}

/**
  * @brief          小米电机回文16位数据转浮点
  * @param[in]      x:16位回文
  * @param[in]      x_min:对应参数下限
  * @param[in]      x_max:对应参数上限
  * @param[in]      bits:参数位数
  * @retval         返回浮点值
  */
static float uint16_to_float(uint16_t x,float x_min,float x_max,int bits)
{
    uint32_t span = (1 << bits) - 1;
    float offset = x_max - x_min;
    return offset * x / span + x_min;
}

/**
  * @brief          小米电机发送浮点转16位数据
  * @param[in]      x:浮点
  * @param[in]      x_min:对应参数下限
  * @param[in]      x_max:对应参数上限
  * @param[in]      bits:参数位数
  * @retval         返回浮点值
  */
static int float_to_uint(float x, float x_min, float x_max, int bits)
{
    float span = x_max - x_min;
    float offset = x_min;
    if (x > x_max) x = x_max;
    else if (x < x_min) x = x_min;
    return (int) ((x - offset) * ((float) ((1 << bits) - 1)) / span);
}

/**
  * @brief          写入电机参数
  * @param[in]      Motor:对应控制电机结构体
  * @param[in]      Index:写入参数对应地址
  * @param[in]      Value:写入参数值
  * @param[in]      Value_type:写入参数数据类型
  * @retval         none
  */
static void Set_Motor_Parameter(MI_Motor *Motor,uint16_t Index,float Value,char Value_type){
    uint8_t tx_data[8];
    txMsg.ExtId = Communication_Type_SetSingleParameter<<24|Master_CAN_ID<<8|Motor->CAN_ID;
    tx_data[0]=Index;
    tx_data[1]=Index>>8;
    tx_data[2]=0x00;
    tx_data[3]=0x00;
    if(Value_type == 'f'){
        Float_to_Byte(Value);
        tx_data[4]=byte[3];
        tx_data[5]=byte[2];
        tx_data[6]=byte[1];
        tx_data[7]=byte[0];
    }
    else if(Value_type == 's'){
        tx_data[4]=(uint8_t)Value;
        tx_data[5]=0x00;
        tx_data[6]=0x00;
        tx_data[7]=0x00;
    }
    can_txd();
}

/**
  * @brief          提取电机回复帧扩展ID中的电机CANID
  * @param[in]      CAN_ID_Frame:电机回复帧中的扩展CANID
  * @retval         电机CANID
  */
static uint32_t Get_Motor_ID(uint32_t CAN_ID_Frame)
{
    return (CAN_ID_Frame&0xFFFF)>>8;
}

/**
  * @brief          电机回复帧数据处理函数
  * @param[in]      Motor:对应控制电机结构体
  * @param[in]      DataFrame:数据帧
  * @param[in]      IDFrame:扩展ID帧
  * @retval         None
  */
static void Motor_Data_Handler(MI_Motor *Motor,uint8_t DataFrame[8],uint32_t IDFrame)
{
    Motor->Angle = uint16_to_float(DataFrame[0] << 8 | DataFrame[1], MIN_P, MAX_P, 16);
    Motor->Speed = uint16_to_float(DataFrame[2] << 8 | DataFrame[3], V_MIN, V_MAX, 16);
    Motor->Torque = uint16_to_float(DataFrame[4] << 8 | DataFrame[5], T_MIN, T_MAX, 16);
    Motor->Temp = (DataFrame[6] << 8 | DataFrame[7]) * Temp_Gain;
    Motor->error_code = (IDFrame & 0x1F0000) >> 16;
}

/**
  * @brief          小米电机ID检查
  * @param[in]      id:  控制电机CAN_ID【出厂默认0x7F】
  * @retval         none
  */
void  check_cybergear(uint8_t ID)
{
    uint8_t tx_data[8] = {0};
    txMsg.ExtId = Communication_Type_GetID<<24|Master_CAN_ID<<8|ID;
    can_txd();
}

/**
  * @brief          使能小米电机
  * @param[in]      Motor:对应控制电机结构体
  * @retval         none
  */
void start_cybergear(MI_Motor *Motor)
{
    uint8_t tx_data[8] = {0};
    txMsg.ExtId = Communication_Type_MotorEnable<<24|Master_CAN_ID<<8|Motor->CAN_ID;
    can_txd();
}

/**
  * @brief          停止电机
  * @param[in]      Motor:对应控制电机结构体
  * @param[in]      clear_error:清除错误位（0 不清除 1清除）
  * @retval         None
  */
void stop_cybergear(MI_Motor *Motor, uint8_t clear_error)
{
    uint8_t tx_data[8] = {0};
    tx_data[0] = clear_error;//清除错误位设置
    txMsg.ExtId = Communication_Type_MotorStop << 24 | Master_CAN_ID << 8 | Motor->CAN_ID;
    can_txd();
}

/**
  * @brief          设置电机模式(必须停止时调整！)
  * @param[in]      Motor:  电机结构体
  * @param[in]      Mode:   电机工作模式（1.运动模式Motion_mode 2. 位置模式Position_mode 3. 速度模式Speed_mode 4. 电流模式Current_mode）
  * @retval         none
  */
void set_mode_cybergear(MI_Motor *Motor, uint8_t Mode)
{
    Set_Motor_Parameter(Motor,Run_mode,Mode,'s');
}

/**
  * @brief          电流控制模式下设置电流
  * @param[in]      Motor:  电机结构体
  * @param[in]      Current:电流设置
  * @retval         none
  */
void set_current_cybergear(MI_Motor *Motor, float Current)
{
    Set_Motor_Parameter(Motor, Iq_Ref, Current, 'f');
}

/**
  * @brief          设置电机零点
  * @param[in]      Motor:  电机结构体
  * @retval         none
  */
void set_zeropos_cybergear(MI_Motor *Motor)
{
    uint8_t tx_data[8] = {0};
    tx_data[0] = 1;
    txMsg.ExtId = Communication_Type_SetPosZero << 24 | Master_CAN_ID << 8 | Motor->CAN_ID;
    can_txd();
}

/**
  * @brief          设置电机CANID
  * @param[in]      Motor:  电机结构体
  * @param[in]      Motor:  设置新ID
  * @retval         none
  */
void set_CANID_cybergear(MI_Motor *Motor, uint8_t CAN_ID)
{
    uint8_t tx_data[8] = {0};
    txMsg.ExtId = Communication_Type_CanID << 24 | CAN_ID << 16 | Master_CAN_ID << 8 | Motor->CAN_ID;
    Motor->CAN_ID = CAN_ID;//将新的ID导入电机结构体
    can_txd();
}
/**
  * @brief          小米电机初始化
  * @param[in]      Motor:  电机结构体
  * @param[in]      Can_Id: 小米电机ID(默认0x7F)
  * @param[in]      Motor_Num: 电机编号
  * @param[in]      mode: 电机工作模式（0.运动模式Motion_mode 1. 位置模式Position_mode 2. 速度模式Speed_mode 3. 电流模式Current_mode）
  * @retval         none
  */
void init_cybergear(MI_Motor *Motor, uint8_t Can_Id, uint8_t mode)
{
    txMsg.StdId = 0;            //配置CAN发送：标准帧清零
    txMsg.ExtId = 0;            //配置CAN发送：扩展帧清零
    txMsg.IDE = CAN_ID_EXT;     //配置CAN发送：扩展帧
    txMsg.RTR = CAN_RTR_DATA;   //配置CAN发送：数据帧
    txMsg.DLC = 0x08;           //配置CAN发送：数据长度

    Motor->CAN_ID = Can_Id;       //ID设置
    set_mode_cybergear(Motor, mode);//设置电机模式
    start_cybergear(Motor);        //使能电机
}

/**
  * @brief          Mi Motor motion control
  * @param[in]      Motor:  Target motor struct
  * @param[in]      torque: desired torque [-12,12] N*M
  * @param[in]      MechPosition: desired position[-12.5,12.5] rad
  * @param[in]      speed: desired speed[-30,30] rpm
  * @param[in]      kp: kp, proportional
  * @param[in]      kd: kd, derivation
  * @retval         none
  */
void motor_controlmode(MI_Motor *Motor, float torque, float MechPosition, float speed, float kp, float kd)
{
    uint8_t tx_data[8];//发送数据初始化
    //装填发送数据
    tx_data[0]=float_to_uint(MechPosition, P_MIN, P_MAX, 16) >> 8;
    tx_data[1]=float_to_uint(MechPosition, P_MIN, P_MAX, 16);
    tx_data[2]=float_to_uint(speed, V_MIN, V_MAX, 16) >> 8;
    tx_data[3]=float_to_uint(speed, V_MIN, V_MAX, 16);
    tx_data[4]=float_to_uint(kp, KP_MIN, KP_MAX, 16) >> 8;
    tx_data[5]=float_to_uint(kp, KP_MIN, KP_MAX, 16);
    tx_data[6]=float_to_uint(kd, KD_MIN, KD_MAX, 16) >> 8;
    tx_data[7]=float_to_uint(kd, KD_MIN, KD_MAX, 16);

    txMsg.ExtId = Communication_Type_MotionControl << 24 | float_to_uint(torque, T_MIN, T_MAX, 16) <<8 | Motor->CAN_ID;//装填扩展帧数据
    can_txd();
}

void motor_position_control(MI_Motor * Motor, float desiredPosition) {
    uint8_t tx_data[8];
    tx_data[0] = 0x16;
    tx_data[1] = 0x70;
    //tx_data[4] = float_to_uint(desiredPosition, -3.14f, 3.14f, 32) >> 24;
    //tx_data[5] = float_to_uint(desiredPosition, -3.14f, 3.14f, 32) >> 16;
    //tx_data[6] = float_to_uint(desiredPosition, -3.14f, 3.14f, 32) >> 8;
    //tx_data[7] = float_to_uint(desiredPosition, -3.14f, 3.14f, 32);
    memcpy(&tx_data[4], &desiredPosition, 4);
    txMsg.ExtId = Communication_Type_SetSingleParameter << 24 | Master_CAN_ID << 8 | Motor->CAN_ID;
    can_txd();
}

extern void motor_get_angle(MI_Motor * Motor) {
    uint8_t tx_data[8];
    tx_data[0] = 0x15;
    tx_data[1] = 0x30;
    txMsg.ExtId = Communication_Type_GetSingleParameter << 24 | Master_CAN_ID << 8 | Motor->CAN_ID;
    can_txd();
}

/*****************************回调函数 负责接回传信息 可转移至别处*****************************/
/**
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);              //LED闪烁指示
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxMsg, rx_data);//接收数据
    Motor_Can_ID=Get_Motor_ID(rxMsg.ExtId);//首先获取回传电机ID信息
    switch(Motor_Can_ID)                   //将对应ID电机信息提取至对应结构体
    {
        case 0X01:  // Motor 0
            if(rxMsg.ExtId >> 24 != 0) {              //检查是否为广播模式
                Motor_Data_Handler(&mi_motor[0], rx_data, rxMsg.ExtId);
                mi_motor[0].Angle = mi_motor[0].Angle - MOTOR0_ANGLE_OFFSET;
            } else {
                mi_motor[0].MCU_ID = rx_data[0];
            }
            break;
        case 0X02:  // Motor 1
            if(rxMsg.ExtId >> 24 != 0) {               //检查是否为广播模式
                Motor_Data_Handler(&mi_motor[1], rx_data, rxMsg.ExtId);
                mi_motor[1].Angle = mi_motor[1].Angle - MOTOR1_ANGLE_OFFSET;
            } else {
                mi_motor[1].MCU_ID = rx_data[1];
            }
            break;
        default:
            break;
    }
}

