# automatically generated by the FlatBuffers compiler, do not modify

# namespace: MotorControl

import flatbuffers
from flatbuffers.compat import import_numpy
np = import_numpy()

class SetCurrentKi(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAs(cls, buf, offset=0):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = SetCurrentKi()
        x.Init(buf, n + offset)
        return x

    @classmethod
    def GetRootAsSetCurrentKi(cls, buf, offset=0):
        """This method is deprecated. Please switch to GetRootAs."""
        return cls.GetRootAs(buf, offset)
    # SetCurrentKi
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

    # SetCurrentKi
    def Ki(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Float32Flags, o + self._tab.Pos)
        return 0.0

def SetCurrentKiStart(builder):
    builder.StartObject(1)

def Start(builder):
    SetCurrentKiStart(builder)

def SetCurrentKiAddKi(builder, ki):
    builder.PrependFloat32Slot(0, ki, 0.0)

def AddKi(builder, ki):
    SetCurrentKiAddKi(builder, ki)

def SetCurrentKiEnd(builder):
    return builder.EndObject()

def End(builder):
    return SetCurrentKiEnd(builder)


class SetCurrentKiT(object):

    # SetCurrentKiT
    def __init__(self):
        self.ki = 0.0  # type: float

    @classmethod
    def InitFromBuf(cls, buf, pos):
        setCurrentKi = SetCurrentKi()
        setCurrentKi.Init(buf, pos)
        return cls.InitFromObj(setCurrentKi)

    @classmethod
    def InitFromPackedBuf(cls, buf, pos=0):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, pos)
        return cls.InitFromBuf(buf, pos+n)

    @classmethod
    def InitFromObj(cls, setCurrentKi):
        x = SetCurrentKiT()
        x._UnPack(setCurrentKi)
        return x

    # SetCurrentKiT
    def _UnPack(self, setCurrentKi):
        if setCurrentKi is None:
            return
        self.ki = setCurrentKi.Ki()

    # SetCurrentKiT
    def Pack(self, builder):
        SetCurrentKiStart(builder)
        SetCurrentKiAddKi(builder, self.ki)
        setCurrentKi = SetCurrentKiEnd(builder)
        return setCurrentKi
