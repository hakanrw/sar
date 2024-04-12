#include <sar.h>
#include <time.h>
#include <string.h>

int test_create_sar() {
  struct sar_meta meta = {
    .time = time(NULL),
    .author = "Hakan Candar",
    .latitude = 40.97221561185785,
    .longitude = 29.152147934012536
  };

  char data[64] = { 0 };
  strcpy(data, "Every move watched, every thought known.");

  int result = create_sar("example.sar", &meta, data, sizeof(data));
  printf("create_sar result: %d\n", result);

  return result;
}

int test_read_sar() {
  struct sar_meta meta;
  char data[64] = { 0 };

  int result = read_sar("example.sar", &meta, data, 64);
  printf("read_sar result: %d\n", result);

  if (result != -1) {
    printf("\nSAR header\n");
    printf("Author: %s\n", meta.author);

    char time_buffer[32];
    time_t time = (time_t) meta.time;
    struct tm *timeinfo = localtime(&time);
    strftime(time_buffer, 32, "%Y-%m-%d %H:%M:%S", timeinfo);

    printf("Time: %s\n", time_buffer);
    printf("Latitude: %f\n", meta.latitude);
    printf("Longtitude: %f\n", meta.longitude);

    printf("\nData\n");
    printf("%s\n", data);
  }

  return result;
}

int main() {
  return !(test_create_sar() != -1 && test_read_sar() != -1);
}
