// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { DriveConfig, unionToDriveConfig, unionListToDriveConfig } from '../fbs/drive-config.js';
import { MixedDriveConfig, MixedDriveConfigT } from '../fbs/mixed-drive-config.js';
import { TankDriveConfig, TankDriveConfigT } from '../fbs/tank-drive-config.js';


export class DriveRobot implements flatbuffers.IUnpackableObject<DriveRobotT> {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):DriveRobot {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsDriveRobot(bb:flatbuffers.ByteBuffer, obj?:DriveRobot):DriveRobot {
  return (obj || new DriveRobot()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsDriveRobot(bb:flatbuffers.ByteBuffer, obj?:DriveRobot):DriveRobot {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new DriveRobot()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

configType():DriveConfig {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readUint8(this.bb_pos + offset) : DriveConfig.NONE;
}

config<T extends flatbuffers.Table>(obj:any):any|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.__union(obj, this.bb_pos + offset) : null;
}

static startDriveRobot(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addConfigType(builder:flatbuffers.Builder, configType:DriveConfig) {
  builder.addFieldInt8(0, configType, DriveConfig.NONE);
}

static addConfig(builder:flatbuffers.Builder, configOffset:flatbuffers.Offset) {
  builder.addFieldOffset(1, configOffset, 0);
}

static endDriveRobot(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createDriveRobot(builder:flatbuffers.Builder, configType:DriveConfig, configOffset:flatbuffers.Offset):flatbuffers.Offset {
  DriveRobot.startDriveRobot(builder);
  DriveRobot.addConfigType(builder, configType);
  DriveRobot.addConfig(builder, configOffset);
  return DriveRobot.endDriveRobot(builder);
}

unpack(): DriveRobotT {
  return new DriveRobotT(
    this.configType(),
    (() => {
      const temp = unionToDriveConfig(this.configType(), this.config.bind(this));
      if(temp === null) { return null; }
      return temp.unpack()
  })()
  );
}


unpackTo(_o: DriveRobotT): void {
  _o.configType = this.configType();
  _o.config = (() => {
      const temp = unionToDriveConfig(this.configType(), this.config.bind(this));
      if(temp === null) { return null; }
      return temp.unpack()
  })();
}
}

export class DriveRobotT implements flatbuffers.IGeneratedObject {
constructor(
  public configType: DriveConfig = DriveConfig.NONE,
  public config: MixedDriveConfigT|TankDriveConfigT|null = null
){}


pack(builder:flatbuffers.Builder): flatbuffers.Offset {
  const config = builder.createObjectOffset(this.config);

  return DriveRobot.createDriveRobot(builder,
    this.configType,
    config
  );
}
}
