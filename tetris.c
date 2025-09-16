#include "tetris.h"

const char *inv_helper(int id)
{
  switch (id)
  {
  case 0:
    return "I";
  case 1:
    return "O";
  case 2:
    return "T";
  case 3:
    return "L";
  case 4:
    return "J";
  case 5:
    return "Z";
  case 6:
    return "S";
  default:
    return "I";
  }
}

int helper(const char *letter)
{
  if (!strcmp(letter, "I"))
  {
    return 0;
  }
  if (!strcmp(letter, "O"))
  {
    return 1;
  }
  if (!strcmp(letter, "T"))
  {
    return 2;
  }
  if (!strcmp(letter, "L"))
  {
    return 3;
  }
  if (!strcmp(letter, "J"))
  {
    return 4;
  }
  if (!strcmp(letter, "Z"))
  {
    return 5;
  }
  if (!strcmp(letter, "S"))
  {
    return 6;
  }
  return 100;
}

int helper_color(int id)
{
  switch (id)
  {
  case 0:
    return RED;
  case 1:
    return BLUE;
  case 2:
    return BROWN;
  case 3:
    return MAGENTA;
  case 4:
    return WHITE;
  case 5:
    return CYAN;
  case 6:
    return GREEN;
  default:
    return WHITE;
  }
}

void draw_grid(SDL_Surface *surface)
{
  SDL_Rect h = {0, 0, WIDTH, LINE_WIDTH};
  for (h.y = 0; h.y < HEIGHT; h.y += CELL_SIZE)
    SDL_FillSurfaceRect(surface, &h, 0x1f1f1f);

  SDL_Rect v = {0, 0, LINE_WIDTH, HEIGHT};
  for (v.x = 0; v.x < WIDTH; v.x += CELL_SIZE)
    SDL_FillSurfaceRect(surface, &v, 0x1f1f1f);
}

bool can_move_down(Piece *p, int cells[COLS][ROWS])
{
  for (int i = 0; i < 4; i++)
  {
    int nx = p->x + p->blocks[i].x;
    int ny = p->y + p->blocks[i].y + 1; // move down by 1
    if (ny >= ROWS)
      return false; // hit bottom
    if (cells[nx][ny])
      return false; // hit existing block
  }
  return true;
}

bool can_move_left(Piece *p, int cells[COLS][ROWS])
{
  for (int i = 0; i < 4; i++)
  {
    int nx = p->x + p->blocks[i].x - 1; // move left
    int ny = p->y + p->blocks[i].y;
    if (nx < 0)
      return false; // hit left wall
    if (cells[nx][ny])
      return false; // hit existing block
  }
  return true;
}

bool can_move_right(Piece *p, int cells[COLS][ROWS])
{
  for (int i = 0; i < 4; i++)
  {
    int nx = p->x + p->blocks[i].x + 1; // move right
    int ny = p->y + p->blocks[i].y;
    if (nx >= COLS)
      return false; // hit right wall
    if (cells[nx][ny])
      return false; // hit existing block
  }
  return true;
}

void draw_cell(SDL_Surface *surface, int x, int y, int cells[COLS][ROWS], Uint32 color)
{
  SDL_Rect cell = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
  SDL_FillSurfaceRect(surface, &cell, color);
}

void fill_cell(int x, int y, int cells[COLS][ROWS], Uint32 color)
{
  cells[x][y] = color;
}

