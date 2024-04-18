import { render } from 'preact';
import MotorDisplay from './components/MotorDisplay';
import GeneralDisplay from './components/GeneralDisplay';
import Variable from './components/Variable';
import DisplayPanel from './components/DisplayPanel';
import { useEffect, useState } from 'preact/hooks';
import { RoverState } from './types';
import './style.css';
import ws from './util/WebSocket';
import { updateBatteries } from './util/Updates';

export function App() {

	const [rover, setRover] = useState<RoverState>({
		motors: {
			motor1: {
				temperature: 22,
				rpm: 40,
				angle: 3
			},
			motor2: {
				temperature: 24,
				rpm: 50,
				angle: 3
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
		signalStrength: -50,
		offset: {
			left: 0,
			right: 0
		}
	});

	// Open websocket connection
	// only open once when the component is mounted

	const wsClient = ws;

	return (
		<DisplayPanel>
			{/*Motors Display*/}
			<MotorDisplay title="Motor 1" temperature={rover.motors.motor1.temperature} rpm={rover.motors.motor1.rpm} angle={rover.motors.motor1.angle} />
			<MotorDisplay title="Motor 2" temperature={rover.motors.motor2.temperature} rpm={rover.motors.motor2.rpm} angle={rover.motors.motor2.angle} />

			{/* Batteries and Solar Panel Display */}
			<GeneralDisplay>
				<Variable title="No. Bats" value={rover.batteries.number} />
				<Variable title='Status' value={rover.batteries.status} />
			</GeneralDisplay>

			{/* Control source */}
			<GeneralDisplay>
				<Variable title="Control Source" value={rover.controlSource} />
			</GeneralDisplay>

			{/* Fuel percentage, Voltage and Current */}
			<GeneralDisplay>
				<Variable title="Fuel" value={rover.fuel} unit='%'/>
			</GeneralDisplay>

			<GeneralDisplay>
				<Variable title="Voltage" value={rover.voltage} unit="V" />
				<Variable title="Current" value={rover.current} unit="A" />
			</GeneralDisplay>

			{/* RSSI and Signal Strength (Link) */}
			<GeneralDisplay>
				<Variable title="RSSI" value={rover.rssi} unit="dBm" />
				<Variable title="Signal" value={rover.signalStrength} unit="dBm" />
			</GeneralDisplay>

			{/*Motor offset left and right*/}
			<GeneralDisplay>
				<Variable title="Offset L" value={rover.offset.left} unit="°" />
				<Variable title="Offset R" value={rover.offset.right} unit="°" />
			</GeneralDisplay>

			<button onClick={() => {ws.send(updateBatteries(10))}}> Test Update</button>
		</DisplayPanel>
	);
}

render(<App />, document.getElementById('app'));
