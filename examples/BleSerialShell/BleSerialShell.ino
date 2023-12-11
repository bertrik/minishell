#include <Arduino.h>

#include <MiniShell.h>
#include <BleSerial.h>

static BleSerial bleSerial;
static MiniShell shell(&bleSerial);

static int do_hello(int argc, char *argv[])
{
    bleSerial.println("Hello to you too!");
    return 0;
}

static const cmd_t commands[] = {
    { "hello", do_hello, "Say hello" },
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

