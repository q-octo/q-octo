# automatically generated by the FlatBuffers compiler, do not modify

# namespace: MotorControl

import flatbuffers
from flatbuffers.compat import import_numpy
np = import_numpy()

class EnableMotor(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAs(cls, buf, offset=0):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = EnableMotor()
        x.Init(buf, n + offset)
        return x

    @classmethod
    def GetRootAsEnableMotor(cls, buf, offset=0):
        """This method is deprecated. Please switch to GetRootAs."""
        return cls.GetRootAs(buf, offset)
    # EnableMotor
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

def EnableMotorStart(builder):
    builder.StartObject(0)

def Start(builder):
    EnableMotorStart(builder)

def EnableMotorEnd(builder):
    return builder.EndObject()

def End(builder):
    return EnableMotorEnd(builder)


class EnableMotorT(object):

    # EnableMotorT
    def __init__(self):
        pass

    @classmethod
    def InitFromBuf(cls, buf, pos):
        enableMotor = EnableMotor()
        enableMotor.Init(buf, pos)
        return cls.InitFromObj(enableMotor)

    @classmethod
    def InitFromPackedBuf(cls, buf, pos=0):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, pos)
        return cls.InitFromBuf(buf, pos+n)

    @classmethod
    def InitFromObj(cls, enableMotor):
        x = EnableMotorT()
        x._UnPack(enableMotor)
        return x

    # EnableMotorT
    def _UnPack(self, enableMotor):
        if enableMotor is None:
            return

    # EnableMotorT
    def Pack(self, builder):
        EnableMotorStart(builder)
        enableMotor = EnableMotorEnd(builder)
        return enableMotor
