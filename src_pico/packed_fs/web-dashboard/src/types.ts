/**
 * Types 
 */

export type RoverState = {
    motors: {
        motor1: {
            temperature: number,
            rpm: number,
            angle: number
        },
        motor2: {
            temperature: number,
            rpm: number,
            angle: number
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
    signalStrength: number,
    offset: {
        left: number,
        right: number
    }
}