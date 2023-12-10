#include <stdbool.h>
#include <Stream.h>

typedef int (cmd_fn_t)(int argc, char *argv[]);

// command table entry
typedef struct {
    const char *name;
    const cmd_fn_t *cmd;
    const char *help;
} cmd_t;


class MiniShell {

public:
    MiniShell(Stream *stream, int line_size = 120);
    ~MiniShell();

    void process(const char *prompt, const cmd_t *commands);

private:
    Stream *_stream;
    char *_line;
    int _line_size;
    int _line_idx;
    
    void show_help(const cmd_t * cmds);
    bool edit_line(char cin);

};

