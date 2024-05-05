// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';



export class UpdateMaxCurrent implements flatbuffers.IUnpackableObject<UpdateMaxCurrentT> {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):UpdateMaxCurrent {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsUpdateMaxCurrent(bb:flatbuffers.ByteBuffer, obj?:UpdateMaxCurrent):UpdateMaxCurrent {
  return (obj || new UpdateMaxCurrent()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsUpdateMaxCurrent(bb:flatbuffers.ByteBuffer, obj?:UpdateMaxCurrent):UpdateMaxCurrent {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new UpdateMaxCurrent()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

maxCurrent():number {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readFloat32(this.bb_pos + offset) : 0.0;
}

static startUpdateMaxCurrent(builder:flatbuffers.Builder) {
  builder.startObject(1);
}

static addMaxCurrent(builder:flatbuffers.Builder, maxCurrent:number) {
  builder.addFieldFloat32(0, maxCurrent, 0.0);
}

static endUpdateMaxCurrent(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createUpdateMaxCurrent(builder:flatbuffers.Builder, maxCurrent:number):flatbuffers.Offset {
  UpdateMaxCurrent.startUpdateMaxCurrent(builder);
  UpdateMaxCurrent.addMaxCurrent(builder, maxCurrent);
  return UpdateMaxCurrent.endUpdateMaxCurrent(builder);
}

unpack(): UpdateMaxCurrentT {
  return new UpdateMaxCurrentT(
    this.maxCurrent()
  );
}


unpackTo(_o: UpdateMaxCurrentT): void {
  _o.maxCurrent = this.maxCurrent();
}
}

export class UpdateMaxCurrentT implements flatbuffers.IGeneratedObject {
constructor(
  public maxCurrent: number = 0.0
){}


pack(builder:flatbuffers.Builder): flatbuffers.Offset {
  return UpdateMaxCurrent.createUpdateMaxCurrent(builder,
    this.maxCurrent
  );
}
}