import asyncio
import websockets
import json
import random

# Function to generate random telemetry data
def generate_telemetry_data():
    return {
        "temp_left": random.randint(0, 40),
        "temp_right": random.randint(0, 40),
        "rpm_left": random.randint(0, 100),
        "rpm_right": random.randint(0, 100),
        "angle_left": random.randint(0, 360),
        "angle_right": random.randint(0, 360),
        "bat_num": random.randint(0, 4),
        "source": random.choice(["Flight Controller", "AI", "Manual Control"]),
        "fuel": random.randint(0, 100),
        "voltage": round(random.uniform(10.0, 25.0),2),
        "current": round(random.uniform(10.0, 21.0),2),
        "rssi": random.randint(60, 100),
        "link": random.randint(0, 100),
        "offset_left": random.randint(0, 360),
        "offset_right": random.randint(0, 360)
    }

# The main handler for the WebSocket connection
async def websocket_handler(websocket, path):
    while True:
        data = generate_telemetry_data()
        await websocket.send(json.dumps(data))
        # Wait for 1 second before sending the next set of data
        await asyncio.sleep(1)

# Start the WebSocket server
start_server = websockets.serve(websocket_handler, "localhost", 8765)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
