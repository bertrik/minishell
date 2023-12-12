#include <stddef.h>
#include <string.h>

#include "MiniShell.h"

constexpr char BELL = 0x07;
constexpr char BS = 0x08;
constexpr char LF = 0x0A;
constexpr char CR = 0x0D;
constexpr char DEL = 0x7F;

// maximum number of arguments (including command itself)
#define CMD_MAX_ARGS 10

// default result codes
#define CMD_OK      0
#define CMD_NO_CMD  -0x7F00
#define CMD_UNKNOWN -0x7F01

MiniShell::MiniShell(Stream *stream, int line_size)
{
    _stream = stream;
    _line = new char[line_size];
    _line_size = line_size;
    _line_idx = 0;
}

MiniShell::~MiniShell()
{
    delete[] _line;
}

void MiniShell::show_help(const cmd_t *commands)
{
    for (const cmd_t * cmd = commands; cmd->cmd != nullptr; cmd++) {
        _stream->printf("%10s: %s\n", cmd->name, cmd->help);
    }
}

static int cmd_split(char *input, char *args[], int maxargs)
{
    int argc = 0;
    char *next = strtok(input, " ");
    while ((next != nullptr) && (argc < maxargs)) {
        args[argc++] = next;
        next = strtok(nullptr, " ");
    }
    return argc;
}

static const cmd_t *cmd_find(const cmd_t *commands, const char *name)
{
    const cmd_t *match = nullptr;
    int len = strlen(name);
    for (const cmd_t * cmd = commands; cmd->cmd != nullptr; cmd++) {
        if (strncmp(name, cmd->name, len) == 0) {
            if (match == nullptr) {
                match = cmd;
            } else {
                // multiple match
                match = nullptr;
                break;
            }
        }
    }
    return match;
}

static int cmd_process(const cmd_t *commands, char *line)
{
    char *argv[CMD_MAX_ARGS];

    // parse line
    int argc = cmd_split(line, argv, CMD_MAX_ARGS);
    if (argc == 0) {
        // no command present
        return CMD_NO_CMD;
    }
    // find matching entry
    const cmd_t *cmd = cmd_find(commands, argv[0]);
    if (cmd == nullptr) {
        // no command found
        return CMD_UNKNOWN;
    }
    // execute
    int res = cmd->cmd(argc, argv);
    return res;
}

void MiniShell::process(const char *prompt, const cmd_t *commands)
{
    while (_stream->available()) {
        char c = _stream->read();
        if (edit_line(c)) {
            int result = cmd_process(commands, _line);
            switch (result) {
            case CMD_OK:
                _stream->print("OK\n");
                break;
            case CMD_NO_CMD:
                break;
            case CMD_UNKNOWN:
                _stream->print("Unknown command, available commands:\n");
                show_help(commands);
                break;
            default:
                _stream->printf("%d\n", result);
                break;
            }
            _stream->print(prompt);
        }
    }
}

bool MiniShell::edit_line(char cin)
{
    char cout = cin;            // echo by default

    bool result = false;
    switch (cin) {
    case CR:                   // carriage return is ignored
        break;

    case LF:                   // end-of-line
        _line[_line_idx] = '\0';
        _line_idx = 0;
        result = true;
        break;

    case DEL:
    case BS:
        if (_line_idx > 0) {
            _line_idx--;
            _stream->write(cout);
            _stream->write(' ');
        } else {
            cout = BELL;
        }
        break;

    default:                   // store char as long as there is space to do so
        if (_line_idx < (_line_size - 1)) {
            _line[_line_idx++] = cin;
        } else {
            cout = BELL;
        }
        break;
    }
    _stream->write(cout);
    return result;
}
