
var connection = new WebSocket("ws://localhost:8765/");
//var connection = new WebSocket("ws://rover.local:81/", ["arduino"]);

connection.onopen = function () {
  connection.send("Connect " + new Date());
};

connection.onerror = function (error) {
  console.log("WebSocket Error ", error);
};

connection.onclose = function (error) {
  console.log("WebSocket Closed ", error);
  //alert("WebSocket closed, refreshing page");

  //window.location.reload();
}

connection.onmessage = function (e) {
  console.log("Server: ", e.data);
  
  // If contains STATE: then parse
  if (e.data.includes("STATE:")) {
    var status = e.data.split("STATE:")[1];

    const state = JSON.parse(status)

    // Loop through the state object
    // If key exists as id in the document, update the value
    for (const key in state) {
      if (state.hasOwnProperty(key)) {
        const element = state[key];
        var el = document.getElementById(key);
        if (el) {
          el.innerHTML = element;
        }
      }
    }

  }
};
