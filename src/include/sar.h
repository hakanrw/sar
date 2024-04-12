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
  double longitude;  /* 64 */
                      /* 72 */
};

int create_sar(const char *filename, const struct sar_meta *meta, const void *data, size_t data_size);

int read_sar(const char *filename, struct sar_meta *meta, void *data, size_t data_size);

#endif
