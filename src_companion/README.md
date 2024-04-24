# Companion Pico


## Communication:

Things we need to do here:
* Push state to the display
* Forward display button events (one to toggle wifi, one to toggle between screens)
    The screen toggle state doesn't require a round trip.
    Wi-Fi should do a round trip to avoid state inconsistencies.

* Push state to websocket clients (as it changes [throttled] and once when a client joins)
* Forward web dashboard update messages
    This should do a round trip to the other pico and back here so other clients
    stay in sync.

**Message types to accept**:
* State update
* Enable web server
* Disable web server

**Message types to send**
* Button pressed (A/B/X)
* Dashboard update message


- [x] Display should create a statically allocated buffer and parse it into a
state object
- [x] Check the size of a serialised Robot object. The buffers to hold it are
currently set to 1024
- [x] Create a macro to define the buffer size in a single place.

For Niall
- [ ] Listen for serial messages. Check the type of an incoming message. If it
is a state message then call `updateState()` on the display and web server tasks.
- [ ] Implement display stuff.
- [ ] Forward binary data received over web sockets to the main pico.
- [ ] Button presses should cause a flatbuffer to be serialised and sent to the
main pico. A new flatbuffers schema will likely be needed.


Note that since we can send multiple flatbuffers messages back to back, we
need to know where one stops and the next one starts.
Assuming that no corruption happens, a length byte (length prefixing) at the
start should suffice.

We should expect this when parsing messages here too because we may want to
send two messages consecutively.