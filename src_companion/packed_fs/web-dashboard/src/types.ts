/**
 * Types 
 */

type Motor = {
    temperature: number,
    rps: number,
    angle: number,
    torque: number,
    torque_limit: number,
    speed_limit: number,
    current_limit: number,
    Kp: number,
    Ki: number,
}

export type RoverState = {
    motors: {
        motor1: Motor,
        motor2: Motor,
    },
    batteries: number
    control_source: string,
    status: string
    voltage: number,
    current: number,
    
    low_voltage_threshold: number,
    critical_voltage_threshold: number,
    
    rssi: number,
    rssi_threshold: number,
    

    link_quality: number,
    link_quality_threshold: number,
    
    motor_error_code: string,
    fuel: number,

    radio_timeout: number,

    web_server_on_launch: boolean,
}