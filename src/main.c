#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "argparse.h"
#include "config.h"
#include "bin2c.h"

#define LOG_LEVEL   DEBUG
#include "verbose_seethe.h"

#define VERSION "0.1.0"

bool verbose_enabled_flag;

static const char *const usage[] = {
    "bin2c [options] [[--] args]",
    "bin2c [options]",
    NULL
};

int main(int argc, const char **argv)
{
    char *config_file = NULL;
    bool show_version = false;
    config_st cfg;

    int result = 0;
    int temp;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_STRING('c', "config", &config_file, "config filename"),
        OPT_BOOLEAN('r', "version", &show_version, "show version"),
        OPT_BOOLEAN('v', "verbose", &verbose_enabled_flag, "verbose mode"),
        OPT_END()
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usage, 0);
    argparse_describe(
        &argparse,
        "\nProgram to create a c source file from a binary image.",
        "\n(Specify '-h' for help).");
    argc = argparse_parse(&argparse, argc, argv);

    if (show_version)
    {
        printf("bin2c version: %s\n", VERSION);
        result = 0;
        goto done;
    }
    if (config_file == NULL)
    {
        config_file = "default.json";
        verbose_info("Using default config file: %s",config_file);
    }
    verbose_info("Config filename: %s",config_file);

    result = config_read(config_file, &cfg);
    if (result != 0)
    {
        verbose_error("Error reading config file: %s",config_file);
        goto done;
    }

    result = bin2c_write(&cfg);
    if (result != 0)
    {
        verbose_error("Error writing output file(s)");
        goto done;
    }

    temp = 0;
    if (temp != 0)
    {
        result = temp;
        goto done;
    }

    verbose_info("Done!");

done:
    return result;
}

