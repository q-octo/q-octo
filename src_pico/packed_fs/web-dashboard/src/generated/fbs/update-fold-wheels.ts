// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

export class UpdateFoldWheels {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):UpdateFoldWheels {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsUpdateFoldWheels(bb:flatbuffers.ByteBuffer, obj?:UpdateFoldWheels):UpdateFoldWheels {
  return (obj || new UpdateFoldWheels()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsUpdateFoldWheels(bb:flatbuffers.ByteBuffer, obj?:UpdateFoldWheels):UpdateFoldWheels {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new UpdateFoldWheels()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static startUpdateFoldWheels(builder:flatbuffers.Builder) {
  builder.startObject(0);
}

static endUpdateFoldWheels(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createUpdateFoldWheels(builder:flatbuffers.Builder):flatbuffers.Offset {
  UpdateFoldWheels.startUpdateFoldWheels(builder);
  return UpdateFoldWheels.endUpdateFoldWheels(builder);
}
}