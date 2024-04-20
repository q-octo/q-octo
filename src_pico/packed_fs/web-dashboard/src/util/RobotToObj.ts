import { Robot } from "../generated/fbs/robot"
import { Motor } from "../generated/fbs/motor"
import { ControlSource } from "../generated/fbs/control-source"
import { Status } from "../generated/fbs/status"

/** Convert Flatbuffer to Object */
export const RobotToObj = (robot : Robot) => {
    return {
        motors: {
            motor1: {
                temperature: robot.motors().motor1().temperature(),
                rps: robot.motors().motor1().rps(),
<<<<<<< HEAD
                angle: robot.motors().motor1().angle()
                torque: robot.motors().motor1().
=======
                angle: robot.motors().motor1().angle(),
                torque: robot.motors().motor1().torque()
>>>>>>> 7a7fbf954edd0d82b69d16f5664d1ed1857751ea
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
        rssi: robot.rssi(),
        signalStrength: robot.signalStrength(),
        max_speed: robot.maxSpeed(),
        low_voltage_threshold: robot.lowVoltageThreshold(),
        critical_voltage_threshold: robot.criticalVoltageThreshold(),
        reference_wheel_angle: robot.referenceWheelAngle(),
        motor_error_code: robot.motorErrorCode(),
        wheels_folded: robot.wheelsFolded(),
        enable_rover: robot.enableRover(),
    }
}