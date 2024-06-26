// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import { Robot, RobotT } from '../fbs/robot.js';


export enum CompanionRxUnion {
  NONE = 0,
  Robot = 1
}

export function unionToCompanionRxUnion(
  type: CompanionRxUnion,
  accessor: (obj:Robot) => Robot|null
): Robot|null {
  switch(CompanionRxUnion[type]) {
    case 'NONE': return null; 
    case 'Robot': return accessor(new Robot())! as Robot;
    default: return null;
  }
}

export function unionListToCompanionRxUnion(
  type: CompanionRxUnion, 
  accessor: (index: number, obj:Robot) => Robot|null, 
  index: number
): Robot|null {
  switch(CompanionRxUnion[type]) {
    case 'NONE': return null; 
    case 'Robot': return accessor(index, new Robot())! as Robot;
    default: return null;
  }
}
