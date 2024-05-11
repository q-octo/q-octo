import { useState } from "preact/hooks";
import { SendUpdate } from "../src/util/SendUpdate";
import ws from "../src/util/WebSocket";
import { UpdateUnion } from "../src/generated/fbs";
import { RoverState } from "../src/types";

enum Field {
    Bats = "bats",
    LowV = "lowV",
    CritV = "critV",
    TorqueLimit = "torqueLimit",
    SpeedLimit = "speedLimit",
    CurrentLimit = "currentLimit",
    Kp = "Kp",
    Ki = "Ki",
    RSSIThres = "RSSIThres",
    LinkQualityThres = "LinkQualityThres",
    ShutdownVoltage = "ShutdownVoltage",
    CriticalVoltage = "CriticalVoltage",
    RadioTimeout = "RadioTimeout",
    WebServerOnLaunch = "WebServerOnLaunch",
}

/**
 * Component for the input form.
 */
export const EditValuesForm = ({ rover }: { rover: RoverState }) => {

    type FieldParams = {
        field_id: string,
        label: string,
        max?: number,
        min?: number,
        step?: number
    }

    // A single form field
    const FormField = ({field_id,label,max,min,step} :  FieldParams) => {


        // Parameters for the input field (step, min, max)
        if (step == undefined) step = 0.1;
        
        let params;

        if (min == undefined || max == undefined) {
            params = {step: step};
        }
        else {
            params = {step: step, min: min, max: max};
        }

        return (
            <form onSubmit={(e) => handleSubmit(field_id, e)} className="mb-4">
                <label htmlFor={`${field_id}`} className="block text-sm font-medium text-gray-700">
                    {label}
                </label>
                <div className="mt-1 flex rounded-md shadow-sm">
                    <input type="number" id={`${field_id}`} name={`${field_id}`} className={`${formColour(field_id)} focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md`} 
                        required
                        onChange={handleChange}
                        {...params}
                    />
                    <button type="submit" className="ml-2 bg-blue-500 text-white py-2 px-4 rounded-r-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-opacity-50">
                        Save
                    </button>
                </div>
            </form>
        );
    }
    
    // Holds state for indicating the colour of the input fields
    const [formValues, setFormValues] = useState({
        1: 0,
        2: 0,
        3: 0,
        4: 0
    });

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

	// Decides what colour to render in the background of the input fields
    // TODO: Update with new field names
	const formColour = (field_id: number) => {


        const colorMatch = "bg-green-100";
        const colorNoMatch = "bg-red-100";

		console.log(formValues);
		console.log(rover);

		switch (field_id) {
			case UpdateUnion.UpdateBatteries:
				if (formValues[1] == rover.batteries) return colorMatch;
				break;
			case UpdateUnion.UpdateLowVoltageThreshold:
				if (formValues[2] == rover.low_voltage_threshold) return colorMatch;
				break;
			case UpdateUnion.UpdateCriticalVoltageThreshold:
				if (formValues[3] == rover.critical_voltage_threshold) return colorMatch;
				break;
			case UpdateUnion.UpdateMaxTorque:
				if ((formValues[4] == rover.motors.motor1.torque_limit) && (formValues[4] == rover.motors.motor2.torque_limit)) {
					return colorMatch;
				}
		}

		return colorNoMatch;
	}

	return (
		<div className="bg-slate-200 p-4 sm:p-6 max-w-lg mx-auto rounded-lg shadow-md">
			<h2 className="text-xl sm:text-2xl font-bold mb-4">Edit Values</h2>

			{/* Number of batteries */}
			<form onSubmit={(e) => handleSubmit(UpdateUnion.UpdateBatteries, e)} className="mb-4">
				<label htmlFor={`${UpdateUnion.UpdateBatteries}`} className="block text-sm font-medium text-gray-700">
					Number of batteries
				</label>
				<div className="mt-1 flex rounded-md shadow-sm">
					<input type="number" id={`${UpdateUnion.UpdateBatteries}`} name={`${UpdateUnion.UpdateBatteries}`} className={`${formColour(Field.Bats)} focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md`} required
						onChange={handleChange}
					/>
					<button type="submit" className="ml-2 bg-blue-500 text-white py-2 px-4 rounded-r-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-opacity-50">
						Save
					</button>
				</div>
			</form>

			{/* Low Voltage Threshold */}
			<form onSubmit={(e) => handleSubmit(UpdateUnion.UpdateLowVoltageThreshold, e)} className="mb-4">
				<label htmlFor={`${UpdateUnion.UpdateLowVoltageThreshold}`} className="block text-sm font-medium text-gray-700">
					Low Voltage threshold
				</label>
				<div className="mt-1 flex rounded-md shadow-sm">
					<input type="number" step="0.1" id={`${UpdateUnion.UpdateLowVoltageThreshold}`} name={`${UpdateUnion.UpdateLowVoltageThreshold}`} className={`${formColour(Field.LowV)} focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md`} required
						onChange={handleChange}
					/>
					<button type="submit" className="ml-2 bg-blue-500 text-white py-2 px-4 rounded-r-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-opacity-50">
						Save
					</button>
				</div>
			</form>

			{/* Critical Voltage Threshold */}
			<form onSubmit={(e) => handleSubmit(UpdateUnion.UpdateCriticalVoltageThreshold, e)} className="mb-4">
				<label htmlFor={`${UpdateUnion.UpdateCriticalVoltageThreshold}`} className="block text-sm font-medium text-gray-700">
					Critical Voltage Threshold
				</label>
				<div className="mt-1 flex rounded-md shadow-sm">
					<input type="number" step="0.1" id={`${UpdateUnion.UpdateCriticalVoltageThreshold}`} name={`${UpdateUnion.UpdateCriticalVoltageThreshold}`} className={`${formColour(Field.CritV)} focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md`} required
						onChange={handleChange}
					/>
					<button type="submit" className="ml-2 bg-blue-500 text-white py-2 px-4 rounded-r-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-opacity-50">
						Save
					</button>
				</div>
			</form>


			{/* Torque Limit */}
			<form onSubmit={(e) => handleSubmit(UpdateUnion.UpdateMaxTorque, e)} className="mb-4">
				<label htmlFor="torqueLimit" className="block text-sm font-medium text-gray-700">
					Torque Limit Left
				</label>
				<div className="mt-1 flex rounded-md shadow-sm">
					<input type="number" step="0.1" id={`${UpdateUnion.UpdateMaxTorque}`} name={`${UpdateUnion.UpdateMaxTorque}`} className={`${formColour(Field.TorqueLimit)} focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md" required`}
						onChange={handleChange}
					/>
					<button type="submit" className="ml-2 bg-blue-500 text-white py-2 px-4 rounded-r-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-opacity-50">
						Save
					</button>
				</div>
			</form>
		</div>
	);
};