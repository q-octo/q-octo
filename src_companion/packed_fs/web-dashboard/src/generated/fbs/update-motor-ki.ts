// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';



export class UpdateMotorKi implements flatbuffers.IUnpackableObject<UpdateMotorKiT> {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):UpdateMotorKi {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsUpdateMotorKi(bb:flatbuffers.ByteBuffer, obj?:UpdateMotorKi):UpdateMotorKi {
  return (obj || new UpdateMotorKi()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsUpdateMotorKi(bb:flatbuffers.ByteBuffer, obj?:UpdateMotorKi):UpdateMotorKi {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new UpdateMotorKi()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

motorKi():number {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readFloat32(this.bb_pos + offset) : 0.0;
}

static startUpdateMotorKi(builder:flatbuffers.Builder) {
  builder.startObject(1);
}

static addMotorKi(builder:flatbuffers.Builder, motorKi:number) {
  builder.addFieldFloat32(0, motorKi, 0.0);
}

static endUpdateMotorKi(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createUpdateMotorKi(builder:flatbuffers.Builder, motorKi:number):flatbuffers.Offset {
  UpdateMotorKi.startUpdateMotorKi(builder);
  UpdateMotorKi.addMotorKi(builder, motorKi);
  return UpdateMotorKi.endUpdateMotorKi(builder);
}

unpack(): UpdateMotorKiT {
  return new UpdateMotorKiT(
    this.motorKi()
  );
}


unpackTo(_o: UpdateMotorKiT): void {
  _o.motorKi = this.motorKi();
}
}

export class UpdateMotorKiT implements flatbuffers.IGeneratedObject {
constructor(
  public motorKi: number = 0.0
){}


pack(builder:flatbuffers.Builder): flatbuffers.Offset {
  return UpdateMotorKi.createUpdateMotorKi(builder,
    this.motorKi
  );
}
}
