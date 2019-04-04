# ESPSkeleton
Skeleton code for an ESP microcontroller that acts as an MQTT client.
Uses a statemachine for handeling the connecting to the WiFi accesspoint.

The device will boot up in setup mode, in this mode an accesspoint is created by the device and a TCP server starts running.
By connecting to the generated AP the TCP server can be reached for configuration (App for configuration is a work in progress).
When connected to WiFi the device starts a connection to a MQTT server (in my case mosquitto MQTT on a VPS) and is ready to handle commands.

Logic for handleing MQTT messages still needs to be implemented.

A simple pushbutton switch must be connected to the ESP, this acts as a reset button.

TODO:
- implement OTA
- create an Android client

Robert Hendriks

