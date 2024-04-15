#include "xiaomi_cybergear_driver.h"
#include "can.h"

/* PUBLIC */
XiaomiCyberGearDriver::XiaomiCyberGearDriver() {};
XiaomiCyberGearDriver::XiaomiCyberGearDriver(uint8_t cybergear_can_id, uint8_t master_can_id) 
    : _cybergear_can_id(cybergear_can_id),
    _master_can_id(master_can_id),
    _run_mode(MODE_MOTION),
    _use_serial_debug(false)
{}

XiaomiCyberGearDriver::~XiaomiCyberGearDriver(){}


void XiaomiCyberGearDriver::init_motor(uint8_t mode){
    stop_motor();
    set_run_mode(mode);
}
void XiaomiCyberGearDriver::enable_motor(){
    uint8_t data[8] = {0x00};
    _send_can_package(_cybergear_can_id, CMD_ENABLE, _master_can_id, 8, data);
}
void XiaomiCyberGearDriver::stop_motor(){
    uint8_t data[8] = {0x00};
    _send_can_package(_cybergear_can_id, CMD_STOP, _master_can_id, 8, data);
}
void XiaomiCyberGearDriver::set_run_mode(uint8_t mode){
    _run_mode = mode;
    uint8_t data[8] = {0x00};
    data[0] = ADDR_RUN_MODE & 0x00FF;
    data[1] = ADDR_RUN_MODE >> 8;
    data[4] = mode;
    _send_can_package(_cybergear_can_id, CMD_RAM_WRITE, _master_can_id, 8, data);
}

void XiaomiCyberGearDriver::set_limit_speed(float speed){
    _send_can_float_package(_cybergear_can_id, ADDR_LIMIT_SPEED, speed, 0.0f, V_MAX);
}
void XiaomiCyberGearDriver::set_limit_current(float current){
    _send_can_float_package(_cybergear_can_id, ADDR_LIMIT_CURRENT, current, 0.0f, I_MAX);
}
void XiaomiCyberGearDriver::set_limit_torque(float torque){
    _send_can_float_package(_cybergear_can_id, ADDR_LIMIT_TORQUE, torque, 0.0f, T_MAX);
}

// MODE_MOTION
void XiaomiCyberGearDriver::send_motion_control(XiaomiCyberGearMotionCommand cmd){
    uint8_t data[8] = {0x00};

    uint16_t position = _float_to_uint(cmd.position, POS_MIN, POS_MAX, 16);
    data[0] = position >> 8;
    data[1] = position & 0x00FF;

    uint16_t speed = _float_to_uint(cmd.speed, V_MIN, V_MAX, 16);
    data[2] = speed >> 8;
    data[3] = speed & 0x00FF;

    uint16_t kp = _float_to_uint(cmd.kp, KP_MIN, KP_MAX, 16);
    data[4] = kp >> 8;
    data[5] = kp & 0x00FF;

    uint16_t kd = _float_to_uint(cmd.kd, KD_MIN, KD_MAX, 16);
    data[6] = kd >> 8;
    data[7] = kd & 0x00FF;

    uint16_t torque = _float_to_uint(cmd.torque, T_MIN, T_MAX, 16);

    _send_can_package(_cybergear_can_id, CMD_POSITION, torque, 8, data);
}

// MODE_CURRENT
void XiaomiCyberGearDriver::set_current_kp(float kp){
    _send_can_float_package(_cybergear_can_id, ADDR_CURRENT_KP, kp, KP_MIN, KP_MAX);
}
void XiaomiCyberGearDriver::set_current_ki(float ki){
    _send_can_float_package(_cybergear_can_id, ADDR_CURRENT_KI, ki, KI_MIN, KI_MAX);
}
void XiaomiCyberGearDriver::set_current_filter_gain(float gain){
    _send_can_float_package(_cybergear_can_id, ADDR_CURRENT_FILTER_GAIN, gain, CURRENT_FILTER_GAIN_MIN, CURRENT_FILTER_GAIN_MAX);
}
void XiaomiCyberGearDriver::set_current_ref(float current){
    _send_can_float_package(_cybergear_can_id, ADDR_I_REF, current, I_MIN, I_MAX);
}

