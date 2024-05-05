import { render } from 'preact';
import { useEffect, useState } from 'preact/hooks';
import { RoverState } from './types';
import './style.css';
import ws from './util/WebSocket';
import { updateBatteries } from './util/Updates';
import { RobotToObj } from './util/RobotToObj';
import { Robot } from './generated/fbs/robot';
import * as flatbuffers from 'flatbuffers';
import { Update, UpdateUnion } from './generated/fbs';
import { SendUpdate } from './util/SendUpdate';

const Dashboard = ({ rover }: { rover: RoverState }) => {

	console.log(rover);

	return (
		<div className="bg-white p-4 sm:p-6 max-w-lg mx-auto rounded-lg shadow-md mb-8">
			<h2 className="text-xl sm:text-2xl font-bold mb-4">Current Values</h2>
			<div className="md:flex md:space-x-4">
				{/* Motor 1 Card */}
				<div className="bg-gray-100 p-6 rounded-lg shadow-sm mb-4 md:mb-0 flex-1 md:mr-2">
					<h3 className="text-lg font-bold mb-2">Motor 1</h3>
					<p className="mb-1">Temp: <span className="font-medium">{rover.motors.motor1.temperature}°C</span></p>
					<p className="mb-1">RPM: <span className="font-medium">{rover.motors.motor1.rps} RPM</span></p>
					<p className="mb-1">Angle: <span className="font-medium">{rover.motors.motor1.angle} °</span></p>
					<p>Torque: <span className="font-medium">{rover.motors.motor1.torque}Nm</span></p>
				</div>

				{/* Motor 2 Card */}
				<div className="bg-gray-100 p-6 rounded-lg shadow-sm flex-1">
					<h3 className="text-lg font-bold mb-2">Motor 2</h3>
					<p className="mb-1">Temp: <span className="font-medium">{rover.motors.motor2.temperature} °C</span></p>
					<p className="mb-1">RPM: <span className="font-medium">{rover.motors.motor2.rps} RPM</span></p>
					<p className="mb-1">Angle: <span className="font-medium">{rover.motors.motor2.angle} °</span></p>
					<p>Torque: <span className="font-medium">{rover.motors.motor1.torque}Nm</span></p>
				</div>
			</div>


			<div className="mt-4 p-6 space-y-2">
				<p class="text-base sm:text-sm">Number of Batteries 🔋 : <span class="font-medium">{rover.batteries}</span></p>
				<p class="text-base sm:text-sm">Status ❓ : <span class="font-medium">{rover.status}</span></p>
				<p class="text-base sm:text-sm">Control Source 🎮 : <span class="font-medium">{rover.control_source}</span></p>
				<p class="text-base sm:text-sm">Fuel ⛽ : <span class="font-medium">{rover.fuel}%</span></p>

				<div class="flex flex-col sm:flex-row justify-between space-y-2 sm:space-y-0">
					<p class="text-base sm:text-sm">Voltage ⚡ : <span class="font-medium">{rover.voltage} V</span></p>
					<p class="text-base sm:text-sm">Current 🅰 : <span class="font-medium">{rover.current} A</span></p>
				</div>


				<div class="flex flex-col sm:flex-row justify-between space-y-2 sm:space-y-0">
					<p class="text-base sm:text-sm">RSSI 📶: <span class="font-medium">{rover.rssi} dBm</span></p>
					<p class="text-base sm:text-sm">Link 📻: <span class="font-medium">{rover.linkQualityThreshold} %</span></p>
				</div>

				{/* 
				<div class="flex flex-col sm:flex-row justify-between space-y-2 sm:space-y-0">
					<p class="text-base sm:text-sm">Offset L: <span class="font-medium">{rover.offset.left} °</span></p>
					<p class="text-base sm:text-sm">Offset R: <span class="font-medium">{rover.offset.right} °</span></p>
				</div> */}


			</div>

		</div>
	);
};

const EditValuesForm = ({ rover }: { rover: RoverState }) => {
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

	const [formValues, setFormValues] = useState({
		1: 0,
		2: 0,
		3: 0,
	});

	const handleChange = (e) => {
		const { name, value } = e.target;
		setFormValues({ ...formValues, [name]: value });
	}

	// Green if value is equal to the current value, red if not
	const formColour = (fieldname: string) => {

		console.log(formValues);
		console.log(rover);

		switch (fieldname) {
			case 'bats':
				if (formValues[1] == rover.batteries) return "bg-green-100";
				break;
			case 'lowV':
				if (formValues[2]  == rover.low_voltage_threshold) return "bg-green-100";
				break;
			case 'critV':
				if (formValues[3]  == rover.critical_voltage_threshold) return "bg-green-100";
				break;
		}
		
		return "bg-red-100";
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
					<input type="number" id={`${UpdateUnion.UpdateBatteries}`} name={`${UpdateUnion.UpdateBatteries}`} className={`${formColour('bats')} focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md`} required
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
					<input type="number" step="0.1" id={`${UpdateUnion.UpdateLowVoltageThreshold}`} name={`${UpdateUnion.UpdateLowVoltageThreshold}`} className={`${formColour('lowV')} focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md`} required
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
					<input type="number" step="0.1" id={`${UpdateUnion.UpdateCriticalVoltageThreshold}`} name={`${UpdateUnion.UpdateCriticalVoltageThreshold}`} className={`${formColour('critV')} focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md`} required
						onChange={handleChange}
					/>
					<button type="submit" className="ml-2 bg-blue-500 text-white py-2 px-4 rounded-r-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-opacity-50">
						Save
					</button>
				</div>
			</form>

			{/* Reference Wheel Angle */}
			{/* <form onSubmit={(e) => handleSubmit(UpdateUnion.UpdateReferenceWheelAngle, e)} className="mb-4">
				<label htmlFor="referenceWheelAngle" className="block text-sm font-medium text-gray-700">
					Reference Wheel Angle
				</label>
				<div className="mt-1 flex rounded-md shadow-sm">
					<input type="number" step="0.2" id={`${UpdateUnion.UpdateReferenceWheelAngle}`} name={`${UpdateUnion.UpdateReferenceWheelAngle}`} className={`${formColour('refWheel')} focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md" required`}
						onChange={handleChange}
					/>
					<button type="submit" className="ml-2 bg-blue-500 text-white py-2 px-4 rounded-r-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-opacity-50">
						Save
					</button>
				</div>
			</form> */}


		</div>
	);
};


export function App() {

	const [rover, setRover] = useState<RoverState>({
		motors: {
			motor1: {
				temperature: 22,
				rps: 40,
				angle: 3,
				torque: 1.5
			},
			motor2: {
				temperature: 24,
				rps: 50,
				angle: 3,
				torque: 1.5
			}
		},
		batteries: 4,
		control_source: "RC",
		status: "NOT_X",
		voltage: 12,
		current: 2,
		rssi: -50,
		linkQualityThreshold: -50,
		max_speed: 5,
		low_voltage_threshold: 10,
		critical_voltage_threshold: 5,
		reference_wheel_angle: 0,
		motor_error_code: "0xFF",
		wheels_folded: false,
		fuel: 50,

	});

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
