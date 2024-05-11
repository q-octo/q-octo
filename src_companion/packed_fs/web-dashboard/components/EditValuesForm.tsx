import { useState } from "preact/hooks";
import { SendUpdate } from "../src/util/SendUpdate";
import ws from "../src/util/WebSocket";
import { UpdateUnion } from "../src/generated/fbs";
import { RoverState } from "../src/types";


type FieldParams = {
    field_id: number,
    label: string,
    onChange: (e: any) => void,
    handleSubmit: (fieldName: number, event: any) => void
    formValues: object,
    rover: RoverState
    max?: number,
    min?: number,
    step?: number
}
// A generic form field component
const FormField = ({ field_id, label, handleSubmit, onChange, formValues, rover, max, min, step }: FieldParams) => {

    // Decides what colour to render in the background of the input fields
    const formColour = (field_id: number) => {

        const colorMatch = "bg-green-100";
        const colorNoMatch = "bg-red-100";

        switch (field_id) {
            case UpdateUnion.UpdateBatteries:
                if (formValues[UpdateUnion.UpdateBatteries] == rover.batteries) return colorMatch;
                break;
            case UpdateUnion.UpdateLowVoltageThreshold:
                if (formValues[UpdateUnion.UpdateLowVoltageThreshold] == rover.low_voltage_threshold) return colorMatch;
                break;
            case UpdateUnion.UpdateCriticalVoltageThreshold:
                if (formValues[UpdateUnion.UpdateCriticalVoltageThreshold] == rover.critical_voltage_threshold) return colorMatch;
                break;
            case UpdateUnion.UpdateMaxTorque:
                if ((formValues[UpdateUnion.UpdateMaxTorque] == rover.motors.motor1.torque_limit) && (formValues[UpdateUnion.UpdateMaxTorque] == rover.motors.motor2.torque_limit)) {
                    return colorMatch;
                }
                break;
            case UpdateUnion.UpdateMaxSpeed:
                if ((formValues[UpdateUnion.UpdateMaxSpeed] == rover.motors.motor1.speed_limit) && (formValues[UpdateUnion.UpdateMaxSpeed] == rover.motors.motor2.speed_limit)) {
                    return colorMatch;
                }
                break;
            case UpdateUnion.UpdateMaxCurrent:
                if ((formValues[UpdateUnion.UpdateMaxCurrent] == rover.motors.motor1.current_limit) && (formValues[UpdateUnion.UpdateMaxCurrent] == rover.motors.motor2.current_limit)) {
                    return colorMatch;
                }
                break;
            case UpdateUnion.UpdateMotorKi:
                if ((formValues[UpdateUnion.UpdateMotorKi] == rover.motors.motor1.Ki) && (formValues[UpdateUnion.UpdateMotorKi] == rover.motors.motor2.Ki)) {
                    return colorMatch;
                }
                break;
            case UpdateUnion.UpdateMotorKp:
                if ((formValues[UpdateUnion.UpdateMotorKp] == rover.motors.motor1.Kp) && (formValues[UpdateUnion.UpdateMotorKp] == rover.motors.motor2.Kp)) {
                    return colorMatch;
                }
                break;
            case UpdateUnion.UpdateLowVoltageThreshold:
                if (formValues[UpdateUnion.UpdateLowVoltageThreshold] == rover.low_voltage_threshold) return colorMatch;
                break;
            case UpdateUnion.UpdateCriticalVoltageThreshold:
                if (formValues[UpdateUnion.UpdateCriticalVoltageThreshold] == rover.critical_voltage_threshold) return colorMatch;
                break;
            case UpdateUnion.UpdateLinkQualityThreshold:
                if (formValues[UpdateUnion.UpdateLinkQualityThreshold] == rover.link_quality_threshold) return colorMatch;
                break;
            case UpdateUnion.UpdateRssiThreshold:
                if (formValues[UpdateUnion.UpdateRssiThreshold] == rover.rssi_threshold) return colorMatch;
                break;
            case UpdateUnion.UpdateStartWebServerOnLaunch:
                if (formValues[UpdateUnion.UpdateStartWebServerOnLaunch] == rover.web_server_on_launch) return colorMatch;
                break;
            case UpdateUnion.UpdateCrsfLinkStatsTimeout:
                if (formValues[UpdateUnion.UpdateCrsfLinkStatsTimeout] == rover.radio_timeout) return colorMatch;
                break;

        }

        return colorNoMatch;
    }

    // Parameters for the input field (step, min, max)
    if (step == undefined) step = 0.1;

    let params;

    if (min == undefined || max == undefined) {
        params = { step: step };
    }
    else {
        params = { step: step, min: min, max: max };
    }

    return (
        <form onSubmit={(e) => handleSubmit(field_id, e)} className="mb-4">
            <label htmlFor={`${field_id}`} className="block text-sm font-medium text-gray-700">
                {label}
            </label>
            <div className="mt-1 flex rounded-md shadow-sm">
                <input type="number" id={`${field_id}`} name={`${field_id}`} className={`${formColour(field_id)} focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md`}
                    required
                    onChange={onChange}
                    {...params}
                    key={field_id}
                />
                <button type="submit" className="ml-2 bg-blue-500 text-white py-2 px-4 rounded-r-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-opacity-50">
                    Save
                </button>
            </div>
        </form>
    );
}

