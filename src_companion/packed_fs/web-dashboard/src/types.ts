/**
 * Types 
 */

export type RoverState = {
    motors: {
        motor1: {
            temperature: number,
            rps: number,
            angle: number,
            torque: number
        },
        motor2: {
            temperature: number,
            rps: number,
            angle: number,
            torque: number
        }
    },
    batteries: number
    control_source: string,
    status: string
    voltage: number,
    current: number,
    rssi: number,
    linkQualityThreshold: number,
    low_voltage_threshold: number,
    critical_voltage_threshold: number,
    motor_error_code: string,
    fuel: number,
    torque_limit_left: number,
    torque_limit_right: number
    max_speed_left: number
    max_speed_right: number
}