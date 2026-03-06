#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LENGTH 256

int getUserInput(const char *message);
int getTextInput(const char *message, char *buffer, size_t bufferSize);
void menu(void);
unsigned char **allocateImage(int height, int width);
void freeImage(unsigned char **image, int height);
unsigned char **readImage(const char *filePath, int *height, int *width, int *maxGray);
int writeImage(const char *filePath, unsigned char **image, int height, int width,
               int maxGray);
void printImage(unsigned char **image, int height, int width);
unsigned char **invertImage(unsigned char **image, int height, int width,
                            int maxGray);
unsigned char **rotateImage(unsigned char **image, int height, int width,
                            int degrees, int *rotatedHeight,
                            int *rotatedWidth);
unsigned char **scaleImage(unsigned char **image, int height, int width, int factor,
                           int *scaledHeight, int *scaledWidth);

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: ./pgmTools image_path\n");
    return 1;
  }

  int imageHeight = 0;
  int imageWidth = 0;
  int maxGray = 0;
  unsigned char **image = readImage(argv[1], &imageHeight, &imageWidth, &maxGray);
  if (image == NULL) {
    return 1;
  }

  while (1) {
    int choice = -1;
    menu();
    while (choice < 1) {
      choice = getUserInput("Enter choice");
      if (choice < 1) {
        printf("Please enter a valid menu option.\n");
      }
    }

    if (choice == 1) {
      printImage(image, imageHeight, imageWidth);
      continue;
    }

    if (choice == 2) {
      char outputPath[MAX_PATH_LENGTH];
      unsigned char **inverted = invertImage(image, imageHeight, imageWidth, maxGray);
      if (inverted == NULL) {
        printf("Failed to invert image.\n");
        continue;
      }

      if (!getTextInput("Enter output file path", outputPath,
                        sizeof(outputPath))) {
        printf("No output path provided.\n");
        freeImage(inverted, imageHeight);
        continue;
      }

      if (writeImage(outputPath, inverted, imageHeight, imageWidth, maxGray)) {
        printf("Inverted image saved to %s\n", outputPath);
      }

      freeImage(inverted, imageHeight);
      continue;
    }

    if (choice == 3) {
      int degrees = getUserInput("Rotate by degrees (90, 180, 270)");
      if (degrees != 90 && degrees != 180 && degrees != 270) {
        printf("Invalid rotation. Please choose 90, 180 or 270.\n");
        continue;
      }

      int rotatedHeight = 0;
      int rotatedWidth = 0;
      unsigned char **rotated = rotateImage(image, imageHeight, imageWidth, degrees,
                                            &rotatedHeight, &rotatedWidth);
      if (rotated == NULL) {
        printf("Failed to rotate image.\n");
        continue;
      }

      char outputPath[MAX_PATH_LENGTH];
      if (!getTextInput("Enter output file path", outputPath,
                        sizeof(outputPath))) {
        printf("No output path provided.\n");
        freeImage(rotated, rotatedHeight);
        continue;
      }

      if (writeImage(outputPath, rotated, rotatedHeight, rotatedWidth, maxGray)) {
        printf("Rotated image saved to %s\n", outputPath);
      }

      freeImage(rotated, rotatedHeight);
      continue;
    }

    if (choice == 4) {
      int factor = getUserInput("Scale down by factor (> 0)");
      if (factor <= 0) {
        printf("Scale factor must be greater than 0.\n");
        continue;
      }

      int scaledHeight = 0;
      int scaledWidth = 0;
      unsigned char **scaled =
          scaleImage(image, imageHeight, imageWidth, factor, &scaledHeight,
                     &scaledWidth);
      if (scaled == NULL) {
        printf("Failed to scale image. Ensure factor fits image dimensions.\n");
        continue;
      }

      char outputPath[MAX_PATH_LENGTH];
      if (!getTextInput("Enter output file path", outputPath,
                        sizeof(outputPath))) {
        printf("No output path provided.\n");
        freeImage(scaled, scaledHeight);
        continue;
      }

      if (writeImage(outputPath, scaled, scaledHeight, scaledWidth, maxGray)) {
        printf("Scaled image saved to %s\n", outputPath);
      }

      freeImage(scaled, scaledHeight);
      continue;
    }

    if (choice == 5) {
      break;
    }

    printf("Bad choice\n");
  }

  freeImage(image, imageHeight);
  return 0;
}

int getUserInput(const char *message) {
  char buffer[100];
  int choice;

  printf("%s: ", message);
  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    return -1;
  }

  if (sscanf(buffer, "%d", &choice) != 1) {
    return -1;
  }

  return choice;
}

int getTextInput(const char *message, char *buffer, size_t bufferSize) {
  size_t length;

  printf("%s: ", message);
  if (fgets(buffer, bufferSize, stdin) == NULL) {
    return 0;
  }

  length = strlen(buffer);
  if (length > 0 && buffer[length - 1] == '\n') {
    buffer[length - 1] = '\0';
  }

  return buffer[0] != '\0';
}

void menu(void) {
  printf("1 - View PGM Image\n");
  printf("2 - Invert Image\n");
  printf("3 - Rotate Image\n");
  printf("4 - Scale Image\n");
  printf("5 - Quit\n");
}

unsigned char **allocateImage(int height, int width) {
  unsigned char **image = calloc((size_t)height, sizeof(unsigned char *));
  if (image == NULL) {
    return NULL;
  }

  for (int row = 0; row < height; row++) {
    image[row] = calloc((size_t)width, sizeof(unsigned char));
    if (image[row] == NULL) {
      for (int previous = 0; previous < row; previous++) {
        free(image[previous]);
      }
      free(image);
      return NULL;
    }
  }

  return image;
}

