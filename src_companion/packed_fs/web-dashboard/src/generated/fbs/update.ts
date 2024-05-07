// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { UpdateBatteries, UpdateBatteriesT } from '../fbs/update-batteries.js';
import { UpdateCriticalVoltageThreshold, UpdateCriticalVoltageThresholdT } from '../fbs/update-critical-voltage-threshold.js';
import { UpdateCrsfLinkStatsTimeout, UpdateCrsfLinkStatsTimeoutT } from '../fbs/update-crsf-link-stats-timeout.js';
import { UpdateEnableRover, UpdateEnableRoverT } from '../fbs/update-enable-rover.js';
import { UpdateFoldWheels, UpdateFoldWheelsT } from '../fbs/update-fold-wheels.js';
import { UpdateLinkQualityThreshold, UpdateLinkQualityThresholdT } from '../fbs/update-link-quality-threshold.js';
import { UpdateLowVoltageThreshold, UpdateLowVoltageThresholdT } from '../fbs/update-low-voltage-threshold.js';
import { UpdateMaxCurrent, UpdateMaxCurrentT } from '../fbs/update-max-current.js';
import { UpdateMaxSpeed, UpdateMaxSpeedT } from '../fbs/update-max-speed.js';
import { UpdateMaxTorque, UpdateMaxTorqueT } from '../fbs/update-max-torque.js';
import { UpdateMotorKi, UpdateMotorKiT } from '../fbs/update-motor-ki.js';
import { UpdateMotorKp, UpdateMotorKpT } from '../fbs/update-motor-kp.js';
import { UpdateReferenceWheelAngle, UpdateReferenceWheelAngleT } from '../fbs/update-reference-wheel-angle.js';
import { UpdateRssiThreshold, UpdateRssiThresholdT } from '../fbs/update-rssi-threshold.js';
import { UpdateStartWebServerOnLaunch, UpdateStartWebServerOnLaunchT } from '../fbs/update-start-web-server-on-launch.js';
import { UpdateUnion, unionToUpdateUnion, unionListToUpdateUnion } from '../fbs/update-union.js';


export class Update implements flatbuffers.IUnpackableObject<UpdateT> {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):Update {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsUpdate(bb:flatbuffers.ByteBuffer, obj?:Update):Update {
  return (obj || new Update()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsUpdate(bb:flatbuffers.ByteBuffer, obj?:Update):Update {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new Update()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static bufferHasIdentifier(bb:flatbuffers.ByteBuffer):boolean {
  return bb.__has_identifier('UPDT');
}

updateType():UpdateUnion {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readUint8(this.bb_pos + offset) : UpdateUnion.NONE;
}

update<T extends flatbuffers.Table>(obj:any):any|null {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.__union(obj, this.bb_pos + offset) : null;
}

static startUpdate(builder:flatbuffers.Builder) {
  builder.startObject(2);
}

static addUpdateType(builder:flatbuffers.Builder, updateType:UpdateUnion) {
  builder.addFieldInt8(0, updateType, UpdateUnion.NONE);
}

static addUpdate(builder:flatbuffers.Builder, updateOffset:flatbuffers.Offset) {
  builder.addFieldOffset(1, updateOffset, 0);
}

static endUpdate(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static finishUpdateBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, 'UPDT');
}

static finishSizePrefixedUpdateBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, 'UPDT', true);
}

static createUpdate(builder:flatbuffers.Builder, updateType:UpdateUnion, updateOffset:flatbuffers.Offset):flatbuffers.Offset {
  Update.startUpdate(builder);
  Update.addUpdateType(builder, updateType);
  Update.addUpdate(builder, updateOffset);
  return Update.endUpdate(builder);
}

unpack(): UpdateT {
  return new UpdateT(
    this.updateType(),
    (() => {
      const temp = unionToUpdateUnion(this.updateType(), this.update.bind(this));
      if(temp === null) { return null; }
      return temp.unpack()
  })()
  );
}


unpackTo(_o: UpdateT): void {
  _o.updateType = this.updateType();
  _o.update = (() => {
      const temp = unionToUpdateUnion(this.updateType(), this.update.bind(this));
      if(temp === null) { return null; }
      return temp.unpack()
  })();
}
}

export class UpdateT implements flatbuffers.IGeneratedObject {
constructor(
  public updateType: UpdateUnion = UpdateUnion.NONE,
  public update: UpdateBatteriesT|UpdateCriticalVoltageThresholdT|UpdateCrsfLinkStatsTimeoutT|UpdateEnableRoverT|UpdateFoldWheelsT|UpdateLinkQualityThresholdT|UpdateLowVoltageThresholdT|UpdateMaxCurrentT|UpdateMaxSpeedT|UpdateMaxTorqueT|UpdateMotorKiT|UpdateMotorKpT|UpdateReferenceWheelAngleT|UpdateRssiThresholdT|UpdateStartWebServerOnLaunchT|null = null
){}


pack(builder:flatbuffers.Builder): flatbuffers.Offset {
  const update = builder.createObjectOffset(this.update);

  return Update.createUpdate(builder,
    this.updateType,
    update
  );
}
}
