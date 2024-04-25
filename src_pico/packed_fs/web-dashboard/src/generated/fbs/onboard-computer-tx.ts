// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { OnboardComputerTxUnion, unionToOnboardComputerTxUnion, unionListToOnboardComputerTxUnion } from '../fbs/onboard-computer-tx-union.js';


export class OnboardComputerTx {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):OnboardComputerTx {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsOnboardComputerTx(bb:flatbuffers.ByteBuffer, obj?:OnboardComputerTx):OnboardComputerTx {
  return (obj || new OnboardComputerTx()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsOnboardComputerTx(bb:flatbuffers.ByteBuffer, obj?:OnboardComputerTx):OnboardComputerTx {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new OnboardComputerTx()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static bufferHasIdentifier(bb:flatbuffers.ByteBuffer):boolean {
  return bb.__has_identifier('OCTX');
}

messageType():OnboardComputerTxUnion {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readUint8(this.bb_pos + offset) : OnboardComputerTxUnion.NONE;
}

message<T extends flatbuffers.Table>(obj:any):any|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.__union(obj, this.bb_pos + offset) : null;
}

static startOnboardComputerTx(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addMessageType(builder:flatbuffers.Builder, messageType:OnboardComputerTxUnion) {
  builder.addFieldInt8(0, messageType, OnboardComputerTxUnion.NONE);
}

static addMessage(builder:flatbuffers.Builder, messageOffset:flatbuffers.Offset) {
  builder.addFieldOffset(1, messageOffset, 0);
}

static endOnboardComputerTx(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static finishOnboardComputerTxBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, 'OCTX');
}

static finishSizePrefixedOnboardComputerTxBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, 'OCTX', true);
}

static createOnboardComputerTx(builder:flatbuffers.Builder, messageType:OnboardComputerTxUnion, messageOffset:flatbuffers.Offset):flatbuffers.Offset {
  OnboardComputerTx.startOnboardComputerTx(builder);
  OnboardComputerTx.addMessageType(builder, messageType);
  OnboardComputerTx.addMessage(builder, messageOffset);
  return OnboardComputerTx.endOnboardComputerTx(builder);
}
}
