import * as flatbuffers from 'flatbuffers';
import { render } from 'preact';
import { useState } from 'preact/hooks';
import { UpdateUnion } from './generated/fbs';
import { Robot } from './generated/fbs/robot';
import { RoverState } from './types';
import { RobotToObj } from './util/RobotToObj';
import { SendUpdate } from './util/SendUpdate';
import { Round, Round4 } from './util/Round';
import ws from './util/WebSocket';
import './style.css';

const Motor = ({ motor, name }) => {

	return (
		<div className="bg-gray-100 p-6 rounded-lg shadow-sm mb-4 md:mb-0 flex-1 md:mr-2">
			<h3 className="text-lg font-bold mb-2">{`Motor ${name}`}</h3>
			<p className="mb-1">Temp: <span className="font-medium">{Round(motor.temperature)} °C</span></p>
			<p className="mb-1">RPM: <span className="font-medium">{Round(motor.rps)} rad/s</span></p>
			<p className="mb-1">Angle: <span className="font-medium">{Round(motor.angle)} °</span></p>
			<p>Kp: <span className="font-medium">{Round(motor.Kp)}</span></p>
			<p>Ki: <span className="font-medium">{Round4(motor.Kp)}</span></p>
			<p>Torque: <span className="font-medium">{Round(motor.torque)} Nm</span></p>
			<p><b>Max Torque:</b> <span className="font-medium">{Round(motor.torque_limit)} Nm</span></p>
			<p><b>Max Speed:</b> <span className="font-medium">{Round(motor.speed_limit)}</span></p>
			<p><b>Max Current:</b> <span className="font-medium">{Round(motor.current_limit)} A</span></p>
		</div>
	);
}

/**
 * This Component is where the current values of the rover are displayed
 */
