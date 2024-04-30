import asyncio
import MotorControl.EnableExtendedMode
import websockets
import flatbuffers

# Import your generated Flatbuffers Python modules here
# from MyFlatbuffersModule import Command, MotorCommands, EncoderCommands, etc.
from MotorControl import MotorCommands
from MotorControl.MotorCommands import *
from MotorControl import EncoderCommands
from MotorControl.EncoderCommands import *
import MotorControl

class MotorAPI:
    def __init__(self, motor_id, ws_url='ws://rover.local'):
        self.motor_id = motor_id
        self.ws_url = ws_url

    async def send_command(self, command):
        builder = flatbuffers.Builder(1024)
        command.Offset(builder)
        builder.Finish(command)
        buf = builder.Output()
        await self.send_ws(buf)

    async def send_ws(self, data):
        async with websockets.connect(self.ws_url) as websocket:
            await websocket.send(data)

    async def init_motor(self, mode):
        # Assume 'Mode' is an enum and needs to be passed correctly
        command = MotorCommands.InitMotorT()
        command.mode = mode
        await self.send_command(command)

    async def enable_motor(self):
        command = MotorCommands.EnableMotorT()
        await self.send_command(command)

    async def stop_motor(self):
        command = MotorCommands.StopMotorT()
        await self.send_command(command)

    async def set_run_mode(self, mode):
        command = MotorCommands.SetRunModeT()
        command.mode = mode
        await self.send_command(command)

    async def send_motion_control(self, position, speed, torque, kp, kd):
        command = MotorCommands.SendMotionControlT()
        command.position = position
        command.speed = speed
        command.torque = torque
        command.kp = kp
        command.kd = kd
        await self.send_command(command)

    async def set_limit_speed(self, speed):
        command = MotorCommands.SetLimitSpeedT()
        command.speed = speed
        await self.send_command(command)

    async def set_limit_current(self, current):
        command = MotorCommands.SetLimitCurrentT()
        command.current = current
        await self.send_command(command)

    async def set_limit_torque(self, torque):
        command = MotorCommands.SetLimitTorqueT()
        command.torque = torque
        await self.send_command(command)

    async def set_current_kp(self, kp):
        command = MotorCommands.SetCurrentKpT()
        command.kp = kp
        await self.send_command(command)

    async def set_current_ki(self, ki):
        command = MotorCommands.SetCurrentKiT()
        command.ki = ki
        await self.send_command(command)

    async def set_current_filter_gain(self, gain):
        command = MotorCommands.SetCurrentFilterGainT()
        command.gain = gain
        await self.send_command(command)

    async def set_current_ref(self, current_ref):
        command = MotorCommands.SetCurrentRefT()
        command.current_ref = current_ref
        await self.send_command(command)

    async def set_position_kp(self, kp):
        command = MotorCommands.SetPositionKpT()
        command.kp = kp
        await self.send_command(command)

    async def set_position_ref(self, position_ref):
        command = MotorCommands.SetPositionRefT()
        command.position_ref = position_ref
        await self.send_command(command)

    async def set_speed_kp(self, kp):
        command = MotorCommands.SetSpeedKpT()
        command.kp = kp
        await self.send_command(command)

    async def set_speed_ki(self, ki):
        command = MotorCommands.SetSpeedKiT()
        command.ki = ki
        await self.send_command(command)

    async def set_speed_ref(self, speed_ref):
        command = MotorCommands.SetSpeedRefT()
        command.speed_ref = speed_ref
        await self.send_command(command)


# from MotorControl import CommandType, , EncoderCommandT, EnableExtendedModeT
import MotorControl.Command
from MotorControl.Command import *
from MotorControl.EncoderCommand import *
from MotorControl.CommandType import *
from MotorControl.EnableExtendedMode import *


# Get the resulting byte array
byte_array = builder.Output()

class EncoderAPI:
    def __init__(self, motor_id, ws_url='ws://rover.local'):
        self.motor_id = motor_id
        self.ws_url = ws_url

    async def send_command(self, command):
        builder = flatbuffers.Builder(1024)
        command.Offset(builder)
        builder.Finish(command)
        buf = builder.Output()
        await self.send_ws(buf)

    async def send_ws(self, data):
        async with websockets.connect(self.ws_url) as websocket:
            await websocket.send(data)

    async def enable_extended_mode(self):

        # Create an EnableExtendedMode command
        enable_extended_mode = EnableExtendedModeT()

        # Create an EncoderCommand
        encoder_command = EncoderCommandT()
        encoder_command.motor_id = 1  # Replace 1 with your motor_id
        encoder_command.command_type = EncoderCommands.EnableExtendedMode
        encoder_command.command = enable_extended_mode

        # Create a Command
        command = CommandT()
        command.type = CommandType.ENCODER_COMMAND
        command.encoder_control = encoder_command

        # Serialize the Command
        builder = flatbuffers.Builder(1024)
        command.Pack(builder)
        builder.Finish()
        await self.send_ws(builder.Output())


    async def start_motor(self):
        command = EncoderCommands.StartMotorT()
        await self.send_command(command)

    async def stop_motor(self):
        command = EncoderCommands.StopMotorT()
        await self.send_command(command)

    async def torque_control_mode(self):
        command = EncoderCommands.TorqueControlModeT()
        await self.send_command(command)

    async def speed_torque_control_mode(self):
        command = EncoderCommands.SpeedTorqueControlModeT()
        await self.send_command(command)

    async def position_speed_torque_mode(self):
        command = EncoderCommands.PositionSpeedTorqueModeT()
        await self.send_command(command)

    async def set_motor_position_to_zero(self):
        command = EncoderCommands.SetMotorPositionToZeroT()
        await self.send_command(command)

    async def move_motor(self, position_degrees, speed, position_gain, position_damping, torque):
        command = EncoderCommands.MoveMotorT()
        command.position_degrees = position_degrees
        command.speed = speed
        command.position_gain = position_gain
        command.position_damping = position_damping
        command.torque = torque
        await self.send_command(command)

# Usage examples:
# motor_api = MotorAPI(1)
# asyncio.run(motor_api.init_motor(0))
# encoder_api = EncoderAPI(1)
# asyncio.run(encoder_api.enable_extended_mode())
