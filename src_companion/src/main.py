import time
import network
import socket
import asyncio
from machine import Pin, UART
import _thread
from microdot import Microdot, send_file
from microdot.websocket import with_websocket

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

        # Send update
        toSend = str(message)
        await ws.send(toSend)
        await asyncio.sleep(1)
    

## Listen for UART data on one thread
uart_thread = _thread.start_new_thread(uart_loop, ())

## Web server on another thread
#connect() # wifi mode
ap_mode() # ap mode
app.run(port=80, debug=True)
