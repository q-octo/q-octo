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
