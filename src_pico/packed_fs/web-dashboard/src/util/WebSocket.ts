import { Robot } from "../../generated/qocto/wsschema";
import { RobotToObj } from "./RobotToObj";
import * as flatbuffers from 'flatbuffers';

//const ws = new WebSocket('ws://' + location.host + '/echo');

const ws = new WebSocket('ws://localhost:8080');

// On websocket open
ws.onopen = () => {
    console.log('Connected to server');
};

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

            }
        } catch (error) {
            console.log("Invalid data received from server");
        }
    }

export default ws;
