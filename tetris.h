#pragma once

#include <stdio.h>
#include <string.h>
#include <SDL3/SDL.h>

#define LINE_WIDTH 2

#define WIDTH 600
#define HEIGHT 900
#define CELL_SIZE 20

#define ROWS (HEIGHT / CELL_SIZE)
#define COLS (WIDTH / CELL_SIZE)

#define DRAW_GRID draw_grid(surface)
#define FILL_CELL(x, y) fill_cell(x, y, cells)
#define DRAW_CELL(x, y) draw_cell(surface, x, y, cells)
#define PLACE_PIECE(piece) place_piece(surface, piece, cells)

typedef struct
{
  int x, y;     // position
  int shape;    // id: 0-6 IOTLJZS
  int rotation; // 0-3
} Piece;