void freeImage(unsigned char **image, int height) {
  if (image == NULL) {
    return;
  }

  for (int row = 0; row < height; row++) {
    free(image[row]);
  }
  free(image);
}

unsigned char **readImage(const char *filePath, int *height, int *width,
                          int *maxGray) {
  FILE *file = fopen(filePath, "r");
  char magicNumber[3] = {0};
  int pixelValue = 0;

  if (file == NULL) {
    printf("Error: could not open file '%s'\n", filePath);
    return NULL;
  }

  if (fscanf(file, "%2s", magicNumber) != 1 || strcmp(magicNumber, "P2") != 0) {
    printf("Error: invalid PGM format. Expected magic number P2.\n");
    fclose(file);
    return NULL;
  }

  if (fscanf(file, "%d %d", height, width) != 2) {
    printf("Error: missing image dimensions.\n");
    fclose(file);
    return NULL;
  }

  if (*height <= 0 || *width <= 0) {
    printf("Error: image dimensions must be positive.\n");
    fclose(file);
    return NULL;
  }

  if (fscanf(file, "%d", maxGray) != 1) {
    printf("Error: missing max gray value.\n");
    fclose(file);
    return NULL;
  }

  if (*maxGray < 0 || *maxGray > 255) {
    printf("Error: max gray value must be between 0 and 255.\n");
    fclose(file);
    return NULL;
  }

  unsigned char **image = allocateImage(*height, *width);
  if (image == NULL) {
    printf("Error: out of memory while loading image.\n");
    fclose(file);
    return NULL;
  }

  for (int row = 0; row < *height; row++) {
    for (int col = 0; col < *width; col++) {
      if (fscanf(file, "%d", &pixelValue) != 1) {
        printf("Error: missing pixel data.\n");
        freeImage(image, *height);
        fclose(file);
        return NULL;
      }

      if (pixelValue < 0 || pixelValue > *maxGray) {
        printf("Error: pixel value %d is out of range (0-%d).\n", pixelValue,
               *maxGray);
        freeImage(image, *height);
        fclose(file);
        return NULL;
      }

      image[row][col] = (unsigned char)pixelValue;
    }
  }

  if (fscanf(file, "%d", &pixelValue) == 1) {
    printf("Error: file contains too many pixel values.\n");
    freeImage(image, *height);
    fclose(file);
    return NULL;
  }

  fclose(file);
  return image;
}

int writeImage(const char *filePath, unsigned char **image, int height, int width,
               int maxGray) {
  FILE *file = fopen(filePath, "w");
  if (file == NULL) {
    printf("Error: could not write file '%s'\n", filePath);
    return 0;
  }

  fprintf(file, "P2\n");
  fprintf(file, "%d %d\n", height, width);
  fprintf(file, "%d\n", maxGray);

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      fprintf(file, "%u ", image[row][col]);
    }
    fprintf(file, "\n");
  }

  fclose(file);
  return 1;
}

void printImage(unsigned char **image, int height, int width) {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      printf("%d%s", image[row][col],
             (image[row][col] < 100) ? (image[row][col] < 10) ? "   " : "  " : " ");
    }
    printf("\n");
  }
}

unsigned char **invertImage(unsigned char **image, int height, int width,
                            int maxGray) {
  unsigned char **inverted = allocateImage(height, width);
  if (inverted == NULL) {
    return NULL;
  }

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      inverted[row][col] = (unsigned char)(maxGray - image[row][col]);
    }
  }

  return inverted;
}

unsigned char **rotateImage(unsigned char **image, int height, int width,
                            int degrees, int *rotatedHeight,
                            int *rotatedWidth) {
  unsigned char **rotated = NULL;

  if (degrees == 180) {
    *rotatedHeight = height;
    *rotatedWidth = width;
  } else {
    *rotatedHeight = width;
    *rotatedWidth = height;
  }

  rotated = allocateImage(*rotatedHeight, *rotatedWidth);
  if (rotated == NULL) {
    return NULL;
  }

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      if (degrees == 90) {
        rotated[col][row] = image[row][col];
      } else if (degrees == 180) {
        rotated[height - row - 1][width - col - 1] = image[row][col];
      } else if (degrees == 270) {
        rotated[width - col - 1][height - row - 1] = image[row][col];
      }
    }
  }

  return rotated;
}

unsigned char **scaleImage(unsigned char **image, int height, int width, int factor,
                           int *scaledHeight, int *scaledWidth) {
  if (factor <= 0) {
    return NULL;
  }

  *scaledHeight = height / factor;
  *scaledWidth = width / factor;

  if (*scaledHeight <= 0 || *scaledWidth <= 0) {
    return NULL;
  }

  unsigned char **scaled = allocateImage(*scaledHeight, *scaledWidth);
  if (scaled == NULL) {
    return NULL;
  }

  for (int row = 0; row < *scaledHeight; row++) {
    for (int col = 0; col < *scaledWidth; col++) {
      unsigned int sum = 0;
      int startRow = row * factor;
      int startCol = col * factor;

      for (int blockRow = 0; blockRow < factor; blockRow++) {
        for (int blockCol = 0; blockCol < factor; blockCol++) {
          sum += image[startRow + blockRow][startCol + blockCol];
        }
      }

      scaled[row][col] = (unsigned char)(sum / (unsigned int)(factor * factor));
    }
  }

  return scaled;
}
