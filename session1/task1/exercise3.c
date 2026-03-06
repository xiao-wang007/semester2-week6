#include <stdio.h>
#include <stdlib.h>

void print_all(int count, int *numbers) {
  for (int i = 0; i < count; i++) {
    printf("%d%c", numbers[i], (i == count - 1) ? '\n' : ' ');
  }
}

int sum_all(int count, int *numbers) {
  int total = 0;

  for (int i = 0; i < count; i++) {
    total += numbers[i];
  }

  return total;
}

float average_all(int count, int *numbers) {
  int total = sum_all(count, numbers);
  return (float)total / count;
}

int find_max(int count, int *numbers) {
  int max = numbers[0];

  for (int i = 1; i < count; i++) {
    if (numbers[i] > max) {
      max = numbers[i];
    }
  }

  return max;
}

int find_min(int count, int *numbers) {
  int min = numbers[0];

  for (int i = 1; i < count; i++) {
    if (numbers[i] < min) {
      min = numbers[i];
    }
  }

  return min;
}

int find_mode(int count, int *numbers, int *has_mode) {
  int mode = numbers[0];
  int best_count = 1;
  *has_mode = 0;

  for (int i = 0; i < count; i++) {
    int current_count = 1;
    for (int j = i + 1; j < count; j++) {
      if (numbers[i] == numbers[j]) {
        current_count++;
      }
    }

    if (current_count > best_count) {
      best_count = current_count;
      mode = numbers[i];
      *has_mode = 1;
    }
  }

  return mode;
}

void print_repeated_values(int count, int *numbers) {
  int found = 0;

  printf("Repeated values: ");

  for (int i = 0; i < count; i++) {
    int already_reported = 0;
    int repeats = 0;

    for (int k = 0; k < i; k++) {
      if (numbers[k] == numbers[i]) {
        already_reported = 1;
        break;
      }
    }

    if (already_reported) {
      continue;
    }

    for (int j = i + 1; j < count; j++) {
      if (numbers[i] == numbers[j]) {
        repeats = 1;
        break;
      }
    }

    if (repeats) {
      if (found) {
        printf(" ");
      }
      printf("%d", numbers[i]);
      found = 1;
    }
  }

  if (!found) {
    printf("None");
  }

  printf("\n");
}

void sort_numbers(int count, int *numbers) {
  for (int i = 0; i < count - 1; i++) {
    for (int j = 0; j < count - i - 1; j++) {
      if (numbers[j] > numbers[j + 1]) {
        int temp = numbers[j];
        numbers[j] = numbers[j + 1];
        numbers[j + 1] = temp;
      }
    }
  }
}

double median_sorted(int count, int *numbers) {
  if (count % 2 == 1) {
    return numbers[count / 2];
  }

  return (numbers[(count / 2) - 1] + numbers[count / 2]) / 2.0;
}

int main(int argc, char **argv) {
  int *numbers;
  int count;
  char buffer[50];
  int choice = 0;

  if (argc < 2) {
    printf("Usage: ./averages num1 ... numx\n");
    return 1;
  }

  count = argc - 1;
  numbers = calloc((size_t)count, sizeof(int));
  if (!numbers) {
    printf("Error: memory allocation failed\n");
    return 1;
  }

  for (int i = 0; i < count; i++) {
    char extra;
    if (sscanf(argv[i + 1], "%d%c", &numbers[i], &extra) != 1) {
      printf("Error: Non-numerical argument\n");
      free(numbers);
      return 1;
    }
  }

  do {
    printf("1 - Show all values\n");
    printf("2 - Calculate sum\n");
    printf("3 - Calculate mean\n");
    printf("4 - Find max\n");
    printf("5 - Find min\n");
    printf("6 - Find range\n");
    printf("7 - Show repeated values\n");
    printf("8 - Find mode\n");
    printf("9 - Sort values\n");
    printf("10 - Find median\n");
    printf("11 - Exit\n");

    printf("Enter choice: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
      printf("Error: Invalid choice\n");
      continue;
    }

    if (sscanf(buffer, "%d", &choice) != 1) {
      printf("Error: Invalid choice\n");
      continue;
    }

    switch (choice) {
    case 1:
      print_all(count, numbers);
      break;
    case 2:
      printf("Total is: %d\n", sum_all(count, numbers));
      break;
    case 3:
      printf("Average is: %.2f\n", average_all(count, numbers));
      break;
    case 4:
      printf("Max is: %d\n", find_max(count, numbers));
      break;
    case 5:
      printf("Min is: %d\n", find_min(count, numbers));
      break;
    case 6:
      printf("Range is: %d\n", find_max(count, numbers) - find_min(count, numbers));
      break;
    case 7:
      print_repeated_values(count, numbers);
      break;
    case 8: {
      int has_mode;
      int mode = find_mode(count, numbers, &has_mode);
      if (has_mode) {
        printf("Mode is: %d\n", mode);
      } else {
        printf("No mode (all values appear once)\n");
      }
      break;
    }
    case 9:
      sort_numbers(count, numbers);
      printf("Sorted values: ");
      print_all(count, numbers);
      break;
    case 10: {
      int *copy = calloc((size_t)count, sizeof(int));
      if (!copy) {
        printf("Error: memory allocation failed\n");
        break;
      }
      for (int i = 0; i < count; i++) {
        copy[i] = numbers[i];
      }
      sort_numbers(count, copy);
      printf("Median is: %.2f\n", median_sorted(count, copy));
      free(copy);
      break;
    }
    case 11:
      printf("Exiting...\n");
      break;
    default:
      printf("Error: Invalid choice\n");
      break;
    }
  } while (choice != 11);

  free(numbers);
  return 0;
}
