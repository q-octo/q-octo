# automatically generated by the FlatBuffers compiler, do not modify

# namespace: MotorControl

import flatbuffers
from flatbuffers.compat import import_numpy
np = import_numpy()

class SetRunMode(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAs(cls, buf, offset=0):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = SetRunMode()
        x.Init(buf, n + offset)
        return x

    @classmethod
    def GetRootAsSetRunMode(cls, buf, offset=0):
        """This method is deprecated. Please switch to GetRootAs."""
        return cls.GetRootAs(buf, offset)
    # SetRunMode
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

    # SetRunMode
    def Mode(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Int8Flags, o + self._tab.Pos)
        return 0

def SetRunModeStart(builder):
    builder.StartObject(1)

def Start(builder):
    SetRunModeStart(builder)

def SetRunModeAddMode(builder, mode):
    builder.PrependInt8Slot(0, mode, 0)

def AddMode(builder, mode):
    SetRunModeAddMode(builder, mode)

def SetRunModeEnd(builder):
    return builder.EndObject()

def End(builder):
    return SetRunModeEnd(builder)


class SetRunModeT(object):

    # SetRunModeT
    def __init__(self):
        self.mode = 0  # type: int

    @classmethod
    def InitFromBuf(cls, buf, pos):
        setRunMode = SetRunMode()
        setRunMode.Init(buf, pos)
        return cls.InitFromObj(setRunMode)

    @classmethod
    def InitFromPackedBuf(cls, buf, pos=0):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, pos)
        return cls.InitFromBuf(buf, pos+n)

    @classmethod
    def InitFromObj(cls, setRunMode):
        x = SetRunModeT()
        x._UnPack(setRunMode)
        return x

    # SetRunModeT
    def _UnPack(self, setRunMode):
        if setRunMode is None:
            return
        self.mode = setRunMode.Mode()

    # SetRunModeT
    def Pack(self, builder):
        SetRunModeStart(builder)
        SetRunModeAddMode(builder, self.mode)
        setRunMode = SetRunModeEnd(builder)
        return setRunMode
