#include <Arduino.h>
#include <MiniShell.h>

static MiniShell shell(&Serial);

static int do_hello(int argc, char *argv[])
{
    Serial.println("Hello to you too!");
    return 0;
}

static const cmd_t commands[] = {
    { "hello", do_hello, "Say hello" },
    { NULL, NULL, NULL }
};

void setup(void)
{
    Serial.begin(115200);
}

void loop(void)
{
    shell.process(">", commands);
}

