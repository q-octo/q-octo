import { Robot } from "../generated/fbs/robot"
import { Motor } from "../generated/fbs/motor"
import { ControlSource } from "../generated/fbs/control-source"
import { Status } from "../generated/fbs/status"
import { RoverState } from "../types"

/** Convert Flatbuffer to Object */
export const RobotToObj = (robot : Robot) => {
    return {
        motors: {
            motor1: {
                temperature: robot.motors().motor1().temperature(),
                rps: robot.motors().motor1().rps(),
                angle: robot.motors().motor1().angle(),
                torque: robot.motors().motor1().torque()
            },
            motor2: {
                temperature: robot.motors().motor2().temperature(),
                rps: robot.motors().motor2().rps(),
                angle: robot.motors().motor2().angle(),
                torque: robot.motors().motor2().torque(),
            }
        },
        batteries: robot.batteries(),
        control_source: ControlSource[robot.controlSource()],
        status: Status[robot.status()],
        voltage: robot.voltage(),
        current: robot.current(),
        linkQualityThreshold: robot.linkQualityThreshold(),
        low_voltage_threshold: robot.lowVoltageThreshold(),
        critical_voltage_threshold: robot.criticalVoltageThreshold(),
        motor_error_code: robot.motorErrorCode(),
        enable_rover: robot.enableRover(),
        fuel: robot.fuel(),
        torque_limit_left: robot.leftMotorLimits().maxTorque(),
        torque_limit_right: robot.rightMotorLimits().maxTorque(),
        max_speed_left: robot.leftMotorLimits().maxSpeed(),
        max_speed_right: robot.rightMotorLimits().maxSpeed(),
        rssi: robot.crsfData().telemetry().rssi()
    } as RoverState;
}