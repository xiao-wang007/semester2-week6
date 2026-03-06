#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 21 // 21x21 grid for -10 to +10 range

// Function prototypes
void draw_grid(char **grid);
char **initialize_grid(void);
void add_point(char **grid, int x, int y);
void move_point(int *x, int *y, int dx, int dy);
void reflect_point(int *x, int *y, char axis);
void swap_coords(int *x, int *y);

int main(void) {
  // this creates a 20 x 20 grid (-10 -> +10, plus 1 row & column used for axes.)
  char **grid = initialize_grid();
  int x = -7, y = 3;

  // you can add a single point like this:
  add_point(grid, x, y);
  draw_grid(grid);

  // if you want to 'reset' and remove things, you can re-init it to clear
  grid = initialize_grid();
  draw_grid(grid);

  return 0;
}

/**
 * @brief Move a point by the provided delta
 *
 * @param x the x coordinate
 * @param y the y coordinate
 * @param dx the x delta
 * @param dy the y delta
 */
void move_point(int *x, int *y, int dx, int dy) {
  // update the coordinates by the given delta
  // i.e. (x+dx, y+dy)
  *x += dx;
  *y += dy;
}

/**
 * @brief Reflects a point across an axis
 *
 * @param x the x coordinate
 * @param y the y coordinate
 * @param axis the axis (either x or y) to reflect across
 */
void reflect_point(int *x, int *y, char axis) {
  // reflect the point across the given axis
  // e.g. reflect (7,3) across the X axis -> (7,-3)
  // across the y axis -> (-7, 3)
  if (axis == 'x' || axis == 'X') {
    *y = -*y;
  } else if (axis == 'y' || axis == 'Y') {
    *x = -*x;
  }
}

/**
 * @brief Swaps the x and y coordinates in a pair
 *
 * @param x the x coordinate
 * @param y the y coordinate
 */
void swap_coords(int *x, int *y) {
  // swap the x and y values of a coordinate
  int temp = *x;
  *x = *y;
  *y = temp;
}

/**
 * @brief Draws the grid
 *
 * This function prints the grid to the console, displaying the axis and any other modifications (such as points or shapes).
 *
 * @param grid The 2D array representing the grid to be printed.
 */
void draw_grid(char **grid) {
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++)
      printf("%c ", grid[i][j]);
    printf("\n");
  }
  printf("\n");
}

/**
 * @brief Adds a point to the grid at the specified coordinates.
 *
 *
 * @param grid The 2D array representing the grid.
 * @param x The x-coordinate of the point to be added.
 * @param y The y-coordinate of the point to be added.
 * @return char** The updated grid with the point added.
 */
void add_point(char **grid, int x, int y) {
  // Adjust coordinates for grid's origin (10, 10)
  int plot_x = x + 10; // Offset the x-coordinate by 10 to fit grid range
  int plot_y = 10 - y;
  // Offset the y-coordinate by 10 and invert it for grid display

  // Check bounds to make sure point stays within grid
  if (plot_x >= 0 && plot_x < GRID_SIZE && plot_y >= 0 && plot_y < GRID_SIZE) {
    grid[plot_y][plot_x] = '*'; // Place the point on the grid
  }
}

/**
 * @brief Initializes the grid with axes.
 *
 * This function sets up the grid by adding the X and Y axes at the appropriate locations.
 * The grid is filled with spaces, and axes are represented by '|' for the Y-axis and '-' for the X-axis.
 *
 * @return char** The initialized 2D array representing the grid.
 */
char **initialize_grid(void) {
  char **grid = (char **)malloc(GRID_SIZE * sizeof(char *));
  for (int i = 0; i < GRID_SIZE; i++) {
    grid[i] = (char *)malloc(GRID_SIZE * sizeof(char));
  }

  // Fill with spaces
  for (int i = 0; i < GRID_SIZE; i++)
    for (int j = 0; j < GRID_SIZE; j++)
      grid[i][j] = ' ';

  // Draw x and y axes
  for (int i = 0; i < GRID_SIZE; i++) {
    grid[i][10] = '|'; // Y-axis
    grid[10][i] = '-'; // X-axis
  }

  grid[10][10] = '+'; // 0,0

  return grid;
}
