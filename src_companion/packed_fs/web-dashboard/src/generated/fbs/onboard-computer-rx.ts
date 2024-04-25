// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { CrsfData, CrsfDataT } from '../fbs/crsf-data.js';
import { OnboardComputerRxUnion, unionToOnboardComputerRxUnion, unionListToOnboardComputerRxUnion } from '../fbs/onboard-computer-rx-union.js';
import { Robot, RobotT } from '../fbs/robot.js';


export class OnboardComputerRx implements flatbuffers.IUnpackableObject<OnboardComputerRxT> {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):OnboardComputerRx {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsOnboardComputerRx(bb:flatbuffers.ByteBuffer, obj?:OnboardComputerRx):OnboardComputerRx {
  return (obj || new OnboardComputerRx()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsOnboardComputerRx(bb:flatbuffers.ByteBuffer, obj?:OnboardComputerRx):OnboardComputerRx {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new OnboardComputerRx()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static bufferHasIdentifier(bb:flatbuffers.ByteBuffer):boolean {
  return bb.__has_identifier('OCRX');
}

messageType():OnboardComputerRxUnion {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readUint8(this.bb_pos + offset) : OnboardComputerRxUnion.NONE;
}

message<T extends flatbuffers.Table>(obj:any):any|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.__union(obj, this.bb_pos + offset) : null;
}

static startOnboardComputerRx(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addMessageType(builder:flatbuffers.Builder, messageType:OnboardComputerRxUnion) {
  builder.addFieldInt8(0, messageType, OnboardComputerRxUnion.NONE);
}

static addMessage(builder:flatbuffers.Builder, messageOffset:flatbuffers.Offset) {
  builder.addFieldOffset(1, messageOffset, 0);
}

static endOnboardComputerRx(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static finishOnboardComputerRxBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, 'OCRX');
}

static finishSizePrefixedOnboardComputerRxBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, 'OCRX', true);
}

static createOnboardComputerRx(builder:flatbuffers.Builder, messageType:OnboardComputerRxUnion, messageOffset:flatbuffers.Offset):flatbuffers.Offset {
  OnboardComputerRx.startOnboardComputerRx(builder);
  OnboardComputerRx.addMessageType(builder, messageType);
  OnboardComputerRx.addMessage(builder, messageOffset);
  return OnboardComputerRx.endOnboardComputerRx(builder);
}

unpack(): OnboardComputerRxT {
  return new OnboardComputerRxT(
    this.messageType(),
    (() => {
      const temp = unionToOnboardComputerRxUnion(this.messageType(), this.message.bind(this));
      if(temp === null) { return null; }
      return temp.unpack()
  })()
  );
}


unpackTo(_o: OnboardComputerRxT): void {
  _o.messageType = this.messageType();
  _o.message = (() => {
      const temp = unionToOnboardComputerRxUnion(this.messageType(), this.message.bind(this));
      if(temp === null) { return null; }
      return temp.unpack()
  })();
}
}

export class OnboardComputerRxT implements flatbuffers.IGeneratedObject {
constructor(
  public messageType: OnboardComputerRxUnion = OnboardComputerRxUnion.NONE,
  public message: CrsfDataT|RobotT|null = null
){}


pack(builder:flatbuffers.Builder): flatbuffers.Offset {
  const message = builder.createObjectOffset(this.message);

  return OnboardComputerRx.createOnboardComputerRx(builder,
    this.messageType,
    message
  );
}
}