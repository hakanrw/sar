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

#include "sar.h"

int create_sar(const char *filename, const struct sar_meta *meta, const void *data, size_t data_size) {
  struct sar_header header = {
    .magic = {'S', 'A', 'R', '\0'},
    .version = 1,
    .size = data_size + sizeof(struct sar_header) + sizeof(struct sar_meta),
  };

  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Failed to open file for writing");
    return -1;
  }

  // Write header to file
  if (fwrite(&header, sizeof(struct sar_header), 1, file) != 1) {
    perror("Failed to write header to file");
    fclose(file);
    return -1;
  }

  // Write meta to file
  if (fwrite(meta, sizeof(struct sar_meta), 1, file) != 1) {
    perror("Failed to write meta to file");
    fclose(file);
    return -1;
  }

  // Write data to file
  if (fwrite(data, data_size, 1, file) != 1) {
    perror("Failed to write data to file");
    fclose(file);
    return -1;
  }

  fclose(file);
  return 0;
}

int read_sar(const char *filename, struct sar_meta *meta, void *data, size_t data_size) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Failed to open file for reading");
    return -1;
  }

  struct sar_header header;
  memset(&header, 0, sizeof(struct sar_header));

  if (fread(&header, sizeof(struct sar_header), 1, file) != 1) {
    perror("Failed to read header from file");
  	fclose(file);
  	return -1;
  }

  if (strcmp(header.magic, "SAR") != 0) {
    perror("Not a SAR archive!");
    fclose(file);
    return -1;
  }

  memset(meta, 0, sizeof(struct sar_meta));

  if (fread(meta, sizeof(struct sar_meta), 1, file) != 1) {
    perror("Failed to read meta from file");
  	fclose(file);
  	return -1;
  }

  int read_amount = fread(data, 1, data_size, file);

  fclose(file);
  return read_amount;
}
