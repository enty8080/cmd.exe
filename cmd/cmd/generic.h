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

#ifndef _CMD_GENERIC_H_
#define _CMD_GENERIC_H_

#include <stdio.h>

#include <cmd/cmd.h>

static int generic_echo(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }

    printf("\n");
    return 0;
}

static int generic_exit(int argc, char **argv)
{
    return 1;
}

void register_generic_commands(struct command **commands)
{
    cmd_add_command(commands, "echo", generic_echo);
    cmd_add_command(commands, "exit", generic_exit);
}

#endif