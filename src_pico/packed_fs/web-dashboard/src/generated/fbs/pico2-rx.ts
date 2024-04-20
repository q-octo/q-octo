// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { Button } from '../fbs/button.js';


export class Pico2RX {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):Pico2RX {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsPico2RX(bb:flatbuffers.ByteBuffer, obj?:Pico2RX):Pico2RX {
  return (obj || new Pico2RX()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsPico2RX(bb:flatbuffers.ByteBuffer, obj?:Pico2RX):Pico2RX {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new Pico2RX()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static bufferHasIdentifier(bb:flatbuffers.ByteBuffer):boolean {
  return bb.__has_identifier('PCO2');
}

button():Button {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readInt8(this.bb_pos + offset) : Button.A;
}

static startPico2RX(builder:flatbuffers.Builder) {
  builder.startObject(1);
}

static addButton(builder:flatbuffers.Builder, button:Button) {
  builder.addFieldInt8(0, button, Button.A);
}

static endPico2RX(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static finishPico2RXBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, 'PCO2');
}

static finishSizePrefixedPico2RXBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, 'PCO2', true);
}

static createPico2RX(builder:flatbuffers.Builder, button:Button):flatbuffers.Offset {
  Pico2RX.startPico2RX(builder);
  Pico2RX.addButton(builder, button);
  return Pico2RX.endPico2RX(builder);
}
}
