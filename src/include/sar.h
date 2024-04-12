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

#ifndef SAR_H
#define SAR_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

struct sar_header
{
  char magic[4];      /* 0 */
  uint32_t version;   /* 4 */
  uint64_t size;      /* 8 */
                      /* 16 */
};

struct sar_meta
{
  char author[32];    /* 16 */
  uint64_t time;      /* 48 */
  double latitude;    /* 56 */
  double longitude;   /* 64 */
                      /* 72 */
};

int create_sar(const char *filename, const struct sar_meta *meta, const void *data, size_t data_size);

int read_sar(const char *filename, struct sar_meta *meta, void *data, size_t data_size);

#endif
