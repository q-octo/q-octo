// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { CrsfChannels, CrsfChannelsT } from '../fbs/crsf-channels.js';
import { CrsfTelemetry, CrsfTelemetryT } from '../fbs/crsf-telemetry.js';


export class CrsfData implements flatbuffers.IUnpackableObject<CrsfDataT> {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):CrsfData {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsCrsfData(bb:flatbuffers.ByteBuffer, obj?:CrsfData):CrsfData {
  return (obj || new CrsfData()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsCrsfData(bb:flatbuffers.ByteBuffer, obj?:CrsfData):CrsfData {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new CrsfData()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

channels(obj?:CrsfChannels):CrsfChannels|null {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? (obj || new CrsfChannels()).__init(this.bb!.__indirect(this.bb_pos + offset), this.bb!) : null;
}

/**
 * Telemetry data associated with the RC frame
 */
telemetry(obj?:CrsfTelemetry):CrsfTelemetry|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? (obj || new CrsfTelemetry()).__init(this.bb!.__indirect(this.bb_pos + offset), this.bb!) : null;
}

/**
 * Failsafe flag with a default value of true
 */
failsafe():boolean {
  const offset = this.bb!.__offset(this.bb_pos, 8);
  return offset ? !!this.bb!.readInt8(this.bb_pos + offset) : true;
}

static startCrsfData(builder:flatbuffers.Builder) {
  builder.startObject(3);
}

static addChannels(builder:flatbuffers.Builder, channelsOffset:flatbuffers.Offset) {
  builder.addFieldOffset(0, channelsOffset, 0);
}

static addTelemetry(builder:flatbuffers.Builder, telemetryOffset:flatbuffers.Offset) {
  builder.addFieldOffset(1, telemetryOffset, 0);
}

static addFailsafe(builder:flatbuffers.Builder, failsafe:boolean) {
  builder.addFieldInt8(2, +failsafe, +true);
}

static endCrsfData(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}


unpack(): CrsfDataT {
  return new CrsfDataT(
    (this.channels() !== null ? this.channels()!.unpack() : null),
    (this.telemetry() !== null ? this.telemetry()!.unpack() : null),
    this.failsafe()
  );
}


unpackTo(_o: CrsfDataT): void {
  _o.channels = (this.channels() !== null ? this.channels()!.unpack() : null);
  _o.telemetry = (this.telemetry() !== null ? this.telemetry()!.unpack() : null);
  _o.failsafe = this.failsafe();
}
}

export class CrsfDataT implements flatbuffers.IGeneratedObject {
constructor(
  public channels: CrsfChannelsT|null = null,
  public telemetry: CrsfTelemetryT|null = null,
  public failsafe: boolean = true
){}


pack(builder:flatbuffers.Builder): flatbuffers.Offset {
  const channels = (this.channels !== null ? this.channels!.pack(builder) : 0);
  const telemetry = (this.telemetry !== null ? this.telemetry!.pack(builder) : 0);

  CrsfData.startCrsfData(builder);
  CrsfData.addChannels(builder, channels);
  CrsfData.addTelemetry(builder, telemetry);
  CrsfData.addFailsafe(builder, this.failsafe);

  return CrsfData.endCrsfData(builder);
}
}
