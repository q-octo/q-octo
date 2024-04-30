// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import { CrsfFrame, CrsfFrameT } from '../fbs/crsf-frame.js';
import { DisplayMessages, DisplayMessagesT } from '../fbs/display-messages.js';
import { DriveRobot, DriveRobotT } from '../fbs/drive-robot.js';


export enum OnboardComputerTxUnion {
  NONE = 0,

  /**
   * A frame to be sent to the transmitter.
   */
  CrsfFrame = 1,
  DriveRobot = 2,
  DisplayMessages = 3
}

export function unionToOnboardComputerTxUnion(
  type: OnboardComputerTxUnion,
  accessor: (obj:CrsfFrame|DisplayMessages|DriveRobot) => CrsfFrame|DisplayMessages|DriveRobot|null
): CrsfFrame|DisplayMessages|DriveRobot|null {
  switch(OnboardComputerTxUnion[type]) {
    case 'NONE': return null; 
    case 'CrsfFrame': return accessor(new CrsfFrame())! as CrsfFrame;
    case 'DriveRobot': return accessor(new DriveRobot())! as DriveRobot;
    case 'DisplayMessages': return accessor(new DisplayMessages())! as DisplayMessages;
    default: return null;
  }
}

export function unionListToOnboardComputerTxUnion(
  type: OnboardComputerTxUnion, 
  accessor: (index: number, obj:CrsfFrame|DisplayMessages|DriveRobot) => CrsfFrame|DisplayMessages|DriveRobot|null, 
  index: number
): CrsfFrame|DisplayMessages|DriveRobot|null {
  switch(OnboardComputerTxUnion[type]) {
    case 'NONE': return null; 
    case 'CrsfFrame': return accessor(index, new CrsfFrame())! as CrsfFrame;
    case 'DriveRobot': return accessor(index, new DriveRobot())! as DriveRobot;
    case 'DisplayMessages': return accessor(index, new DisplayMessages())! as DisplayMessages;
    default: return null;
  }
}
