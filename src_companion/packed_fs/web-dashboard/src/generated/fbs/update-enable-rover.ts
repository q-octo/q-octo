// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';



export class UpdateEnableRover implements flatbuffers.IUnpackableObject<UpdateEnableRoverT> {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):UpdateEnableRover {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsUpdateEnableRover(bb:flatbuffers.ByteBuffer, obj?:UpdateEnableRover):UpdateEnableRover {
  return (obj || new UpdateEnableRover()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsUpdateEnableRover(bb:flatbuffers.ByteBuffer, obj?:UpdateEnableRover):UpdateEnableRover {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new UpdateEnableRover()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

enableRover():boolean {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? !!this.bb!.readInt8(this.bb_pos + offset) : false;
}

static startUpdateEnableRover(builder:flatbuffers.Builder) {
  builder.startObject(1);
}

static addEnableRover(builder:flatbuffers.Builder, enableRover:boolean) {
  builder.addFieldInt8(0, +enableRover, +false);
}

static endUpdateEnableRover(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createUpdateEnableRover(builder:flatbuffers.Builder, enableRover:boolean):flatbuffers.Offset {
  UpdateEnableRover.startUpdateEnableRover(builder);
  UpdateEnableRover.addEnableRover(builder, enableRover);
  return UpdateEnableRover.endUpdateEnableRover(builder);
}

unpack(): UpdateEnableRoverT {
  return new UpdateEnableRoverT(
    this.enableRover()
  );
}


unpackTo(_o: UpdateEnableRoverT): void {
  _o.enableRover = this.enableRover();
}
}

export class UpdateEnableRoverT implements flatbuffers.IGeneratedObject {
constructor(
  public enableRover: boolean = false
){}


pack(builder:flatbuffers.Builder): flatbuffers.Offset {
  return UpdateEnableRover.createUpdateEnableRover(builder,
    this.enableRover
  );
}
}