const Dashboard = ({ rover }: { rover: RoverState }) => {

	console.log(rover);

	return (
		<div className="bg-white p-4 sm:p-6 max-w-lg mx-auto rounded-lg shadow-md mb-8">
			<h2 className="text-xl sm:text-2xl font-bold mb-4">Current Values</h2>
			<div className="md:flex md:space-x-4">
				{/* Motor 1 Card */}
				<Motor motor={rover.motors.motor1} name="1" />

				{/* Motor 2 Card */}
				<Motor motor={rover.motors.motor2} name="2" />
			</div>


			<div className="mt-4 p-6 space-y-2">

				<div class="flex flex-col sm:flex-row justify-between space-y-2 sm:space-y-0">
					<p class="text-base sm:text-sm">Number of Batteries 🔋 : <span class="font-medium">{rover.batteries}</span></p>
					<p class="text-base sm:text-sm">Fuel ⛽ : <span class="font-medium">{Round(rover.fuel)}%</span></p>
				</div>

				<div class="flex flex-col sm:flex-row justify-between space-y-2 sm:space-y-0">
					<p class="text-base sm:text-sm">Status ❓ : <span class="font-medium">{rover.status}</span></p>
					<p class="text-base sm:text-sm">Control Source 🎮 : <span class="font-medium">{rover.control_source}</span></p>
				</div>


				<div class="flex flex-col sm:flex-row justify-between space-y-2 sm:space-y-0">
					<p class="text-base sm:text-sm">Voltage ⚡ : <span class="font-medium">{Round(rover.voltage)} V</span></p>
					<p class="text-base sm:text-sm">Current 🅰 : <span class="font-medium">{Round(rover.current)} A</span></p>
				</div>

				<p class="text-base sm:text-sm">Dashboard On Launch 🌐 : <span class="font-medium">{rover.web_server_on_launch ? "True" : "False"}</span></p>

				{/*Vertical spacer*/}
				<div class="h-4"></div>


				<div class="flex flex-col sm:flex-row justify-between space-y-2 sm:space-y-0">
					<p class="text-base sm:text-sm">RSSI 📶: <span class="font-medium">{Round(rover.rssi)} dBm</span></p>
					<p class="text-base sm:text-sm">Thres. : <span class="font-medium">{Round(rover.rssi_threshold)} dBm</span></p>
				</div>

				<div class="flex flex-col sm:flex-row justify-between space-y-2 sm:space-y-0">
					<p class="text-base sm:text-sm">Link 📻: <span class="font-medium">{Round(rover.link_quality)} %</span></p>
					<p class="text-base sm:text-sm">Thres : <span class="font-medium">{Round(rover.link_quality_threshold)} %</span></p>
				</div>

				<p class="text-base sm:text-sm">Radio Timeout ⌛ : <span class="font-medium">{rover.radio_timeout}ms</span></p>

				{/*Vertical spacer*/}
				<div class="h-4"></div>

				<div class="flex flex-col sm:flex-row justify-between space-y-2 sm:space-y-0">
					<p class="text-base sm:text-sm">⚠⚙Motor Shut : <span class="font-medium">{Round(rover.low_voltage_threshold)} V</span></p>
					<p class="text-base sm:text-sm">🟥System Shut : <span class="font-medium">{Round(rover.critical_voltage_threshold)} V</span></p>
				</div>



			</div>

		</div>
	);
};

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

	const isMatch = (field_id: number) => {
		switch (field_id) {
			case UpdateUnion.UpdateBatteries:
				if (formValues[UpdateUnion.UpdateBatteries] == rover.batteries) return true;
				break;
			case UpdateUnion.UpdateLowVoltageThreshold:
				if (formValues[UpdateUnion.UpdateLowVoltageThreshold] == rover.low_voltage_threshold) return true;
				break;
			case UpdateUnion.UpdateCriticalVoltageThreshold:
				if (formValues[UpdateUnion.UpdateCriticalVoltageThreshold] == rover.critical_voltage_threshold) return true;
				break;
			case UpdateUnion.UpdateMaxTorque:
				if ((formValues[UpdateUnion.UpdateMaxTorque] == rover.motors.motor1.torque_limit) && (formValues[UpdateUnion.UpdateMaxTorque] == rover.motors.motor2.torque_limit)) {
					return true;
				}
				break;
			case UpdateUnion.UpdateMaxSpeed:
				if ((formValues[UpdateUnion.UpdateMaxSpeed] == rover.motors.motor1.speed_limit) && (formValues[UpdateUnion.UpdateMaxSpeed] == rover.motors.motor2.speed_limit)) {
					return true;
				}
				break;
			case UpdateUnion.UpdateMaxCurrent:
				if ((formValues[UpdateUnion.UpdateMaxCurrent] == rover.motors.motor1.current_limit) && (formValues[UpdateUnion.UpdateMaxCurrent] == rover.motors.motor2.current_limit)) {
					return true;
				}
				break;
			case UpdateUnion.UpdateMotorKi:
				if ((formValues[UpdateUnion.UpdateMotorKi] == rover.motors.motor1.Ki) && (formValues[UpdateUnion.UpdateMotorKi] == rover.motors.motor2.Ki)) {
					return true;
				}
				break;
			case UpdateUnion.UpdateMotorKp:
				if ((formValues[UpdateUnion.UpdateMotorKp] == rover.motors.motor1.Kp) && (formValues[UpdateUnion.UpdateMotorKp] == rover.motors.motor2.Kp)) {
					return true;
				}
				break;
			case UpdateUnion.UpdateLowVoltageThreshold:
				if (formValues[UpdateUnion.UpdateLowVoltageThreshold] == rover.low_voltage_threshold) return true;
				break;
			case UpdateUnion.UpdateCriticalVoltageThreshold:
				if (formValues[UpdateUnion.UpdateCriticalVoltageThreshold] == rover.critical_voltage_threshold) return true;
				break;
			case UpdateUnion.UpdateLinkQualityThreshold:
				if (formValues[UpdateUnion.UpdateLinkQualityThreshold] == rover.link_quality_threshold) return true;
				break;
			case UpdateUnion.UpdateRssiThreshold:
				if (formValues[UpdateUnion.UpdateRssiThreshold] == rover.rssi_threshold) return true;
				break;
			case UpdateUnion.UpdateStartWebServerOnLaunch:
				if (formValues[UpdateUnion.UpdateStartWebServerOnLaunch] == rover.web_server_on_launch) return true;
				break;
			case UpdateUnion.UpdateCrsfLinkStatsTimeout:
				if (formValues[UpdateUnion.UpdateCrsfLinkStatsTimeout] == rover.radio_timeout) return true;
				break;

		}

		return false;
	}

	// Decides what colour to render in the background of the input fields
	const formColour = (field_id: number) => {

		const colorMatch = "bg-green-100";
		const colorNoMatch = "bg-red-100";

		return isMatch(field_id) ? colorMatch : colorNoMatch;
	}


	// Parameters for the input field (step, min, max)
	if (step == undefined) step = 0.1;
	if (min == undefined) min = 0;
	if (max == undefined) max = 100;

	return (
		<form onSubmit={(e) => handleSubmit(field_id, e)} className="mb-4">
			<label htmlFor={`${field_id}`} className="block text-sm font-medium text-gray-700">
				{label}
			</label>
			<div className="mt-1 flex rounded-md shadow-sm">
				<input type="number" id={`${field_id}`} name={`${field_id}`} className={`${formColour(field_id)} focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md`}
					required
					onChange={onChange}
					key={field_id}
					max={max}
					min={min}
					step={step}
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
const EditValuesForm = ({ rover }: { rover: RoverState }) => {

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
			<FormField field_id={UpdateUnion.UpdateMaxTorque} label="Torque Limit" step={0.01} onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />
			<FormField field_id={UpdateUnion.UpdateMaxSpeed} label="Speed Limit" step={0.01} min={0} max={30} onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />
			<FormField field_id={UpdateUnion.UpdateMaxCurrent} label="Current Limit" step={0.01} min={0} max={27} onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />

			<h3 className="text-lg sm:text-xl font-bold mb-4">⚡ Voltage Thresholds</h3>
			<FormField field_id={UpdateUnion.UpdateLowVoltageThreshold} label="Low Voltage Threshold" step={0.01} onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />
			<FormField field_id={UpdateUnion.UpdateCriticalVoltageThreshold} label="Critical Voltage Threshold" step={0.01} onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />

			<h3 className="text-lg sm:text-xl font-bold mb-4">📻 Radio Settings</h3>
			<FormField field_id={UpdateUnion.UpdateRssiThreshold} min={0} max={130} label="RSSI Threshold" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />
			<FormField field_id={UpdateUnion.UpdateLinkQualityThreshold} min={0} max={100} label="Link Quality Threshold" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />
			<FormField field_id={UpdateUnion.UpdateCrsfLinkStatsTimeout} min={0} max={10000} label="CRSF Link Stats Timeout" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />

			<h3 className="text-lg sm:text-xl font-bold mb-4">🔧 Motor PID Constants</h3>
			<FormField field_id={UpdateUnion.UpdateMotorKi} label="Motor Ki" step={0.001} onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />
			<FormField field_id={UpdateUnion.UpdateMotorKp} label="Motor Kp" step={0.01} onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />

			<h3 className="text-lg sm:text-xl font-bold mb-4">⚙ Other Settings</h3>
			<FormField field_id={UpdateUnion.UpdateBatteries} label="Number of batteries" onChange={handleChange} handleSubmit={handleSubmit} formValues={formValues} rover={rover} />

			{/* The WiFi launch enable is just a toggle so we'll use a button*/}
			<label className="block text-sm font-medium text-gray-700">
				Launch Web Dashboard on Startup
			</label>

			<button label={formValues[UpdateUnion.UpdateStartWebServerOnLaunch] ? "Disable" : "Enable"} onClick={(e) => {
				ws.send(SendUpdate(UpdateUnion.UpdateStartWebServerOnLaunch, !rover.web_server_on_launch));
			}
			} className={`w-full ${rover.web_server_on_launch ? "bg-green-500" : "bg-red-500"} text-white py-2 px-4 rounded-md hover:${rover.web_server_on_launch ? "bg-green-100" : "bg-red-100"} focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-opacity-50`}>
				Enabled = {rover.web_server_on_launch ? "True" : "False"}
			</button>
			

		</div>
	);
};


