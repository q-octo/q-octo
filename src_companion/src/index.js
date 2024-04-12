// Open web socket connection
//const socket = new WebSocket('ws://' + location.host + '/echo');
const socket = new WebSocket('ws://localhost:8765/echo');

const updateDisplay = (data) => {
  // Modify the elements with id's matching the keys in the data object
  for (let key in data) {

    const element = document.getElementById(key);

    if (element !== null) {
        document.getElementById(key).textContent = data[key];
    }

  }
};

socket.addEventListener('message', ev => {

    console.log('Message from server', ev.data);

    updateDisplay(JSON.parse(ev.data));
  });
  socket.addEventListener('close', ev => {
    console.log('Connection closed', ev);
});

