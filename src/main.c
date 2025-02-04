/*
 * MIT License
 *
 * Copyright (c) 2025 Ivan Nikolskiy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linenoise.h>

#include <cmd/cmd.h>

void shell_loop(struct command *commands)
{
    char *line;
    char *token;
    char *rest;

    char *argv[64];
    int argc;

    struct command *cmd;

    linenoiseHistoryLoad("history.txt");

    while ((line = linenoise("C:\\> ")) != NULL)
    {
        argc = 0;
        rest = line;

        while ((token = strtok_r(rest, " ", &rest)) && argc < 63)
        {
            argv[argc++] = token;
        }

        argv[argc] = NULL;

        if (argc <= 0)
        {
            linenoiseHistoryAdd(line);
            free(line);
            continue;
        }

        HASH_FIND_STR(commands, argv[0], cmd);

        if (cmd == NULL)
        {
            printf("'%s' is not recognized as an internal or external command,\n"
                   "operable program or batch file.\n", argv[0]);
            linenoiseHistoryAdd(line);
            free(line);
            continue;
        }

        if (cmd->func(argc, argv))
        {
            linenoiseHistoryAdd(line);
            free(line);
            break;
        }

        linenoiseHistoryAdd(line);
        free(line);
    }

    linenoiseHistorySave("history.txt");
}

int main(void)
{
    struct command *commands;
    struct command *current, *tmp;

    commands = NULL;
    register_all_commands(&commands);

    linenoiseSetMultiLine(1);
    linenoiseSetCompletionCallback(NULL);
    shell_loop(commands);

    HASH_ITER(hh, commands, current, tmp)
    {
        HASH_DEL(commands, current);
        free(current->name);
        free(current);
    }

    return 0;
}