Piece create_I_piece()
{
  Piece p = {COLS / 2, 0, 0, 0, RED, {{-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
  return p;
}

Piece create_O_piece()
{
  Piece p = {COLS / 2, 0, 1, 0, BLUE, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}};
  return p;
}

Piece create_T_piece()
{
  Piece p = {COLS / 2, 0, 2, 0, BROWN, {{-1, 0}, {0, 0}, {1, 0}, {0, 1}}};
  return p;
}

Piece create_L_piece()
{
  Piece p = {COLS / 2, 0, 3, 0, MAGENTA, {{-1, 0}, {0, 0}, {1, 0}, {-1, 1}}};
  return p;
}

Piece create_J_piece()
{
  Piece p = {COLS / 2, 0, 4, 0, WHITE, {{-1, 0}, {0, 0}, {1, 0}, {1, 1}}};
  return p;
}

Piece create_Z_piece()
{
  Piece p = {COLS / 2, 0, 5, 0, CYAN, {{-1, 0}, {0, 0}, {0, 1}, {1, 1}}};
  return p;
}

Piece create_S_piece()
{
  Piece p = {COLS / 2, 0, 6, 0, GREEN, {{0, 0}, {1, 0}, {-1, 1}, {0, 1}}};
  return p;
}

Piece create_piece(const char *letter)
{
  int id = helper(letter);
  switch (id)
  {
  case 0: // I
    return create_I_piece();
  case 1: // O
    return create_O_piece();
  case 2:
    return create_T_piece();
  case 3:
    return create_L_piece();
  case 4:
    return create_J_piece();
  case 5:
    return create_Z_piece();
  case 6:
    return create_S_piece();
  default:
    return create_I_piece();
  }
}

Piece get_random_piece()
{
  int id = SDL_rand(7);
  return create_piece(inv_helper(id));
}

void place_piece(SDL_Surface *surface, Piece *p, int cells[COLS][ROWS])
{
  int color = p->color;
  for (int i = 0; i < 4; i++)
  {
    int x = p->x + p->blocks[i].x;
    int y = p->y + p->blocks[i].y;
    FILL_CELL(x, y);
  }
}

void draw_piece(SDL_Surface *surface, Piece *p, int cells[COLS][ROWS])
{
  int color = p->color;
  for (int i = 0; i < 4; i++)
  {
    int x = p->x + p->blocks[i].x;
    int y = p->y + p->blocks[i].y;
    DRAW_CELL(x, y);
  }
}

void tick(SDL_Surface *surface, Piece *current, int cells[COLS][ROWS])
{
  if (can_move_down(current, cells))
  {
    current->y++;
  }
  else
  {
    PLACE_PIECE(current);
    check_lines(cells);
    *current = get_random_piece();
    // *current = create_piece("I");
  }
}

void draw_cells(SDL_Surface *surface, int cells[COLS][ROWS])
{
  for (int x = 0; x < COLS; x++)
  {
    for (int y = 0; y < ROWS; y++)
    {
      if (cells[x][y])
      {
        // drawing a cell
        SDL_Rect rect = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_FillSurfaceRect(surface, &rect, cells[x][y]);
      }
    }
  }
}

void reset_row(int row, int cells[COLS][ROWS])
{
  for (int y = row; y > 0; y--)
  {
    for (int x = 0; x < COLS; x++)
    {
      cells[x][y] = cells[x][y - 1]; // copy row above down (all rows)
    }
  }
  // clear finally the top row
  for (int x = 0; x < COLS; x++)
    cells[x][0] = 0;
}

bool check_line(int row[COLS])
{
  for (int col = 0; col < COLS; col++)
  {
    if (!row[col])
      return false;
  }
  return true;
}

void check_lines(int cells[COLS][ROWS])
{
  for (int i = 0; i < ROWS; i++)
  {
    int row[COLS] = {0};
    for (int j = 0; j < COLS; j++)
    {
      row[j] = cells[j][i];
    }
    if (check_line(row))
    {
      reset_row(i, cells);
    }
  }
}

void rotate_piece(Piece *p, int cells[COLS][ROWS])
{
  if (p->shape == 1)
    return;

  Block new_blocks[4];

  for (int i = 0; i < 4; i++)
  {
    int x = p->blocks[i].x;
    int y = p->blocks[i].y;

    // clockwise rotation
    new_blocks[i].x = y;
    new_blocks[i].y = -x;
  }

  // Check if new position is valid
  for (int i = 0; i < 4; i++)
  {
    int nx = p->x + new_blocks[i].x;
    int ny = p->y + new_blocks[i].y;
    if (nx < 0 || nx >= COLS || ny < 0 || ny >= ROWS || cells[nx][ny])
    {
      return; // invalid rotation
    }
  }

  // Apply rotation
  for (int i = 0; i < 4; i++)
    p->blocks[i] = new_blocks[i];
}

int main()
{
  SDL_Event event;
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    puts("init video failed");
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Tetris Classic", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);

  SDL_Surface *surface = SDL_GetWindowSurface(window);

  int game = 1;
  int fps = 60;
  bool is_pressed_space = false;
  int cells[COLS][ROWS] = {0};
  // Piece current = create_piece("I");
  Piece current = get_random_piece();

  Uint32 last_tick = SDL_GetTicks();
  Uint32 delay = 150;

  while (game)
  {
    Uint32 now = SDL_GetTicks();
    SDL_ClearSurface(surface, 0, 0, 0, 255);

    // handling events
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_EVENT_QUIT:
        game = 0;
        break;
      case SDL_EVENT_KEY_DOWN:
      {
        switch (event.key.key)
        {
        case SDLK_ESCAPE:
          game = 0;
          break;
        case SDLK_RIGHT:
          if (can_move_right(&current, cells))
            current.x++;
          break;
        case SDLK_LEFT:
          if (can_move_left(&current, cells))
            current.x--;
          break;
        case SDLK_DOWN:
          if (can_move_down(&current, cells))
            current.y++;
          break;
        case SDLK_SPACE:
          if (!is_pressed_space)
          {
            rotate_piece(&current, cells);
          }
          break;
        }
      }
      break;
      case SDL_EVENT_KEY_UP:
      {
        switch (event.key.key)
        {
        case SDLK_SPACE:
          if (is_pressed_space)
          {
            is_pressed_space = false;
          }
          break;
        default:
          break;
        }
      }
      default:
        break;
      }
    }

    if (now - last_tick > delay)
    {
      last_tick = now;
      tick(surface, &current, cells);
    }

    draw_cells(surface, cells);
    draw_piece(surface, &current, cells);

    DRAW_GRID;

    SDL_UpdateWindowSurface(window);
  }

  SDL_Quit();
  return 0;
}