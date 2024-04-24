const ws = new WebSocket('ws://rover.local:81/');
//const ws = new WebSocket('ws://localhost:8080');

// On websocket open
ws.onopen = () => {
    console.log('Connected to server');
};

export default ws;
