import { Update, UpdateBatteries, UpdateCriticalVoltageThreshold, UpdateCrsfLinkStatsTimeout, UpdateEnableRover, UpdateFoldWheels, UpdateLinkQualityThreshold, UpdateLowVoltageThreshold, UpdateMaxCurrent, UpdateMaxSpeed, UpdateMaxTorque, UpdateMotorKi, UpdateMotorKp, UpdateReferenceWheelAngle, UpdateRssiThreshold, UpdateStartWebServerOnLaunch, UpdateUnion } from "../generated/fbs"
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

        case UpdateUnion.UpdateMaxTorque:
            UpdateMaxTorque.startUpdateMaxTorque(builder);
            UpdateMaxTorque.addMaxTorque(builder, value);

            const maxTorqueUpdate = UpdateMaxTorque.endUpdateMaxTorque(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateMaxTorque);
            Update.addUpdate(builder, maxTorqueUpdate);

            update = Update.endUpdate(builder);
            break;
        case UpdateUnion.UpdateMaxSpeed:
            UpdateMaxSpeed.startUpdateMaxSpeed(builder);
            UpdateMaxSpeed.addMaxSpeed(builder, value);

            const maxSpeedUpdate = UpdateMaxSpeed.endUpdateMaxSpeed(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateMaxSpeed);
            Update.addUpdate(builder, maxSpeedUpdate);

            update = Update.endUpdate(builder);
            break;
        case UpdateUnion.UpdateMaxCurrent:
            UpdateMaxCurrent.startUpdateMaxCurrent(builder);
            UpdateMaxCurrent.addMaxCurrent(builder, value);

            const maxCurrentUpdate = UpdateMaxCurrent.endUpdateMaxCurrent(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateMaxCurrent);
            Update.addUpdate(builder, maxCurrentUpdate);

            update = Update.endUpdate(builder);
            break;

        case UpdateUnion.UpdateMotorKp:
            UpdateMotorKp.startUpdateMotorKp(builder);
            UpdateMotorKp.addMotorKp(builder, value);

            const motorKpUpdate = UpdateMotorKp.endUpdateMotorKp(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateMotorKp);
            Update.addUpdate(builder, motorKpUpdate);

            update = Update.endUpdate(builder);
            break;
        case UpdateUnion.UpdateMotorKi:
            UpdateMotorKi.startUpdateMotorKi(builder);
            UpdateMotorKi.addMotorKi(builder, value);

            const motorKiUpdate = UpdateMotorKi.endUpdateMotorKi(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateMotorKi);
            Update.addUpdate(builder, motorKiUpdate);

            update = Update.endUpdate(builder);
            break;
        case UpdateUnion.UpdateCrsfLinkStatsTimeout:
            UpdateCrsfLinkStatsTimeout.startUpdateCrsfLinkStatsTimeout(builder);
            UpdateCrsfLinkStatsTimeout.addTimeoutMillis(builder, value);

            const crsfLinkStatsTimeoutUpdate = UpdateCrsfLinkStatsTimeout.endUpdateCrsfLinkStatsTimeout(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateCrsfLinkStatsTimeout);
            Update.addUpdate(builder, crsfLinkStatsTimeoutUpdate);

            update = Update.endUpdate(builder);
            break;
        case UpdateUnion.UpdateStartWebServerOnLaunch:
            UpdateStartWebServerOnLaunch.startUpdateStartWebServerOnLaunch(builder);
            UpdateStartWebServerOnLaunch.addStartWebServerOnLaunch(builder, value);

            console.log('Start Web Server on Launch: ', value);

            const startWebServerOnLaunchUpdate = UpdateStartWebServerOnLaunch.endUpdateStartWebServerOnLaunch(builder);

            Update.startUpdate(builder);
            Update.addUpdateType(builder, UpdateUnion.UpdateStartWebServerOnLaunch);
            Update.addUpdate(builder, startWebServerOnLaunchUpdate);

            update = Update.endUpdate(builder);
            break;
        
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