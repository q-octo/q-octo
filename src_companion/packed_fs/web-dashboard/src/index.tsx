import * as flatbuffers from 'flatbuffers';
import { render } from 'preact';
import { useState } from 'preact/hooks';
import { UpdateUnion } from './generated/fbs';
import { Robot } from './generated/fbs/robot';
import './style.css';
import { RoverState } from './types';
import { RobotToObj } from './util/RobotToObj';
import { SendUpdate } from './util/SendUpdate';
import { Round, Round4 } from './util/Round';
import ws from './util/WebSocket';
import { EditValuesForm } from '../components/EditValuesForm';

const Motor = ({motor, name}) => {

	return(
		<div className="bg-gray-100 p-6 rounded-lg shadow-sm mb-4 md:mb-0 flex-1 md:mr-2">
			<h3 className="text-lg font-bold mb-2">{`Motor ${name}`}</h3>
			<p className="mb-1">Temp: <span className="font-medium">{Round(motor.temperature)}°C</span></p>
			<p className="mb-1">RPM: <span className="font-medium">{Round(motor.rps)} RPM</span></p>
			<p className="mb-1">Angle: <span className="font-medium">{Round(motor.angle)} °</span></p>
			<p>Kp: <span className="font-medium">{Round(motor.Kp)}</span></p>
			<p>Ki: <span className="font-medium">{Round4(motor.Kp)}</span></p>
			<p>Torque: <span className="font-medium">{Round(motor.torque)}Nm</span></p>
			<p><b>Max Torque:</b> <span className="font-medium">{Round(motor.torque_limit)}Nm</span></p>
			<p><b>Max Speed:</b> <span className="font-medium">{Round(motor.speed_limit)}</span></p>
			<p><b>Max Current:</b> <span className="font-medium">{Round(motor.current_limit)}A</span></p>
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
		status: "NOT_X",
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