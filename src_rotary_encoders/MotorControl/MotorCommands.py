# automatically generated by the FlatBuffers compiler, do not modify

# namespace: MotorControl

class MotorCommands(object):
    NONE = 0
    InitMotor = 1
    EnableMotor = 2
    StopMotor = 3
    SetRunMode = 4
    SendMotionControl = 5
    SetLimitSpeed = 6
    SetLimitCurrent = 7
    SetLimitTorque = 8
    SetCurrentKp = 9
    SetCurrentKi = 10
    SetCurrentFilterGain = 11
    SetCurrentRef = 12
    SetPositionKp = 13
    SetPositionRef = 14
    SetSpeedKp = 15
    SetSpeedKi = 16
    SetSpeedRef = 17

def MotorCommandsCreator(unionType, table):
    from flatbuffers.table import Table
    if not isinstance(table, Table):
        return None
    if unionType == MotorCommands().InitMotor:
        import MotorControl.InitMotor
        return MotorControl.InitMotor.InitMotorT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().EnableMotor:
        import MotorControl.EnableMotor
        return MotorControl.EnableMotor.EnableMotorT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().StopMotor:
        import MotorControl.StopMotor
        return MotorControl.StopMotor.StopMotorT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SetRunMode:
        import MotorControl.SetRunMode
        return MotorControl.SetRunMode.SetRunModeT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SendMotionControl:
        import MotorControl.SendMotionControl
        return MotorControl.SendMotionControl.SendMotionControlT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SetLimitSpeed:
        import MotorControl.SetLimitSpeed
        return MotorControl.SetLimitSpeed.SetLimitSpeedT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SetLimitCurrent:
        import MotorControl.SetLimitCurrent
        return MotorControl.SetLimitCurrent.SetLimitCurrentT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SetLimitTorque:
        import MotorControl.SetLimitTorque
        return MotorControl.SetLimitTorque.SetLimitTorqueT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SetCurrentKp:
        import MotorControl.SetCurrentKp
        return MotorControl.SetCurrentKp.SetCurrentKpT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SetCurrentKi:
        import MotorControl.SetCurrentKi
        return MotorControl.SetCurrentKi.SetCurrentKiT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SetCurrentFilterGain:
        import MotorControl.SetCurrentFilterGain
        return MotorControl.SetCurrentFilterGain.SetCurrentFilterGainT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SetCurrentRef:
        import MotorControl.SetCurrentRef
        return MotorControl.SetCurrentRef.SetCurrentRefT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SetPositionKp:
        import MotorControl.SetPositionKp
        return MotorControl.SetPositionKp.SetPositionKpT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SetPositionRef:
        import MotorControl.SetPositionRef
        return MotorControl.SetPositionRef.SetPositionRefT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SetSpeedKp:
        import MotorControl.SetSpeedKp
        return MotorControl.SetSpeedKp.SetSpeedKpT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SetSpeedKi:
        import MotorControl.SetSpeedKi
        return MotorControl.SetSpeedKi.SetSpeedKiT.InitFromBuf(table.Bytes, table.Pos)
    if unionType == MotorCommands().SetSpeedRef:
        import MotorControl.SetSpeedRef
        return MotorControl.SetSpeedRef.SetSpeedRefT.InitFromBuf(table.Bytes, table.Pos)
    return None
