/**
 * Simple demo of a 'remote shell' to an ESP8266 over TCP port 8888.
 * 
 * Upon first use, you need to connect to the WiFiManager host 'netcat' to configure the WiFi settings. 
 * 
 * On the host side, you can then use the following to interact with the shell running on the ESP8266:
 *   netcat netcat.local 8888 
 */

#include <Arduino.h>

#include <WiFiManager.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#include <MiniShell.h>

static WiFiManager wifiManager;
static WiFiServer server(8888);
static WiFiClient client;

static int do_hello(int argc, char *argv[])
{
    client.printf("Hello to you too, you provided %d arguments!\n", argc);
    return 0;
}

static const cmd_t commands[] = {
    {"hello", do_hello, "Say hello"},
    {NULL, NULL, NULL}
};

void setup(void)
{
    wifiManager.autoConnect();
    server.begin();
    MDNS.begin("netcat");
}

void loop(void)
{
    client = server.accept();
    if (client) {
        MiniShell shell(&client);
        while (client.connected()) {
            shell.process(">", commands);
        }
    }
    MDNS.update();
}

