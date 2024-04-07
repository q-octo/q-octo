import asyncio
from websockets.server import serve
import time
import random
import json

async def hello(websocket):
    while True:
        await websocket.send(dummyData())
        await asyncio.sleep(0.5)
    

async def main():
    async with serve(hello, "localhost", 8765):
        await asyncio.Future()  # run forever


def dummyData():
    data ={"no_batt": 4,
        "conn_type": "FL_CON",
        "status": "NOTX",
        "temp1": random.randint(20, 25),
        "rpm1": random.randint(40, 45),
        "ang1": random.randint(0, 180),
        "wifi": 0,
        "temp2": random.randint(20, 25),
        "rpm2": random.randint(40, 45),
        "ang2": random.randint(0, 180),
        "fuel": 84,
        "voltage": 22.1,
        "current": 14.1,
        "rssi": 77,
        "link": 64}
    
    dump = json.dumps(data)

    return f"STATE:{dump}"
    

asyncio.run(main())