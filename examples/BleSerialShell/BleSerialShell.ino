#include <Arduino.h>

#include <MiniShell.h>
#include <BLESerial.h>

static BLESerial bleSerial;
static MiniShell shell(&bleSerial);

static int do_hello(int argc, char *argv[])
{
    bleSerial.println("Hello to you too!");
    bleSerial.flush();
    return 0;
}

static int do_led(int argc, char *argv[])
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    return 0;
}

static const cmd_t commands[] = {
    { "hello", do_hello, "Say hello" },
    { "led", do_led, "LED" },
    { NULL, NULL, NULL }
};

void setup(void)
{
    bleSerial.begin("BLE-serial");
}

void loop(void)
{
    shell.process(">", commands);
}
