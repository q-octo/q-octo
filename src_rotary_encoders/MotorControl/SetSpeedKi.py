# automatically generated by the FlatBuffers compiler, do not modify

# namespace: MotorControl

import flatbuffers
from flatbuffers.compat import import_numpy
np = import_numpy()

class SetSpeedKi(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAs(cls, buf, offset=0):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = SetSpeedKi()
        x.Init(buf, n + offset)
        return x

    @classmethod
    def GetRootAsSetSpeedKi(cls, buf, offset=0):
        """This method is deprecated. Please switch to GetRootAs."""
        return cls.GetRootAs(buf, offset)
    # SetSpeedKi
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

    # SetSpeedKi
    def Ki(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Float32Flags, o + self._tab.Pos)
        return 0.0

def SetSpeedKiStart(builder):
    builder.StartObject(1)

def Start(builder):
    SetSpeedKiStart(builder)

def SetSpeedKiAddKi(builder, ki):
    builder.PrependFloat32Slot(0, ki, 0.0)

def AddKi(builder, ki):
    SetSpeedKiAddKi(builder, ki)

def SetSpeedKiEnd(builder):
    return builder.EndObject()

def End(builder):
    return SetSpeedKiEnd(builder)


class SetSpeedKiT(object):

    # SetSpeedKiT
    def __init__(self):
        self.ki = 0.0  # type: float

    @classmethod
    def InitFromBuf(cls, buf, pos):
        setSpeedKi = SetSpeedKi()
        setSpeedKi.Init(buf, pos)
        return cls.InitFromObj(setSpeedKi)

    @classmethod
    def InitFromPackedBuf(cls, buf, pos=0):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, pos)
        return cls.InitFromBuf(buf, pos+n)

    @classmethod
    def InitFromObj(cls, setSpeedKi):
        x = SetSpeedKiT()
        x._UnPack(setSpeedKi)
        return x

    # SetSpeedKiT
    def _UnPack(self, setSpeedKi):
        if setSpeedKi is None:
            return
        self.ki = setSpeedKi.Ki()

    # SetSpeedKiT
    def Pack(self, builder):
        SetSpeedKiStart(builder)
        SetSpeedKiAddKi(builder, self.ki)
        setSpeedKi = SetSpeedKiEnd(builder)
        return setSpeedKi
