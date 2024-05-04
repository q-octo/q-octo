# automatically generated by the FlatBuffers compiler, do not modify

# namespace: MotorControl

import flatbuffers
from flatbuffers.compat import import_numpy
np = import_numpy()

class SetPositionKp(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAs(cls, buf, offset=0):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = SetPositionKp()
        x.Init(buf, n + offset)
        return x

    @classmethod
    def GetRootAsSetPositionKp(cls, buf, offset=0):
        """This method is deprecated. Please switch to GetRootAs."""
        return cls.GetRootAs(buf, offset)
    # SetPositionKp
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

    # SetPositionKp
    def Kp(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Float32Flags, o + self._tab.Pos)
        return 0.0

def SetPositionKpStart(builder):
    builder.StartObject(1)

def Start(builder):
    SetPositionKpStart(builder)

def SetPositionKpAddKp(builder, kp):
    builder.PrependFloat32Slot(0, kp, 0.0)

def AddKp(builder, kp):
    SetPositionKpAddKp(builder, kp)

def SetPositionKpEnd(builder):
    return builder.EndObject()

def End(builder):
    return SetPositionKpEnd(builder)


class SetPositionKpT(object):

    # SetPositionKpT
    def __init__(self):
        self.kp = 0.0  # type: float

    @classmethod
    def InitFromBuf(cls, buf, pos):
        setPositionKp = SetPositionKp()
        setPositionKp.Init(buf, pos)
        return cls.InitFromObj(setPositionKp)

    @classmethod
    def InitFromPackedBuf(cls, buf, pos=0):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, pos)
        return cls.InitFromBuf(buf, pos+n)

    @classmethod
    def InitFromObj(cls, setPositionKp):
        x = SetPositionKpT()
        x._UnPack(setPositionKp)
        return x

    # SetPositionKpT
    def _UnPack(self, setPositionKp):
        if setPositionKp is None:
            return
        self.kp = setPositionKp.Kp()

    # SetPositionKpT
    def Pack(self, builder):
        SetPositionKpStart(builder)
        SetPositionKpAddKp(builder, self.kp)
        setPositionKp = SetPositionKpEnd(builder)
        return setPositionKp
