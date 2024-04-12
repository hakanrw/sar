#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sar.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename> [-m | -o-]\n", argv[0]);
        printf("Options:\n");
        printf("  -m   Only show metadata of the SAR archive\n");
        printf("  -o-   Output to terminal instead of creating a file\n");
        return 1;
    }

    const char *filename = argv[1];
    int show_meta_only = 0;
    int print_to_terminal = 0;

    // Check if -m option is provided
    if (argc == 3 && strcmp(argv[2], "-m") == 0) {
        show_meta_only = 1;
    }

    // Check if -o- option is provided
    if (argc == 3 && strcmp(argv[2], "-o-") == 0) {
        print_to_terminal = 1;
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file for reading");
        fclose(file);
        return -1;
    }

    // Skip metadata
    fseek(file, sizeof(struct sar_header) + sizeof(struct sar_meta), SEEK_SET);

    struct sar_meta meta;

    // Read SAR file
    if (read_sar(filename, &meta, NULL, 0) == -1) {
        printf("Failed to read SAR file\n");
        fclose(file);
        return -1;
    }
    
    if (show_meta_only) {
        // Display metadata if requested
        printf("Author: %s\n", meta.author);
        printf("Timestamp: %lu\n", meta.time);
        printf("Latitude: %f\n", meta.latitude);
        printf("Longitude: %f\n", meta.longitude);
        fclose(file);
        return 0;
    }

    if (print_to_terminal) {
        int c;
        while ((c = fgetc(file)) != EOF) {
            putchar(c);
        }

        fclose(file);
        return 0;
    }

    // Create output filename without .sar extension or with .1 suffix
    char output_filename[256];
    char *dot_sar = strstr(filename, ".sar");
    if (dot_sar != NULL && dot_sar == filename + strlen(filename) - 4) {
        strncpy(output_filename, filename, strlen(filename) - 4);
        output_filename[strlen(filename) - 4] = '\0';
    } else {
        snprintf(output_filename, sizeof(output_filename), "%s.1", filename);
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        perror("Failed to open output file for writing");
        fclose(file);
        fclose(output_file);
        return -1;
    }

    // Print file content
    int c;
    while ((c = fgetc(file)) != EOF) {
        fputc(c, output_file);
    }

    fclose(file);
    fclose(output_file);
    return 0;
}