/**
 * Root component of the application.
 */
export function App() {

	// These defaults are shown until the first message is received from the server
	const [rover, setRover] = useState<RoverState>({
		motors: {
			motor1: {
				temperature: -1,
				rps: -1,
				angle: -1,
				torque: -1,
				torque_limit: -1,
				speed_limit: -1,
				current_limit: -1,
				Kp: -1,
				Ki: -1
			},
			motor2: {
				temperature: -1,
				rps: -1,
				angle: -1,
				torque: -1,
				torque_limit: -1,
				speed_limit: -1,
				current_limit: -1,
				Kp: -1,
				Ki: -1
			}
		},
		batteries: -1,
		control_source: "RC",
		status: "NO_TX",
		voltage: -1,
		current: -1,
		rssi: -1,
		rssi_threshold: -1,
		link_quality_threshold: -1,
		link_quality: -1,
		low_voltage_threshold: -1,
		critical_voltage_threshold: -1,
		motor_error_code: "0xFF",
		radio_timeout: -1,
		fuel: -1,
		web_server_on_launch: false
	});

	// WebSocket client
	// Just a typical listener callback pattern.
	const wsClient = ws;
	ws.onmessage = (event) => {
		console.log(event)

		// From blob to array buffer
		try {
			const reader = new FileReader();
			reader.readAsArrayBuffer(event.data);
			reader.onload = () => {
				const buf = new flatbuffers.ByteBuffer(new Uint8Array(reader.result as ArrayBuffer));
				const robot = Robot.getRootAsRobot(buf);
				console.log(RobotToObj(robot));

				// Update the state
				setRover(RobotToObj(robot));

			}
		} catch (error) {
			console.log("Invalid data received from server");
		}
	}


	return (
		<main className="space-y-8">
			<Dashboard rover={rover} />
			<EditValuesForm rover={rover} />
		</main>
	);
}
render(<App />, document.getElementById('app'));