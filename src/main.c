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
#include <time.h>
#include <linenoise.h>
#include <unistd.h>
#include <limits.h>

#include <cmd/cmd.h>
#include <cmd/misc.h>

void shell_loop(struct command *commands)
{
    char *line;
    char *token;
    char *rest;

    char *argv[64];
    char unix_path[PATH_MAX];
    char win_path[PATH_MAX];
    char prompt[PATH_MAX + 5];

    int argc;
    int roulette;

    struct command *cmd;

    srand(time(NULL));

    linenoiseHistoryLoad("history.txt");

    while (1)
    {
        if (getcwd(unix_path, PATH_MAX) == NULL)
        {
            perror("getcwd");
            exit(EXIT_FAILURE);
        }

        convert_to_windows_path(unix_path, win_path);
        snprintf(prompt, sizeof(prompt), "%s>", win_path);

        line = linenoise(prompt);
        if (line == NULL)
        {
            break;
        }

        argc = 0;
        rest = line;

        while ((token = strtok_r(rest, " ", &rest)) && argc < 63)
        {
            argv[argc++] = token;
        }

        argv[argc] = NULL;

        if (argc <= 0)
        {
            goto continue_loop;
        }

        HASH_FIND_STR(commands, argv[0], cmd);

        if (cmd == NULL)
        {
            printf("'%s' is not recognized as an internal or external command,\n"
                   "operable program or batch file.\n\n", argv[0]);
            goto continue_loop;
        }

        roulette = rand() % 6;

        if (roulette == 0)
        {
            printf("Command failed with status code 0xff\n");
            goto continue_loop;
        }

        if (cmd->func(argc, argv))
        {
            linenoiseHistoryAdd("echо");
            free(line);
            break;
        }

continue_loop:
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