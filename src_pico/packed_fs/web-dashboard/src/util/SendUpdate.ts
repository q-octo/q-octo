import { Update, UpdateBatteries, UpdateCriticalVoltageThreshold, UpdateEnableRover, UpdateFoldWheels, UpdateLinkQualityThreshold, UpdateLowVoltageThreshold, UpdateReferenceWheelAngle, UpdateRssiThreshold, UpdateUnion } from "../generated/fbs"
import * as flatbuffers from 'flatbuffers';

export const SendUpdate = (index: number, value: any) => {

    let builder = new flatbuffers.Builder(1024);

    let update;

    console.log('Update Index: ', index);

    switch(index) {
        case UpdateUnion.UpdateBatteries:
            UpdateBatteries.startUpdateBatteries(builder);
            UpdateBatteries.addBatteries(builder, value);

            const batteryUpdate = UpdateBatteries.endUpdateBatteries(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateBatteries);
            Update.addUpdate(builder, batteryUpdate);

            update = Update.endUpdate(builder);
            break;
        case UpdateUnion.UpdateLowVoltageThreshold:
            UpdateLowVoltageThreshold.startUpdateLowVoltageThreshold(builder);
            UpdateLowVoltageThreshold.addLowVoltageThreshold(builder, value);

            const lowVoltageUpdate = UpdateLowVoltageThreshold.endUpdateLowVoltageThreshold(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateLowVoltageThreshold);
            Update.addUpdate(builder, lowVoltageUpdate);

            update = Update.endUpdate(builder);
            break;
        case UpdateUnion.UpdateCriticalVoltageThreshold:
            UpdateCriticalVoltageThreshold.startUpdateCriticalVoltageThreshold(builder);
            UpdateCriticalVoltageThreshold.addCriticalVoltageThreshold(builder, value);

            const criticalVoltageUpdate = UpdateCriticalVoltageThreshold.endUpdateCriticalVoltageThreshold(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateCriticalVoltageThreshold);
            Update.addUpdate(builder, criticalVoltageUpdate);

            update = Update.endUpdate(builder);
            break;
        case UpdateUnion.UpdateReferenceWheelAngle:
            UpdateReferenceWheelAngle.startUpdateReferenceWheelAngle(builder);
            UpdateReferenceWheelAngle.addReferenceWheelAngle(builder, value);

            const referenceWheelAngleUpdate = UpdateReferenceWheelAngle.endUpdateReferenceWheelAngle(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateReferenceWheelAngle);
            Update.addUpdate(builder, referenceWheelAngleUpdate);

            update = Update.endUpdate(builder);
            break;
        case UpdateUnion.UpdateEnableRover:
            UpdateEnableRover.startUpdateEnableRover(builder);
            UpdateEnableRover.addEnableRover(builder, value);

            const enableRoverUpdate = UpdateEnableRover.endUpdateEnableRover(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateEnableRover);
            Update.addUpdate(builder, enableRoverUpdate);

            update = Update.endUpdate(builder);
            break;
        case UpdateUnion.UpdateLinkQualityThreshold:
            UpdateLinkQualityThreshold.startUpdateLinkQualityThreshold(builder);
            UpdateLinkQualityThreshold.addLinkQualityThreshold(builder, value);

            const linkQualityUpdate = UpdateLinkQualityThreshold.endUpdateLinkQualityThreshold(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateLinkQualityThreshold);
            Update.addUpdate(builder, linkQualityUpdate);

            update = Update.endUpdate(builder);
            break;
        case UpdateUnion.UpdateRssiThreshold:
            UpdateRssiThreshold.startUpdateRssiThreshold(builder);
            UpdateRssiThreshold.addRssiThreshold(builder, value);

            const rssiUpdate = UpdateRssiThreshold.endUpdateRssiThreshold(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateRssiThreshold);
            Update.addUpdate(builder, rssiUpdate);

            update = Update.endUpdate(builder);
            break;

        // TODO: Add folding wheels
        default:
            console.log('Invalid index');
            break;   
    }
        
    if (update != null){
        builder.finish(update);

        return builder.asUint8Array(); // Return buffer to send to server
    }

    return null;
        
}