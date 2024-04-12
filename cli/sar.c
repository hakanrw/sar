/*
 * SAR - Simple Archive Format
 * Copyright (C) 2024 Hakan Candar
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sar.h>

// Function to read file contents
char *read_file(const char *filename, size_t *file_size) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file for reading");
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate buffer to hold file contents
    char *buffer = (char *)malloc(*file_size);
    if (!buffer) {
        perror("Failed to allocate memory for file contents");
        fclose(file);
        return NULL;
    }

    // Read file contents into buffer
    if (fread(buffer, 1, *file_size, file) != *file_size) {
        perror("Failed to read file contents");
        free(buffer);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return buffer;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename> [-a author] [-t timestamp] [-lat latitude] [-long longitude]\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    const char *author = NULL;
    uint64_t timestamp = 0;
    double latitude = 0.0;
    double longitude = 0.0;

    // Parse command-line arguments
    for (int i = 2; i < argc; i += 2) {
        if (strcmp(argv[i], "-a") == 0) {
            if (i + 1 < argc) {
                author = argv[i + 1];
            } else {
                printf("Missing argument for -a option\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-t") == 0) {
            if (i + 1 < argc) {
                timestamp = strtoull(argv[i + 1], NULL, 10);
            } else {
                printf("Missing argument for -t option\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-lat") == 0) {
            if (i + 1 < argc) {
                latitude = atof(argv[i + 1]);
            } else {
                printf("Missing argument for -lat option\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-long") == 0) {
            if (i + 1 < argc) {
                longitude = atof(argv[i + 1]);
            } else {
                printf("Missing argument for -long option\n");
                return 1;
            }
        } else {
            printf("Unknown option: %s\n", argv[i]);
            return 1;
        }
    }

    // Get current Linux username as author if not provided
    if (author == NULL) {
        struct passwd *pw = getpwuid(getuid());
        if (pw) {
            author = pw->pw_name;
        } else {
            printf("Failed to get current Linux username\n");
            return 1;
        }
    }

    // Get current time as timestamp if not provided
    if (timestamp == 0) {
        timestamp = (uint64_t)time(NULL);
    }

    // Create SAR metadata
    struct sar_meta meta;
    strncpy(meta.author, author, sizeof(meta.author));
    meta.time = timestamp;
    meta.latitude = latitude;
    meta.longitude = longitude;


    // Read file
    size_t file_size;
    char *file_contents = read_file(filename, &file_size);
    if (!file_contents) {
        printf("Failed to read file: %s\n", filename);
        return 1;
    }
    
    char out_filename[256] = { 0 };
    strcat(out_filename, filename);
    strcat(out_filename, ".sar");
    
    // Create SAR file
    if (create_sar(out_filename, &meta, file_contents, file_size) == -1) {
        printf("Failed to create SAR file\n");
        return 1;
    }

    free(file_contents);

    return 0;
}