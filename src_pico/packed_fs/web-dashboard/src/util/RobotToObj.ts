import { ControlSource, Robot, Status } from "../../generated/qocto/wsschema";

/** Convert Flatbuffer to Object */
export const RobotToObj = (robot : Robot) => {
    return {
        motors: {
            motor1: {
                temperature: robot.motors().motor1().temperature(),
                rps: robot.motors().motor1().rps(),
                angle: robot.motors().motor1().angle()
                torque: robot.motors().motor1().
            },
            motor2: {
                temperature: robot.motors().motor2().temperature(),
                rps: robot.motors().motor2().rps(),
                angle: robot.motors().motor2().angle()
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