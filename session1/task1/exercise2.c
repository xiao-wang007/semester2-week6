#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159

static int parse_positive_double(const char *value, double *result) {
  char *endptr;
  double parsed;

  parsed = strtod(value, &endptr);
  if (*value == '\0' || *endptr != '\0' || parsed <= 0) {
    return 0;
  }

  *result = parsed;
  return 1;
}

double rectangle_area(double width, double height) {
  return width * height;
}

double circle_area(double radius) {
  return PI * radius * radius;
}

double triangle_area(double base, double height) {
  return 0.5 * base * height;
}

int main(int argc, char *argv[]) {
  const char *shape;
  double a;
  double b;

  if (argc < 3) {
    printf("Usage:\n");
    printf("  ./shapes rectangle width height\n");
    printf("  ./shapes triangle base height\n");
    printf("  ./shapes circle radius\n");
    return 1;
  }

  shape = argv[1];

  if (strcmp(shape, "rectangle") == 0 || strcmp(shape, "triangle") == 0) {
    if (argc != 4) {
      printf("Error: %s needs two positive numbers.\n", shape);
      return 1;
    }

    if (!parse_positive_double(argv[2], &a) ||
        !parse_positive_double(argv[3], &b)) {
      printf("Error: inputs must be positive numbers.\n");
      return 1;
    }

    if (strcmp(shape, "rectangle") == 0) {
      printf("Rectangle Area: %.2f\n", rectangle_area(a, b));
    } else {
      printf("Triangle Area: %.2f\n", triangle_area(a, b));
    }

    return 0;
  }

  if (strcmp(shape, "circle") == 0) {
    if (argc != 3) {
      printf("Error: circle needs one positive number.\n");
      return 1;
    }

    if (!parse_positive_double(argv[2], &a)) {
      printf("Error: radius must be a positive number.\n");
      return 1;
    }

    printf("Circle Area: %.2f\n", circle_area(a));
    return 0;
  }

  printf("Error: Unknown shape '%s'. Use rectangle, triangle, or circle.\n",
         shape);
  return 1;
}
