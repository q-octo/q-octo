#pragma once
#include "xiaomi_cybergear_defs.h"
#include <cstdint>

enum ResponseModeStatus {
    RESET = 0,
    CALIBRATION = 1,
    RUN = 2,
};

struct XiaomiCyberGearStatus {
    float position;
    float speed;
    float torque;
    uint16_t temperature;
    
    bool notCalibrated;
    bool hallEncodingFailure;
    bool magneticEncodingFailure;
    bool temperatureLimitExceeded;
    bool currentLimitExceeded;
    bool voltageLowerLimitExceeded;
    ResponseModeStatus modeStatus;
};


struct XiaomiCyberGearMotionCommand {
    float position;
    float speed;
    float torque;
    float kp;
    float kd;
};

struct XiaomiCyberGearMotorParameter
{
  // Most parameters are unused.
  // We assume -1 for limits until they are updated.
  unsigned long stamp_usec = 0;
  uint16_t run_mode;
  float iq_ref;
  float spd_ref;
  float limit_torque = -1;
  float cur_kp;
  float cur_ki;
  float cur_filt_gain;
  float loc_ref;
  float limit_spd = -1;
  float limit_cur = -1;
  float mech_pos;
  float iqf;
  float mech_vel;
  float vbus;
  int16_t rotation;
  float loc_kp;
  float spd_kp;
  float spd_ki;
};

class XiaomiCyberGearDriver {
    public:
        XiaomiCyberGearDriver();
        XiaomiCyberGearDriver(uint8_t cybergear_can_id, uint8_t master_can_id);
        virtual ~XiaomiCyberGearDriver();

        void init_motor(uint8_t mode);
        void enable_motor();
        void stop_motor();
        void set_run_mode(uint8_t mode);

        void set_limit_speed(float speed);
        void set_limit_current(float current);
        void set_limit_torque(float torque);

        // MODE MOTION
        void send_motion_control(XiaomiCyberGearMotionCommand cmd);

        // MODE_CURRENT
        void set_current_kp(float kp);
        void set_current_ki(float ki);
        void set_current_filter_gain(float gain);
        void set_current_ref(float current);

        // MODE_POSITION
        void set_position_kp(float kp);
        void set_position_ref(float position);

        // MODE_SPEED
        void set_speed_kp(float kp);
        void set_speed_ki(float ki);
        void set_speed_ref(float speed);

        uint8_t get_run_mode() const;
        uint8_t get_motor_can_id() const;
        void set_motor_can_id(uint8_t can_id);

        void request_status();
        void process_message(uint32_t id, uint8_t* data);
        XiaomiCyberGearStatus get_status() const;
        void set_mech_position_to_zero();
        void read_ram_data(uint16_t index);
        void get_vbus();
        void get_speed_limit();
        void get_current_limit();
        void get_torque_limit();

        /**
        * @brief Get the motor param object
        *
        * @return MotorParameter motor param
        */
        XiaomiCyberGearMotorParameter get_motor_param() const { return motor_param_; }
        
    private:
        uint16_t _float_to_uint(float x, float x_min, float x_max, int bits);
        float _uint_to_float(uint16_t x, float x_min, float x_max);
        void _send_can_package(uint8_t can_id, uint8_t cmd_id, uint16_t option, uint8_t len, uint8_t* data);
        void _send_can_float_package(uint8_t can_id,uint16_t addr, float value, float min, float max);
        void process_read_parameter_packet(const uint8_t *data);

        uint8_t _cybergear_can_id;
        uint8_t _master_can_id;
        uint8_t _run_mode;
        bool _use_serial_debug;
        XiaomiCyberGearStatus _status;
        XiaomiCyberGearMotorParameter motor_param_;
};