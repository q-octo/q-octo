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
    batteries: {
        number: number,
        status: string
    },
    controlSource: string,
    fuel: number,
    voltage: number,
    current: number,
    rssi: number,
    linkQualityThreshold: number,
    offset: {
        left: number,
        right: number
    }
}