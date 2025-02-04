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
#include <dirent.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

#include <cmd/misc.h>

void format_number_with_commas(long long num, char *buffer)
{
    char num_str[30];
    int len;
    int comma_pos;
    int iter2;
    int iter;

    sprintf(num_str, "%lld", num);
    len = strlen(num_str);
    comma_pos = len % 3 == 0 ? 3 : len % 3;
    iter2 = 0;

    for (iter = 0; iter < len; iter++)
    {
        buffer[iter2++] = num_str[iter];
        if (--comma_pos == 0 && iter != len - 1)
        {
            buffer[iter2++] = ',';
            comma_pos = 3;
        }
    }

    buffer[iter2] = '\0';
}

void convert_to_windows_path(const char *unix_path, char *win_path)
{
    char abs_path[PATH_MAX];
    char *path;

    realpath(unix_path, abs_path);

    if (abs_path[0] == '/')
    {
        strcpy(win_path, "C:\\");
        strcat(win_path, abs_path + 1);
    }
    else
    {
        strcpy(win_path, "C:\\");
        strcat(win_path, abs_path);
    }

    for (path = win_path; *path; path++)
    {
        if (*path == '/') *path = '\\';
    }
}
