#include <stdio.h>
#include <stdlib.h>

static int read_int(const char *prompt, int *out_value) {
  char buffer[64];
  char extra;

  printf("%s", prompt);
  if (!fgets(buffer, sizeof(buffer), stdin)) {
    return 0;
  }

  if (sscanf(buffer, "%d %c", out_value, &extra) != 1) {
    return 0;
  }

  return 1;
}

static int calculate_fine(int speed_limit, int driver_speed) {
  int excess_speed = driver_speed - speed_limit;

  if (excess_speed <= 0) {
    return 0;
  }
  if (excess_speed <= 10) {
    return 50;
  }
  if (excess_speed <= 20) {
    return 100;
  }
  return 200;
}

int main(void) {
  int speed_limit;
  int driver_speed;

  if (!read_int("Enter the speed limit: ", &speed_limit) || speed_limit < 0) {
    printf("Error: invalid speed limit.\n");
    return 1;
  }

  if (!read_int("Enter the driver's speed: ", &driver_speed) ||
      driver_speed < 0) {
    printf("Error: invalid driver speed.\n");
    return 1;
  }

  int fine = calculate_fine(speed_limit, driver_speed);
  if (fine == 0) {
    printf("No fine needed.\n");
  } else {
    printf("Fine: $%d\n", fine);
  }

  return 0;
}
