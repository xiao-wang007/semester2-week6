#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int factorial(int n) {
  if (n == 0) {
    return 1;
  }

  return n * factorial(n - 1);
}

static int parse_positive_int(const char *text, int *result) {
  char *endptr;
  long value;

  value = strtol(text, &endptr, 10);
  if (*text == '\0' || *endptr != '\0' || value < 0 || value > 12) {
    return 0;
  }

  *result = (int)value;
  return 1;
}

static int ncr(int n, int r) {
  return factorial(n) / (factorial(r) * factorial(n - r));
}

static int npr(int n, int r) {
  return factorial(n) / factorial(n - r);
}

int main(int argc, char **argv) {
  int n;
  int r;
  char op;

  if (argc != 4) {
    printf("Usage: ./combinatorials n C|P r\n");
    return 1;
  }

  if (!parse_positive_int(argv[1], &n) || !parse_positive_int(argv[3], &r)) {
    printf("Error: n and r must be integers from 0 to 12.\n");
    return 1;
  }

  if (r > n) {
    printf("Error: r must be less than or equal to n.\n");
    return 1;
  }

  if (argv[2][0] == '\0' || argv[2][1] != '\0') {
    printf("Error: operation must be C or P.\n");
    return 1;
  }

  op = (char)toupper((unsigned char)argv[2][0]);

  if (op == 'C') {
    printf("%dC%d = %d\n", n, r, ncr(n, r));
    return 0;
  }

  if (op == 'P') {
    printf("%dP%d = %d\n", n, r, npr(n, r));
    return 0;
  }

  printf("Error: operation must be C or P.\n");
  return 1;
}