/**
 * Component for the input form.
 */
export const EditValuesForm = ({ rover }: { rover: RoverState }) => {

    // Default values for the form fields (initial state)
    const FieldDefaults = [
        { key: UpdateUnion.UpdateBatteries, value: rover.batteries },
        { key: UpdateUnion.UpdateLowVoltageThreshold, value: rover.low_voltage_threshold },
        { key: UpdateUnion.UpdateCriticalVoltageThreshold, value: rover.critical_voltage_threshold },
        { key: UpdateUnion.UpdateMaxTorque, value: rover.motors.motor1.torque_limit },
        { key: UpdateUnion.UpdateMaxSpeed, value: rover.motors.motor1.speed_limit },
        { key: UpdateUnion.UpdateMaxCurrent, value: rover.motors.motor1.current_limit },
        { key: UpdateUnion.UpdateMotorKi, value: rover.motors.motor1.Kp },
        { key: UpdateUnion.UpdateMotorKp, value: rover.motors.motor1.Ki },
        { key: UpdateUnion.UpdateLowVoltageThreshold, value: rover.low_voltage_threshold },
        { key: UpdateUnion.UpdateCriticalVoltageThreshold, value: rover.critical_voltage_threshold },
        { key: UpdateUnion.UpdateLinkQualityThreshold, value: rover.link_quality_threshold },
        { key: UpdateUnion.UpdateRssiThreshold, value: rover.rssi_threshold },
        { key: UpdateUnion.UpdateStartWebServerOnLaunch, value: rover.web_server_on_launch },
        { key: UpdateUnion.UpdateCrsfLinkStatsTimeout, value: rover.radio_timeout },
    ]

    let key_value = {}
    FieldDefaults.forEach((field) => {
        key_value[field.key] = field.value;
    })


    // Holds state for indicating the colour of the input fields
    const [formValues, setFormValues] = useState(key_value);

    // Function to handle form submission for each field
    const handleSubmit = (fieldName, event) => {
        event.preventDefault();

        // Loop through the form data to get the values
        const formData = new FormData(event.target);
        const data = {};
        for (let [key, value] of formData.entries()) {
            data[key] = parseFloat(value);
            console.log(key, value);
        }

        // Send the update
        ws.send(SendUpdate(fieldName, data[fieldName]));
    };


    const handleChange = (e) => {
        const { name, value } = e.target;
        setFormValues({ ...formValues, [name]: value });
    }

    return (
        <div className="bg-slate-200 p-4 sm:p-6 max-w-lg mx-auto rounded-lg shadow-md">
            <h2 className="text-xl sm:text-2xl font-bold mb-4">Edit Values</h2>
            <h3 className="text-lg sm:text-xl font-bold mb-4">❗ Motor Limits</h3>
            <FormField field_id={UpdateUnion.UpdateMaxTorque} label="Torque Limit"  onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />
            <FormField field_id={UpdateUnion.UpdateMaxSpeed} label="Speed Limit" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />
            <FormField field_id={UpdateUnion.UpdateMaxCurrent} label="Current Limit" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />

            <h3 className="text-lg sm:text-xl font-bold mb-4">⚡ Voltage Thresholds</h3>
            <FormField field_id={UpdateUnion.UpdateLowVoltageThreshold} label="Low Voltage Threshold" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />
            <FormField field_id={UpdateUnion.UpdateCriticalVoltageThreshold} label="Critical Voltage Threshold" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />

            <h3 className="text-lg sm:text-xl font-bold mb-4">📻 Radio Settings</h3>
            <FormField field_id={UpdateUnion.UpdateRssiThreshold} label="RSSI Threshold" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />
            <FormField field_id={UpdateUnion.UpdateLinkQualityThreshold} label="Link Quality Threshold" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />
            <FormField field_id={UpdateUnion.UpdateCrsfLinkStatsTimeout} label="CRSF Link Stats Timeout" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />

            <h3 className="text-lg sm:text-xl font-bold mb-4">🔧 Motor PID Constants</h3>
            <FormField field_id={UpdateUnion.UpdateMotorKi} label="Motor Ki" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />
            <FormField field_id={UpdateUnion.UpdateMotorKp} label="Motor Kp" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />

            <h3 className="text-lg sm:text-xl font-bold mb-4">⚙ Other Settings</h3>
            <FormField field_id={UpdateUnion.UpdateBatteries} label="Number of batteries" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />

        </div>
    );
};