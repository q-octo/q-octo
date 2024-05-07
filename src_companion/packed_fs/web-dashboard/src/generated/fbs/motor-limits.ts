// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';



export class MotorLimits implements flatbuffers.IUnpackableObject<MotorLimitsT> {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):MotorLimits {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsMotorLimits(bb:flatbuffers.ByteBuffer, obj?:MotorLimits):MotorLimits {
  return (obj || new MotorLimits()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsMotorLimits(bb:flatbuffers.ByteBuffer, obj?:MotorLimits):MotorLimits {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new MotorLimits()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

maxSpeed():number {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readFloat32(this.bb_pos + offset) : -1.0;
}

maxCurrent():number {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.readFloat32(this.bb_pos + offset) : -1.0;
}

maxTorque():number {
  const offset = this.bb!.__offset(this.bb_pos, 8);
  return offset ? this.bb!.readFloat32(this.bb_pos + offset) : -1.0;
}

speedKp():number {
  const offset = this.bb!.__offset(this.bb_pos, 10);
  return offset ? this.bb!.readFloat32(this.bb_pos + offset) : 1.0;
}

speedKi():number {
  const offset = this.bb!.__offset(this.bb_pos, 12);
  return offset ? this.bb!.readFloat32(this.bb_pos + offset) : 0.002;
}

static startMotorLimits(builder:flatbuffers.Builder) {
  builder.startObject(5);
}

static addMaxSpeed(builder:flatbuffers.Builder, maxSpeed:number) {
  builder.addFieldFloat32(0, maxSpeed, -1.0);
}

static addMaxCurrent(builder:flatbuffers.Builder, maxCurrent:number) {
  builder.addFieldFloat32(1, maxCurrent, -1.0);
}

static addMaxTorque(builder:flatbuffers.Builder, maxTorque:number) {
  builder.addFieldFloat32(2, maxTorque, -1.0);
}

static addSpeedKp(builder:flatbuffers.Builder, speedKp:number) {
  builder.addFieldFloat32(3, speedKp, 1.0);
}

static addSpeedKi(builder:flatbuffers.Builder, speedKi:number) {
  builder.addFieldFloat32(4, speedKi, 0.002);
}

static endMotorLimits(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static createMotorLimits(builder:flatbuffers.Builder, maxSpeed:number, maxCurrent:number, maxTorque:number, speedKp:number, speedKi:number):flatbuffers.Offset {
  MotorLimits.startMotorLimits(builder);
  MotorLimits.addMaxSpeed(builder, maxSpeed);
  MotorLimits.addMaxCurrent(builder, maxCurrent);
  MotorLimits.addMaxTorque(builder, maxTorque);
  MotorLimits.addSpeedKp(builder, speedKp);
  MotorLimits.addSpeedKi(builder, speedKi);
  return MotorLimits.endMotorLimits(builder);
}

unpack(): MotorLimitsT {
  return new MotorLimitsT(
    this.maxSpeed(),
    this.maxCurrent(),
    this.maxTorque(),
    this.speedKp(),
    this.speedKi()
  );
}


unpackTo(_o: MotorLimitsT): void {
  _o.maxSpeed = this.maxSpeed();
  _o.maxCurrent = this.maxCurrent();
  _o.maxTorque = this.maxTorque();
  _o.speedKp = this.speedKp();
  _o.speedKi = this.speedKi();
}
}

export class MotorLimitsT implements flatbuffers.IGeneratedObject {
constructor(
  public maxSpeed: number = -1.0,
  public maxCurrent: number = -1.0,
  public maxTorque: number = -1.0,
  public speedKp: number = 1.0,
  public speedKi: number = 0.002
){}


pack(builder:flatbuffers.Builder): flatbuffers.Offset {
  return MotorLimits.createMotorLimits(builder,
    this.maxSpeed,
    this.maxCurrent,
    this.maxTorque,
    this.speedKp,
    this.speedKi
  );
}
}
