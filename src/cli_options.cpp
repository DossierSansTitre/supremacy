#include <cstring>
#include <cli_options.h>

CliOptions::CliOptions()
: legacy(false)
{

}

CliOptions& CliOptions::instance()
{
    static CliOptions opts;
    return opts;
}

void CliOptions::parse(int argc, char** argv)
{
    argc--;
    argv++;

    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "--legacy") == 0)
            legacy = true;
        else if (strcmp(argv[i], "--window") == 0)
            windowed = true;
    }
}
