import serial
import time
import threading

# Configure the UART port
uart = serial.Serial("/dev/ttyS0", baudrate=9600, timeout=1)

# Function to send a message on UART
def send_message(message):
    uart.write(message.encode())

# Function to receive and print messages on UART
def receive_messages():
    while True:
        if uart.in_waiting > 0:
            received_message = uart.readline().decode().strip()
            print("Received message:", received_message)

# Function to periodically send a "Hello" message on UART
def send_hello_message():
    while True:
        send_message("Hello")
        time.sleep(1)  # Wait for 1 second

# Start receiving messages in a separate thread
receive_thread = threading.Thread(target=receive_messages)
receive_thread.start()

# Start sending "Hello" messages in a separate thread
send_thread = threading.Thread(target=send_hello_message)
send_thread.start()