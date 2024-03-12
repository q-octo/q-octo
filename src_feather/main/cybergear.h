/**
  ****************************(C)SWJTU_ROBOTCON****************************
  * @file       cybergear.c/h
  * @brief      Xiaomi Motor Function Library
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

// Control parameters (do not change unless you have to)
#define P_MIN (-12.5f)
#define P_MAX 12.5f
#define V_MIN (-30.0f)
#define V_MAX 30.0f
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f
#define T_MIN (-12.0f)
#define T_MAX 12.0f
// #define MAX_P 720
// #define MIN_P -720
#define MAX_P 12.5663706144f
#define MIN_P (-12.5663706144f)

#define Master_CAN_ID 0x00 // Master CAN ID
// Control commands
#define Communication_Type_GetID 0x00         // Get motor ID
#define Communication_Type_MotionControl 0x01 // Send motion control command
#define Communication_Type_MotorRequest 0x02  // Request motor status
#define Communication_Type_MotorEnable 0x03   // Enable the motor
#define Communication_Type_MotorStop 0x04     // Disable the motor
#define Communication_Type_SetPosZero 0x06    // Set the mechanical zero position
#define Communication_Type_CanID 0x07         // Change the CAN ID of the motor
#define Communication_Type_Control_Mode 0x12
#define Communication_Type_GetSingleParameter 0x11 // Get Single Parameter
#define Communication_Type_SetSingleParameter 0x12 // SetSingleParameter
#define Communication_Type_ErrorFeedback 0x15      // Error Feedback
// Parameter Read Macro Definition
#define Run_mode 0x7005
#define Iq_Ref 0x7006
#define Spd_Ref 0x700A
#define Limit_Torque 0x700B
#define Cur_Kp 0x7010
#define Cur_Ki 0x7011
#define Cur_Filt_Gain 0x7014
#define Loc_Ref 0x7016
#define Limit_Spd 0x7017
#define Limit_Cur 0x7018

#define Gain_Angle 720 / 32767.0
#define Bias_Angle 0x8000
#define Gain_Speed 30 / 32767.0
#define Bias_Speed 0x8000
#define Gain_Torque 12 / 32767.0
#define Bias_Torque 0x8000
#define Temp_Gain 0.1

#define Motor_Error 0x00
#define Motor_OK 0X01

#define MOTOR0_ANGLE_OFFSET 0.558952f
#define MOTOR1_ANGLE_OFFSET (-0.716955f)

enum CONTROL_MODE // Control modes
{
    Motion_mode = 0, // Motion control
    Position_mode,   // Position control
    Speed_mode,      // Speed control
    Current_mode     // Torque control
};

enum ERROR_TAG // Error Tags
{
    OK = 0,                // No error
    BAT_LOW_ERR = 1,       // Low battery voltage
    OVER_CURRENT_ERR = 2,  // Overcurrent
    OVER_TEMP_ERR = 3,     // Overheating
    MAGNETIC_ERR = 4,      // Magnetic sensor fail
    HALL_ERR_ERR = 5,      // Hall encoder fail
    NO_CALIBRATION_ERR = 6 // Not calibrated
};

typedef struct
{                   // Mi motor callback struct
    uint8_t CAN_ID; // CAN ID
    uint8_t MCU_ID; // MCU ID[last 8 bits only，64 bits in total]
    float Angle;    // Motor angle
    float Speed;    // Motor speed
    float Torque;   // Motor torque
    float Temp;     // Motor temperature

    uint16_t set_current;
    uint16_t set_speed;
    uint16_t set_position;

    uint8_t error_code;

    float Angle_Bias;

} MI_Motor;
extern MI_Motor mi_motor[2]; // Define two motors here

extern void init_can();
extern void check_cybergear(uint8_t ID);
extern void start_cybergear(MI_Motor *Motor);
extern void stop_cybergear(MI_Motor *Motor, uint8_t clear_error);
extern void set_mode_cybergear(MI_Motor *Motor, uint8_t Mode);
extern void set_current_cybergear(MI_Motor *Motor, float Current);
extern void set_zeropos_cybergear(MI_Motor *Motor);
extern void set_CANID_cybergear(MI_Motor *Motor, uint8_t CAN_ID);
extern void init_cybergear(MI_Motor *Motor, uint8_t Can_Id, uint8_t mode);
extern void motor_controlmode(MI_Motor *Motor, float torque, float MechPosition, float speed, float kp, float kd);
extern void motor_position_control(MI_Motor *Motor, float desiredPosition);
extern void motor_get_angle(MI_Motor *Motor);