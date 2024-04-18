import * as flatbuffers from 'flatbuffers';
import { UpdateBatteries, UpdateLowVoltageThreshold, UpdateCriticalVoltageThreshold, UpdateEnableRover, UpdateReferenceWheelAngle, UpdateWheelsFolded } from '../../generated/qocto/wsschema';

export const updateBatteries = (batteries: number) => {
    const builder = new flatbuffers.Builder(1);

    UpdateBatteries.startUpdateBatteries(builder);
    UpdateBatteries.addBatteries(builder, batteries);
    const updateBatteries = UpdateBatteries.endUpdateBatteries(builder);

    builder.finish(updateBatteries);

    return builder.asUint8Array();
}

export const updateLowVoltageThreshold = (lowVoltageThreshold: number) => {
    const builder = new flatbuffers.Builder(0);
    UpdateLowVoltageThreshold.startUpdateLowVoltageThreshold(builder);
    UpdateLowVoltageThreshold.addLowVoltageThreshold(builder, lowVoltageThreshold);
    const updateLowVoltageThreshold = UpdateLowVoltageThreshold.endUpdateLowVoltageThreshold(builder);
    builder.finish(updateLowVoltageThreshold);
    return builder.asUint8Array();
}

export const updateCriticalVoltageThreshold = (criticalVoltageThreshold: number) => {
    const builder = new flatbuffers.Builder(0);
    UpdateCriticalVoltageThreshold.startUpdateCriticalVoltageThreshold(builder);
    UpdateCriticalVoltageThreshold.addCriticalVoltageThreshold(builder, criticalVoltageThreshold);
    const updateCriticalVoltageThreshold = UpdateCriticalVoltageThreshold.endUpdateCriticalVoltageThreshold(builder);
    builder.finish(updateCriticalVoltageThreshold);
    return builder.asUint8Array();
}

export const updateReferenceWheelAngle = (referenceWheelAngle: number) => {
    const builder = new flatbuffers.Builder(0);
    UpdateReferenceWheelAngle.startUpdateReferenceWheelAngle(builder);
    UpdateReferenceWheelAngle.addReferenceWheelAngle(builder, referenceWheelAngle);
    const updateReferenceWheelAngle = UpdateReferenceWheelAngle.endUpdateReferenceWheelAngle(builder);
    builder.finish(updateReferenceWheelAngle);
    return builder.asUint8Array();
}

export const updateWheelsFolded = (wheelsFolded: boolean) => {
    const builder = new flatbuffers.Builder(0);
    UpdateWheelsFolded.startUpdateWheelsFolded(builder);
    UpdateWheelsFolded.addWheelsFolded(builder, wheelsFolded);
    const updateWheelsFolded = UpdateWheelsFolded.endUpdateWheelsFolded(builder);
    builder.finish(updateWheelsFolded);
    return builder.asUint8Array();
}

export const updateEnableRover = (enableRover: boolean) => {
    const builder = new flatbuffers.Builder(0);
    UpdateEnableRover.startUpdateEnableRover(builder);
    UpdateEnableRover.addEnableRover(builder, enableRover);
    const updateEnableRover = UpdateEnableRover.endUpdateEnableRover(builder);
    builder.finish(updateEnableRover);
    return builder.asUint8Array();
}
