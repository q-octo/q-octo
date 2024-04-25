// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { DriveConfig, unionToDriveConfig, unionListToDriveConfig } from '../fbs/drive-config.js';


export class RobotDriveConfig {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):RobotDriveConfig {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsRobotDriveConfig(bb:flatbuffers.ByteBuffer, obj?:RobotDriveConfig):RobotDriveConfig {
  return (obj || new RobotDriveConfig()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsRobotDriveConfig(bb:flatbuffers.ByteBuffer, obj?:RobotDriveConfig):RobotDriveConfig {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new RobotDriveConfig()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static bufferHasIdentifier(bb:flatbuffers.ByteBuffer):boolean {
  return bb.__has_identifier('OCDV');
}

configType():DriveConfig {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readUint8(this.bb_pos + offset) : DriveConfig.NONE;
}

config<T extends flatbuffers.Table>(obj:any):any|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.__union(obj, this.bb_pos + offset) : null;
}

static startRobotDriveConfig(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addConfigType(builder:flatbuffers.Builder, configType:DriveConfig) {
  builder.addFieldInt8(0, configType, DriveConfig.NONE);
}

static addConfig(builder:flatbuffers.Builder, configOffset:flatbuffers.Offset) {
  builder.addFieldOffset(1, configOffset, 0);
}

static endRobotDriveConfig(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static finishRobotDriveConfigBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, 'OCDV');
}

static finishSizePrefixedRobotDriveConfigBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, 'OCDV', true);
}

static createRobotDriveConfig(builder:flatbuffers.Builder, configType:DriveConfig, configOffset:flatbuffers.Offset):flatbuffers.Offset {
  RobotDriveConfig.startRobotDriveConfig(builder);
  RobotDriveConfig.addConfigType(builder, configType);
  RobotDriveConfig.addConfig(builder, configOffset);
  return RobotDriveConfig.endRobotDriveConfig(builder);
}
}