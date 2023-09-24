#include <stdio.h>
#include <stdlib.h>

#include "argparse.h"
#include "config.h"
#include "bin2c.h"

#define LOG_LEVEL   DEBUG
#include "seethe.h"

static const char *const usage[] = {
    "bin2c [options] [[--] args]",
    "bin2c [options]",
    NULL
};

int main(int argc, const char **argv)
{
    char *config_file = NULL;
    config_st cfg;

    int result = 0;
    int temp;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_STRING('c', "config", &config_file, "config filename"),
        OPT_END()
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usage, 0);
    argparse_describe(
        &argparse,
        "\nProgram to create a c source file from a binary image.",
        "\n(All file parameters are required).");
    argc = argparse_parse(&argparse, argc, argv);


    info("Config filename: %s",config_file);

    result = config_read(config_file, &cfg);
    if (result != 0)
    {
        error("Error reading config file: %s",config_file);
        goto done;
    }

    result = bin2c_write(&cfg);
    if (result != 0)
    {
        error("Error writing output file(s)");
        goto done;
    }

    temp = 0;
    if (temp != 0)
    {
        result = temp;
        goto done;
    }

    info("Done!");

done:
    return result;
}

