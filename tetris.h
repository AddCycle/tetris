#pragma once

#include <stdio.h>
#include <string.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#define RED 0xff0000
#define BLUE 0x0000ff
#define BROWN 0x964b00
#define MAGENTA 0xff00ff
#define WHITE 0xffffff
#define CYAN 0x00ffff
#define GREEN 0x00ff00

#define LINE_WIDTH 2

#define WIDTH 600
#define HEIGHT 900
#define CELL_SIZE 30

#define ROWS (HEIGHT / CELL_SIZE)
#define COLS (WIDTH / CELL_SIZE)

#define DRAW_GRID draw_grid(surface)
#define FILL_CELL(x, y) fill_cell(x, y, cells, color)
#define DRAW_CELL(x, y) draw_cell(surface, x, y, cells, color)
#define PLACE_PIECE(piece) place_piece(surface, piece, cells)

typedef struct
{
  int score;
} GameState;

typedef struct
{
  int x, y; // offsets from pivot
} Block;

typedef struct
{
  int x, y;     // position
  int shape;    // id: 0-6 IOTLJZS
  int rotation; // 0-3
  int color;
  Block blocks[4]; // offsets from pivot
} Piece;

int helper(const char *letter);

void draw_grid(SDL_Surface *surface);

bool can_move_down(Piece *p, int cells[COLS][ROWS]);
void draw_cell(SDL_Surface *surface, int x, int y, int cells[COLS][ROWS], Uint32 color);
void fill_cell(int x, int y, int cells[COLS][ROWS], Uint32 color);
Piece create_piece(const char *letter);
Piece get_random_piece();
void place_piece(SDL_Surface *surface, Piece *p, int cells[COLS][ROWS]);
void draw_piece(SDL_Surface *surface, Piece *p, int cells[COLS][ROWS]);
void tick(SDL_Surface *surface, Piece *current, int cells[COLS][ROWS], GameState *gamestate);
void draw_cells(SDL_Surface *surface, int cells[COLS][ROWS]);
void reset_row(int row, int cells[COLS][ROWS], GameState *gamestate);
bool check_line(int row[COLS]);
void check_lines(int cells[COLS][ROWS], GameState *gamestate);