import { render } from 'preact';
import { useEffect, useState } from 'preact/hooks';
import { RoverState } from './types';
import './style.css';
import ws from './util/WebSocket';
import { updateBatteries } from './util/Updates';


const Dashboard = ({rover} : {rover: RoverState}) => {

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
				<p class="text-base sm:text-sm">Number of Batteries 🔋 : <span class="font-medium">{rover.batteries.number}</span></p>
				<p class="text-base sm:text-sm">Status ❓ : <span class="font-medium">{rover.batteries.status}</span></p>
				<p class="text-base sm:text-sm">Control Source 🎮 : <span class="font-medium">{rover.controlSource}</span></p>
				<p class="text-base sm:text-sm">Fuel ⛽ : <span class="font-medium">{rover.fuel}%</span></p>

				<div class="flex flex-col sm:flex-row justify-between space-y-2 sm:space-y-0">
					<p class="text-base sm:text-sm">Voltage ⚡ : <span class="font-medium">{rover.voltage} V</span></p>
					<p class="text-base sm:text-sm">Current 🅰 : <span class="font-medium">{rover.current} A</span></p>
				</div>


				<div class="flex flex-col sm:flex-row justify-between space-y-2 sm:space-y-0">
					<p class="text-base sm:text-sm">RSSI 📶: <span class="font-medium">{rover.rssi} dBm</span></p>
					<p class="text-base sm:text-sm">Signal 📻: <span class="font-medium">{rover.linkQualityThreshold} dBm</span></p>
				</div>


				<div class="flex flex-col sm:flex-row justify-between space-y-2 sm:space-y-0">
					<p class="text-base sm:text-sm">Offset L: <span class="font-medium">{rover.offset.left} °</span></p>
					<p class="text-base sm:text-sm">Offset R: <span class="font-medium">{rover.offset.right} °</span></p>
				</div>


			</div>

		</div>
	);
};

const EditValuesForm = () => {
	// Function to handle form submission for each field
	const handleSubmit = (fieldName, event) => {
	  event.preventDefault();
	  // Process individual field value here
	  // You would typically want to send the value of the field specified by `fieldName`
	  console.log(fieldName, event.target[fieldName].value);
	};
  
	return (
	  <div className="bg-slate-200 p-4 sm:p-6 max-w-lg mx-auto rounded-lg shadow-md">
		<h2 className="text-xl sm:text-2xl font-bold mb-4">Edit Values</h2>
		
		{/* Number of batteries */}
		<form onSubmit={(e) => handleSubmit('numBatteries', e)} className="mb-4">
		  <label htmlFor="numBatteries" className="block text-sm font-medium text-gray-700">
			Number of batteries
		  </label>
		  <div className="mt-1 flex rounded-md shadow-sm">
			<input type="number" id="numBatteries" name="numBatteries" className="focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md" required />
			<button type="submit" className="ml-2 bg-blue-500 text-white py-2 px-4 rounded-r-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-opacity-50">
			  Save
			</button>
		  </div>
		</form>
  
		{/* Low Voltage Threshold */}
		<form onSubmit={(e) => handleSubmit('lowVoltageThreshold', e)} className="mb-4">
		  <label htmlFor="lowVoltageThreshold" className="block text-sm font-medium text-gray-700">
			Low Voltage threshold
		  </label>
		  <div className="mt-1 flex rounded-md shadow-sm">
			<input type="number" step="0.1" id="lowVoltageThreshold" name="lowVoltageThreshold" className="focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md" required />
			<button type="submit" className="ml-2 bg-blue-500 text-white py-2 px-4 rounded-r-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-opacity-50">
			  Save
			</button>
		  </div>
		</form>
  
		{/* Critical Voltage Threshold */}
		<form onSubmit={(e) => handleSubmit('criticalVoltageThreshold', e)} className="mb-4">
		  <label htmlFor="criticalVoltageThreshold" className="block text-sm font-medium text-gray-700">
			Critical Voltage Threshold
		  </label>
		  <div className="mt-1 flex rounded-md shadow-sm">
			<input type="number" step="0.1" id="criticalVoltageThreshold" name="criticalVoltageThreshold" className="focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md" required />
			<button type="submit" className="ml-2 bg-blue-500 text-white py-2 px-4 rounded-r-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-opacity-50">
			  Save
			</button>
		  </div>
		</form>
  
		{/* Reference Wheel Angle */}
		<form onSubmit={(e) => handleSubmit('referenceWheelAngle', e)} className="mb-4">
		  <label htmlFor="referenceWheelAngle" className="block text-sm font-medium text-gray-700">
			Reference Wheel Angle
		  </label>
		  <div className="mt-1 flex rounded-md shadow-sm">
			<input type="number" step="0.2" id="referenceWheelAngle" name="referenceWheelAngle" className="focus:ring-blue-500 focus:border-blue-500 block w-full pl-2 sm:text-sm border-gray-300 rounded-l-md" required />
			<button type="submit" className="ml-2 bg-blue-500 text-white py-2 px-4 rounded-r-md hover:bg-blue-600 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-opacity-50">
			  Save
			</button>
		  </div>
		</form>
  
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
		batteries: {
			number: 4,
			status: "NOT_X"
		},
		controlSource: "RC",
		fuel: 50,
		voltage: 12,
		current: 2,
		rssi: -50,
		linkQualityThreshold: -50,
		offset: {
			left: 0,
			right: 0
		}
	});

	// Open websocket connection
	// only open once when the component is mounted
	useEffect(() => {
		//const ws = new WebSocket('ws://' + location.host + '/echo');
		const ws = new WebSocket('ws://rover.local:81/');

		// On websocket open
		ws.onopen = () => {
			console.log('Connected to server');
		};

		ws.onmessage = (event) => {

			console.log('Message received: ', event.data);

			// const data = JSON.parse(event.data);
			// setRover(data);
		}

		return () => {
			ws.close();
		}
	}
		, []);

	const wsClient = ws;

	return (
		<main className="space-y-8">
			<Dashboard rover={rover}/>
			<EditValuesForm />
		</main>
	);
}

render(<App />, document.getElementById('app'));