// MODE_POSITION
void XiaomiCyberGearDriver::set_position_kp(float kp){
    _send_can_float_package(_cybergear_can_id, ADDR_POSITION_KP, kp, KP_MIN, KP_MAX);
}
void XiaomiCyberGearDriver::set_position_ref(float position){
    _send_can_float_package(_cybergear_can_id, ADDR_POSITION_REF, position, POS_MIN, POS_MAX);
}

// MODE_SPEED
void XiaomiCyberGearDriver::set_speed_kp(float kp){
    _send_can_float_package(_cybergear_can_id, ADDR_SPEED_KP, kp, KP_MIN, KP_MAX);
}
void XiaomiCyberGearDriver::set_speed_ki(float ki){
    _send_can_float_package(_cybergear_can_id, ADDR_SPEED_KI, ki, KI_MIN, KI_MAX);
}
void XiaomiCyberGearDriver::set_speed_ref(float speed){
    _send_can_float_package(_cybergear_can_id, ADDR_SPEED_REF, speed, V_MIN, V_MAX);
}

void XiaomiCyberGearDriver::set_motor_can_id(uint8_t can_id){
    uint8_t data[8] = {0x00};
    uint16_t option = can_id << 8 | _master_can_id;
    _send_can_package(_cybergear_can_id, CMD_SET_CAN_ID, option, 8, data);
    _cybergear_can_id = can_id;
}

uint8_t XiaomiCyberGearDriver::get_run_mode() const {
    return _run_mode;
}
uint8_t XiaomiCyberGearDriver::get_motor_can_id() const {
    return _cybergear_can_id;
}

void XiaomiCyberGearDriver::request_status()
{
    uint8_t data[8] = {0x00};
    // TODO This whole function may not even be necessary (i think so!)
    _send_can_package(_cybergear_can_id, CMD_GET_MOTOR_FAIL, _master_can_id, 8, data);
}

void XiaomiCyberGearDriver::process_message(uint32_t id, uint8_t *data)
{
    const uint8_t type = id >> 24;
    switch (type)
    {
    case CMD_RESPONSE:
    {
        uint16_t raw_position = data[1] | data[0] << 8;
        uint16_t raw_speed = data[3] | data[2] << 8;
        uint16_t raw_torque = data[5] | data[4] << 8;
        uint16_t raw_temperature = data[7] | data[6] << 8;

        _status.position = _uint_to_float(raw_position, POS_MIN, POS_MAX);
        _status.speed = _uint_to_float(raw_speed, V_MIN, V_MAX);
        _status.torque = _uint_to_float(raw_torque, T_MIN, T_MAX);
        _status.temperature = raw_temperature;
        break;
    }
    case CMD_DEVICE_ID:
        break;
    case CMD_RAM_READ:
        break;
    case CMD_GET_MOTOR_FAIL:
        // TODO handle this case
        break;
    default:
        break;
    }
}
XiaomiCyberGearStatus XiaomiCyberGearDriver::get_status() const {
    return _status;
}

/* PRIVATE */
uint16_t XiaomiCyberGearDriver::_float_to_uint(float x, float x_min, float x_max, int bits){
    if (bits>16) bits=16;
    float span = x_max - x_min;
    float offset = x_min;
    if(x > x_max) x = x_max;
    else if(x < x_min) x = x_min;
    return (int) ((x-offset)*((float)((1<<bits)-1))/span);
}
float XiaomiCyberGearDriver::_uint_to_float(uint16_t x, float x_min, float x_max){
    uint16_t type_max = 0xFFFF;
    float span = x_max - x_min;
    return (float) x / type_max * span + x_min;
}
void XiaomiCyberGearDriver::_send_can_package(uint8_t can_id, uint8_t cmd_id, uint16_t option, uint8_t len, uint8_t* data){
    uint32_t id = cmd_id << 24 | option << 8 | can_id;
    // [cmd_id:5] [option:16] [can_id:8]
    CanCommunication::sendCANPacket(id, data);
}


void XiaomiCyberGearDriver::_send_can_float_package(uint8_t can_id, uint16_t addr, float value, float min, float max){
    uint8_t data[8] = {0x00};
    data[0] = addr & 0x00FF;
    data[1] = addr >> 8;

    float val = (max < value) ? max : value;
    val = (min > value) ? min : value;
    memcpy(&data[4], &value, 4);
    _send_can_package(can_id, CMD_RAM_WRITE, _master_can_id, 8, data);
}