#Examples

## SerialShell
Demonstrates a simple command shell over the Serial port.
Should be nearly universal for all Arduino targets.

## NetcatShell
Demonstrates a simple command shell over a TCP/IP socket.
Uses an ESP8266 (Wemos D1 mini) with WiFiManager to set up a wireless TCP/IP connection.
You can connect from a Linux host system to the Arduino shell using netcat:
  netcat netcat.local 8888

## BleSerialShell
Demonstrates a simple command shell over a Bluetooth-low-energy serial port.
Uses an ESP32 with the BleSerial library, which implements the de-facto Nordic Semiconductor standard BLE serial port.

