import time
import network
import socket
from machine import Pin, UART
import _thread
from microdot import Microdot

# Constants
UART_TX_PIN = 0
UART_RX_PIN = 1
UART_ID = 0
BAUD_RATE = 9600

ssid = 'NPhone'
password = 'Hello12345'

message = 'Nothing'

# Connect to the network 
def connect():
    wlan = network.WLAN(network.STA_IF)
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
@app.route('/')
def index(request):
    return f'<h1>Message: {message}</h1>'

## Listen for UART data on one thread
uart_thread = _thread.start_new_thread(uart_loop, ())

## Web server on another thread
connect()
app.run(port=80)
