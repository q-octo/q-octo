// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import { ButtonPressed, ButtonPressedT } from '../fbs/button-pressed.js';


export enum CompanionTxUnion {
  NONE = 0,
  ButtonPressed = 1
}

export function unionToCompanionTxUnion(
  type: CompanionTxUnion,
  accessor: (obj:ButtonPressed) => ButtonPressed|null
): ButtonPressed|null {
  switch(CompanionTxUnion[type]) {
    case 'NONE': return null; 
    case 'ButtonPressed': return accessor(new ButtonPressed())! as ButtonPressed;
    default: return null;
  }
}

export function unionListToCompanionTxUnion(
  type: CompanionTxUnion, 
  accessor: (index: number, obj:ButtonPressed) => ButtonPressed|null, 
  index: number
): ButtonPressed|null {
  switch(CompanionTxUnion[type]) {
    case 'NONE': return null; 
    case 'ButtonPressed': return accessor(index, new ButtonPressed())! as ButtonPressed;
    default: return null;
  }
}
