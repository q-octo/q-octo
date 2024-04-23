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
    max_speed: number,
    low_voltage_threshold: number,
    critical_voltage_threshold: number,
    reference_wheel_angle: number,
    motor_error_code: string,
    wheels_folded: boolean,
    fuel: number,
}