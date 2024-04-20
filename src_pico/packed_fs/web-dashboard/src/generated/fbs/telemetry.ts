// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

export class Telemetry {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):Telemetry {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsTelemetry(bb:flatbuffers.ByteBuffer, obj?:Telemetry):Telemetry {
  return (obj || new Telemetry()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsTelemetry(bb:flatbuffers.ByteBuffer, obj?:Telemetry):Telemetry {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new Telemetry()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

/**
 * Signal strength indicator
 */
rssi():number {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readUint8(this.bb_pos + offset) : 0;
}

/**
 * Link quality
 */
linkQuality():number {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.readUint8(this.bb_pos + offset) : 0;
}

/**
 * Signal to noise ratio
 */
snr():number {
  const offset = this.bb!.__offset(this.bb_pos, 8);
  return offset ? this.bb!.readInt8(this.bb_pos + offset) : 0;
}

/**
 * Transmission power
 */
txPower():number {
  const offset = this.bb!.__offset(this.bb_pos, 10);
  return offset ? this.bb!.readUint16(this.bb_pos + offset) : 0;
}

static startTelemetry(builder:flatbuffers.Builder) {
  builder.startObject(4);
}

static addRssi(builder:flatbuffers.Builder, rssi:number) {
  builder.addFieldInt8(0, rssi, 0);
}

static addLinkQuality(builder:flatbuffers.Builder, linkQuality:number) {
  builder.addFieldInt8(1, linkQuality, 0);
}

static addSnr(builder:flatbuffers.Builder, snr:number) {
  builder.addFieldInt8(2, snr, 0);
}

static addTxPower(builder:flatbuffers.Builder, txPower:number) {
  builder.addFieldInt16(3, txPower, 0);
}

static endTelemetry(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createTelemetry(builder:flatbuffers.Builder, rssi:number, linkQuality:number, snr:number, txPower:number):flatbuffers.Offset {
  Telemetry.startTelemetry(builder);
  Telemetry.addRssi(builder, rssi);
  Telemetry.addLinkQuality(builder, linkQuality);
  Telemetry.addSnr(builder, snr);
  Telemetry.addTxPower(builder, txPower);
  return Telemetry.endTelemetry(builder);
}
}
