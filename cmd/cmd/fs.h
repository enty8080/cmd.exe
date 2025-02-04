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

#ifndef _CMD_FS_H_
#define _CMD_FS_H_

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
#include <cmd/cmd.h>

static int fs_dir(int argc, char **argv)
{
    DIR *dir;

    struct dirent *entry;
    struct stat file_stat;
    struct statvfs vfs;
    struct tm *tm_info;

    char full_path[PATH_MAX];
    char win_path[PATH_MAX];
    char *path;

    char date_str[20];
    char time_str[20];
    char size_str[20];
    char with_commas[20];
    char total_files_str[20];
    char total_size_str[20];
    char free_space_str[20];
    char total_dirs_str[20];

    int num_files;
    int num_dirs;
    long long total_size;
    unsigned long long free_space;

    free_space = 0;
    num_files = 0;
    num_dirs = 0;
    total_size = 0;

    if (argc > 1)
    {
        path = argv[1];
    }

    path = ".";

    if ((dir = opendir(path)) == NULL)
    {
        perror("Directory not found");
        return 0;
    }

    convert_to_windows_path(path, win_path);

    printf("\n Volume in drive C has no label.\n");
    printf(" Volume Serial Number is 0000-0000\n\n");
    printf(" Directory of %s\n\n", win_path);

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.' &&
            strcmp(entry->d_name, ".") != 0 &&
            strcmp(entry->d_name, "..") != 0)
        {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (lstat(full_path, &file_stat) == -1)
        {
            continue;
        }

        tm_info = localtime(&file_stat.st_mtime);
        strftime(date_str, sizeof(date_str), "%m/%d/%Y", tm_info);
        strftime(time_str, sizeof(time_str), "%I:%M %p", tm_info);

        if (S_ISDIR(file_stat.st_mode))
        {
            strcpy(size_str, "<DIR>");
            num_dirs++;
        }
        else
        {
            format_number_with_commas(file_stat.st_size, with_commas);
            snprintf(size_str, sizeof(size_str), "%14s", with_commas);
            num_files++;
            total_size += file_stat.st_size;
        }

        printf("%10s  %8s %14s  %s\n",
               date_str, time_str,
               S_ISDIR(file_stat.st_mode) ? "<DIR>" : size_str,
               entry->d_name);
    }

    closedir(dir);

    if (statvfs(path, &vfs) == 0)
    {
        free_space = vfs.f_bavail * vfs.f_frsize;
    }

    format_number_with_commas(num_files, total_files_str);
    format_number_with_commas(total_size, total_size_str);
    format_number_with_commas(free_space, free_space_str);
    format_number_with_commas(num_dirs, total_dirs_str);

    printf("\n%15s File(s) %15s bytes\n", total_files_str, total_size_str);
    printf("%15s Dir(s)  %15s bytes free\n\n", total_dirs_str, free_space_str);

    return 0;
}

void register_fs_commands(struct command **commands)
{
    cmd_add_command(commands, "dir", fs_dir);
}

#endif