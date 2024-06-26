// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';



export class UpdateRssiThreshold implements flatbuffers.IUnpackableObject<UpdateRssiThresholdT> {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):UpdateRssiThreshold {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsUpdateRssiThreshold(bb:flatbuffers.ByteBuffer, obj?:UpdateRssiThreshold):UpdateRssiThreshold {
  return (obj || new UpdateRssiThreshold()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsUpdateRssiThreshold(bb:flatbuffers.ByteBuffer, obj?:UpdateRssiThreshold):UpdateRssiThreshold {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new UpdateRssiThreshold()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

rssiThreshold():number {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readInt32(this.bb_pos + offset) : 0;
}

static startUpdateRssiThreshold(builder:flatbuffers.Builder) {
  builder.startObject(1);
}

static addRssiThreshold(builder:flatbuffers.Builder, rssiThreshold:number) {
  builder.addFieldInt32(0, rssiThreshold, 0);
}

static endUpdateRssiThreshold(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createUpdateRssiThreshold(builder:flatbuffers.Builder, rssiThreshold:number):flatbuffers.Offset {
  UpdateRssiThreshold.startUpdateRssiThreshold(builder);
  UpdateRssiThreshold.addRssiThreshold(builder, rssiThreshold);
  return UpdateRssiThreshold.endUpdateRssiThreshold(builder);
}

unpack(): UpdateRssiThresholdT {
  return new UpdateRssiThresholdT(
    this.rssiThreshold()
  );
}


unpackTo(_o: UpdateRssiThresholdT): void {
  _o.rssiThreshold = this.rssiThreshold();
}
}

export class UpdateRssiThresholdT implements flatbuffers.IGeneratedObject {
constructor(
  public rssiThreshold: number = 0
){}


pack(builder:flatbuffers.Builder): flatbuffers.Offset {
  return UpdateRssiThreshold.createUpdateRssiThreshold(builder,
    this.rssiThreshold
  );
}
}
