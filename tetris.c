#include "tetris.h"

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
  switch (p->shape)
  {
  case 0: // I
    printf("spawned I\n");
    for (int i = 0; i < 4; i++)
    {
      int nx = p->x + i;
      int ny = p->y + 1;
      if (ny >= ROWS || cells[nx][ny])
        return false;
    }
    return true;
  case 1: // O
    int ny = p->y + 2;
    printf("spawned O\n");
    if (ny >= ROWS || cells[p->x][ny] || cells[p->x + 1][ny])
      return false;
    return true;
  case 2: // T
    printf("spawned T\n");
    if (p->y + 1 >= ROWS || cells[p->x][p->y + 1])
      return false;
    if (p->y + 2 >= ROWS || cells[p->x + 1][p->y + 2])
      return false;
    if (cells[p->x + 2][p->y + 1])
      return false;
    return true;
  case 3: // L
    printf("spawned L\n");
    if (p->y + 2 >= ROWS || cells[p->x][p->y + 2])
      return false;
    if (p->y + 1 >= ROWS || cells[p->x + 1][p->y + 1])
      return false;
    if (cells[p->x + 2][p->y + 1])
      return false;
    return true;
  case 4:
    printf("spawned J\n");
    if (p->y + 1 >= ROWS || cells[p->x][p->y + 1])
      return false;
    if (cells[p->x + 1][p->y + 1])
      return false;
    if (p->y + 2 >= ROWS || cells[p->x + 2][p->y + 2])
      return false;
    return true;
  case 5:
    printf("spawned Z\n");
    if (p->y + 1 >= ROWS || cells[p->x][p->y + 1])
      return false;
    if (cells[p->x + 1][p->y + 2])
      return false;
    if (p->y + 2 >= ROWS || cells[p->x + 2][p->y + 1])
      return false;
    return true;
  case 6:
    printf("spawned S\n");
    if (p->y + 2 >= ROWS || cells[p->x][p->y + 2])
      return false;
    if (cells[p->x + 1][p->y + 2])
      return false;
    if (p->y + 1 >= ROWS || cells[p->x + 2][p->y + 1])
      return false;
    return true;
  default:
    return false;
  }
}

void draw_cell(SDL_Surface *surface, int x, int y, int cells[COLS][ROWS])
{
  SDL_Rect cell = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
  SDL_FillSurfaceRect(surface, &cell, 0xffffff);
}

void fill_cell(int x, int y, int cells[COLS][ROWS])
{
  cells[x][y] = 1;
}

Piece create_piece(const char *letter)
{
  Piece p = {COLS / 2, 0, helper(letter), 0};
  return p;
}

Piece get_random_piece()
{
  Piece p = {COLS / 2, 0, SDL_rand(6), 0};
  return p;
}

void place_piece(SDL_Surface *surface, Piece *p, int cells[COLS][ROWS])
{
  int x = p->x;
  int y = p->y;
  switch (p->shape)
  {
  case 0:
    for (int i = 0; i < 4; i++)
    {
      FILL_CELL(x + i, y);
    }
    printf("spawned I\n");
    break;
  case 1:
    FILL_CELL(x, y);
    FILL_CELL(x, y + 1);
    FILL_CELL(x + 1, y);
    FILL_CELL(x + 1, y + 1);
    printf("spawned O\n");
    break;
  case 2:
    FILL_CELL(x, y);
    FILL_CELL(x + 1, y);
    FILL_CELL(x + 2, y);
    FILL_CELL(x + 1, y + 1);
    printf("spawned T\n");
    break;
  case 3:
    FILL_CELL(x, y);
    FILL_CELL(x + 1, y);
    FILL_CELL(x + 2, y);
    FILL_CELL(x, y + 1);
    printf("spawned L\n");
    break;
  case 4:
    FILL_CELL(x, y);
    FILL_CELL(x + 1, y);
    FILL_CELL(x + 2, y);
    FILL_CELL(x + 2, y + 1);
    printf("spawned J\n");
    break;
  case 5:
    FILL_CELL(x, y);
    FILL_CELL(x + 1, y);
    FILL_CELL(x + 1, y + 1);
    FILL_CELL(x + 2, y + 1);
    printf("spawned Z\n");
    break;
  case 6:
    FILL_CELL(x + 1, y);
    FILL_CELL(x + 2, y);
    FILL_CELL(x, y + 1);
    FILL_CELL(x + 1, y + 1);
    printf("spawned S\n");
    break;
  default:
    break;
  }
}

void draw_piece(SDL_Surface *surface, Piece *p, int cells[COLS][ROWS])
{
  int x = p->x;
  int y = p->y;
  switch (p->shape)
  {
  case 0:
    for (int i = 0; i < 4; i++)
    {
      DRAW_CELL(x + i, y);
    }
    // printf("drawing I\n");
    break;
  case 1:
    DRAW_CELL(x, y);
    DRAW_CELL(x, y + 1);
    DRAW_CELL(x + 1, y);
    DRAW_CELL(x + 1, y + 1);
    // printf("drawing O\n");
    break;
  case 2:
    DRAW_CELL(x, y);
    DRAW_CELL(x + 1, y);
    DRAW_CELL(x + 2, y);
    DRAW_CELL(x + 1, y + 1);
    // printf("drawing T\n");
    break;
  case 3:
    DRAW_CELL(x, y);
    DRAW_CELL(x + 1, y);
    DRAW_CELL(x + 2, y);
    DRAW_CELL(x, y + 1);
    // printf("drawing L\n");
    break;
  case 4:
    DRAW_CELL(x, y);
    DRAW_CELL(x + 1, y);
    DRAW_CELL(x + 2, y);
    DRAW_CELL(x + 2, y + 1);
    // printf("drawing J\n");
    break;
  case 5:
    DRAW_CELL(x, y);
    DRAW_CELL(x + 1, y);
    DRAW_CELL(x + 1, y + 1);
    DRAW_CELL(x + 2, y + 1);
    // printf("drawing Z\n");
    break;
  case 6:
    DRAW_CELL(x + 1, y);
    DRAW_CELL(x + 2, y);
    DRAW_CELL(x, y + 1);
    DRAW_CELL(x + 1, y + 1);
    // printf("drawing S\n");
    break;
  default:
    break;
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
    // *current = get_random_piece();
    *current = create_piece("I");
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
        SDL_FillSurfaceRect(surface, &rect, 0xffffff);
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
  int cells[COLS][ROWS] = {0};
  Piece current = create_piece("I");

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
          int size = current.shape;
          if (current.shape == 0)
            size = 4;
          if (current.shape == 1)
            size = 2;
          if (current.shape > 1)
            size = 3;
          if (current.x + size < COLS)
            current.x++;
          break;
        case SDLK_LEFT:
          if (current.x - 1 >= 0)
            current.x--;
          break;
        case SDLK_DOWN:
          if (can_move_down(&current, cells))
            current.y++;
          break;
        }
      }
      break;
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