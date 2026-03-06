#include <ctype.h>
#include <stdio.h>

int main(int argc, char **argv) {
  FILE *file;
  int ch;
  long lines = 0;
  long words = 0;
  long chars = 0;
  int in_word = 0;

  if (argc != 2) {
    printf("Usage: ./word_count <filename>\n");
    return 1;
  }

  file = fopen(argv[1], "r");
  if (!file) {
    printf("Error: could not open file '%s'\n", argv[1]);
    return 1;
  }

  while ((ch = fgetc(file)) != EOF) {
    chars++;

    if (ch == '\n') {
      lines++;
    }

    if (isspace((unsigned char)ch)) {
      in_word = 0;
    } else if (!in_word) {
      words++;
      in_word = 1;
    }
  }

  fclose(file);

  printf("%3ld lines\n", lines);
  printf("%3ld words\n", words);
  printf("%3ld characters\n", chars);

  return 0;
}
