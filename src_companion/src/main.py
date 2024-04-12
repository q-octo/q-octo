import time
import network
import socket
import asyncio
from machine import Pin, UART
import _thread
from microdot import Microdot, send_file
from microdot.websocket import with_websocket
import random
import json

# Constants
UART_TX_PIN = 0
UART_RX_PIN = 1
UART_ID = 0
BAUD_RATE = 9600

ssid = 'NPhone'
password = 'Hello12345'

message = 'Nothing'
state = 0

# Connect to the wifi network 
def connect():
    wlan = network.WLAN(network.STA_IF)

    # Configure to rover.local
    wlan.config(hostname='picow')

    status = wlan.ifconfig()
    print( 'ip = ' + status[0] )

    ## If already connected, return
    if wlan.isconnected():
        print('Already connected to network')
        return

    wlan.active(True)
    wlan.connect(ssid, password)
    max_wait = 10
    while max_wait > 0:
        if wlan.status() < 0 or wlan.status() >= 3:
            break
        max_wait -= 1
        print('waiting for connection...')
        time.sleep(1)
        
    if wlan.status() != 3:
        raise RuntimeError('network connection failed')
    else:
        print('connected')
        status = wlan.ifconfig()
        print( 'ip = ' + status[0] )
        addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
        print('listening on', addr)

# For testing purposes, generate random telemetry data
def generate_telemetry_data():
    return json.dumps({
        "temp_left": random.randint(0, 40),
        "temp_right": random.randint(0, 40),
        "rpm_left": random.randint(0, 100),
        "rpm_right": random.randint(0, 100),
        "angle_left": random.randint(0, 360),
        "angle_right": random.randint(0, 360),
        "bat_num": random.randint(0, 4),
        "status": random.choice(["OK", "FOO", "BAR"]),
        "source": random.choice(["Flight Controller", "AI", "Manual Control"]),
        "fuel": random.randint(0, 100),
        "voltage": round(random.uniform(10.0, 25.0),2),
        "current": round(random.uniform(10.0, 21.0),2),
        "rssi": random.randint(60, 100),
        "link": random.randint(0, 100),
        "offset_left": random.randint(0, 360),
        "offset_right": random.randint(0, 360)
        })

# AP mode instead of station mode
def ap_mode():
    #network.hostname('picow')
    #network.country('GB')

    ap = network.WLAN(network.AP_IF)
    ap.config(essid='mysterious pico', password='password')
    ap.ifconfig(("10.10.1.1", "255.255.255.0", "10.10.1.1", "10.10.1.1"))
    ap.active(True)

# Listen for UART data
def uart_loop():
    global message
    uart = UART(UART_ID, BAUD_RATE, tx=UART_TX_PIN, rx=UART_RX_PIN)
    uart.init()
    # Check UART buffer for incoming data
    while True:
            if uart.any():
                message = uart.read()
                print("Received: " + str(message))

# Initialize the web server
app = Microdot()

@app.route('/hello')
async def hello(request):

    global state

    state += 1

    return f"Hello World! {state}"

@app.route('/')
async def index(request):
    return send_file('index.html')

@app.route('/echo')
@with_websocket
async def echo(request, ws):
    
    global message
    
    while True:
        # Send state update
        toSend = str(generate_telemetry_data())
        
        await ws.send(toSend)

        await asyncio.sleep(1)

@app.route('/state')
async def state(request):   

    print("State: " + str(request.body))

    return "Success"
    
## Listen for UART data on one thread
uart_thread = _thread.start_new_thread(uart_loop, ())

## Web server on another thread
connect() # wifi mode
#ap_mode() # ap mode
app.run(port=80, debug=True)
