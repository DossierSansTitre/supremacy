#ifndef CLI_OPTIONS_H
#define CLI_OPTIONS_H

struct CliOptions
{
    CliOptions();

    static CliOptions&  instance();
    void                parse(int argc, char** argv);

    bool    legacy;
    bool    sdl;
    bool    windowed;
};

#endif
