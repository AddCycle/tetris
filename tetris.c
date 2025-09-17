#include "tetris.h"

void draw_text(SDL_Surface *target, TTF_Font *font, int x, int y, SDL_Color color, const char *fmt, ...)
{
  // Render to a surface
  char buffer[256]; // posix paths max
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  SDL_Surface *text_surf = TTF_RenderText_Blended(font, buffer, 0, color);
  if (!text_surf)
  {
    printf("Rendering text: %s\n", SDL_GetError());
    return;
  }

  SDL_Rect dest = {x, y, text_surf->w, text_surf->h};
  SDL_BlitSurface(text_surf, NULL, target, &dest);

  SDL_DestroySurface(text_surf); // cleanup temp surface
}

void write_high_score(int score)
{
  FILE *file = fopen("high_score.txt", "w");

  fprintf(file, "%d", score);

  fclose(file);
}

int read_high_score()
{
  FILE *file = fopen("high_score.txt", "r");
  if (!file)
  {
    write_high_score(0);
    return 0;
  }
  int score = 0;

  fscanf(file, "%d", &score);

  fclose(file);

  return score;
}

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

void draw_next_piece(SDL_Surface *surface, Piece *p, int cells[COLS][ROWS])
{
  int color = p->color;
  for (int i = 0; i < 4; i++)
  {
    int x = p->x + p->blocks[i].x + 12;
    int y = p->y + p->blocks[i].y;
    DRAW_CELL(x, y);
  }
}

void tick(SDL_Surface *surface, Piece *current, Piece *next, int cells[COLS][ROWS], GameState *gamestate)
{
  if (can_move_down(current, cells))
  {
    current->y++;
  }
  else
  {
    PLACE_PIECE(current);
    check_lines(cells, gamestate);
    *current = *next;
    *next = get_random_piece();
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

void reset_row(int row, int cells[COLS][ROWS], GameState *gamestate)
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

  gamestate->score++; // incrementing the score
  printf("Score: %d\n", gamestate->score);
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

void check_lines(int cells[COLS][ROWS], GameState *gamestate)
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
      reset_row(i, cells, gamestate);
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

// the main menu of the game
int main_menu(SDL_Window *window, SDL_Surface *surface, int width, int height)
{
  SDL_Event event;
  int choice = 0; // 1 = Play, 2 = Quit
  int running = 1;
  TTF_Font *font = TTF_OpenFont("PressStart2P.ttf", 20);
  if (!font)
  {
    SDL_Log("Failed to load font: %s", SDL_GetError());
    return 2;
  }

  SDL_Color white = {255, 255, 255, 255};
  SDL_Color black = {0, 0, 0, 255};

  while (running)
  {
    // Clear screen
    SDL_ClearSurface(surface, 0, 0, 0, 0);

    SDL_Surface *text_surface = TTF_RenderText_Blended(font, "SNAKE CLASSIC", 0, white);
    if (text_surface)
    {
      SDL_Rect text_rect;
      text_rect.x = width / 2 - text_surface->w / 2;
      text_rect.y = height / 4 - text_surface->h / 2;
      SDL_BlitSurface(text_surface, NULL, surface, &text_rect);
      SDL_DestroySurface(text_surface);
    }

    // Draw simple rectangles as buttons with outlines
    SDL_Rect outline_rect_play = {width / 2 - 110, height / 2 - 55, 220, 60};
    SDL_Rect play_rect = {width / 2 - 100, height / 2 - 50, 200, 50};

    SDL_Rect outline_rect_quit = {width / 2 - 110, height / 2 - 30 + play_rect.h, 220, 60};
    SDL_Rect quit_rect = {width / 2 - 100, height / 2 - 25 + play_rect.h, 200, 50};

    if (choice == 1)
      SDL_FillSurfaceRect(surface, &outline_rect_play, 0xffffff);
    SDL_FillSurfaceRect(surface, &play_rect, 0x00ff00); // green
    if (choice == 2)
      SDL_FillSurfaceRect(surface, &outline_rect_quit, 0xffffff);
    SDL_FillSurfaceRect(surface, &quit_rect, 0xff0000); // red

    SDL_Surface *play_text_surface = TTF_RenderText_Blended(font, "PLAY", 0, black);
    if (play_text_surface)
    {
      SDL_Rect text_rect;
      text_rect.x = width / 2 - play_text_surface->w / 2;
      text_rect.y = height / 4 - play_text_surface->h / 2 + 200;
      SDL_BlitSurface(play_text_surface, NULL, surface, &text_rect);
      SDL_DestroySurface(play_text_surface);
    }

    SDL_Surface *quit_text_surface = TTF_RenderText_Blended(font, "EXIT", 0, black);
    if (quit_text_surface)
    {
      SDL_Rect text_rect;
      text_rect.x = width / 2 - quit_text_surface->w / 2;
      text_rect.y = height / 4 - quit_text_surface->h / 2 + 275;
      SDL_BlitSurface(quit_text_surface, NULL, surface, &text_rect);
      SDL_DestroySurface(quit_text_surface);
    }

    SDL_UpdateWindowSurface(window);

    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_EVENT_QUIT)
      {
        TTF_CloseFont(font);
        return 2; // quit
      }
      if (event.type == SDL_EVENT_KEY_DOWN)
      {
        if (event.key.key == SDLK_UP || event.key.key == SDLK_W)
          choice = 1; // Play selected
        if (event.key.key == SDLK_DOWN || event.key.key == SDLK_S)
          choice = 2; // Quit selected
        if (event.key.key == SDLK_RETURN || event.key.key == SDLK_KP_ENTER || event.key.key == SDLK_SPACE)
        {
          TTF_CloseFont(font);
          return choice; // return selection
        }
        if (event.key.key == SDLK_ESCAPE)
        {
          TTF_CloseFont(font);
          return 2; // quit immediately
        }
      }
      if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
      {
        SDL_MouseButtonEvent mb = event.button;
        if (mb.button == SDL_BUTTON_LEFT)
        {
          // button 1 box : play
          if (mb.x >= width / 2 - 100 && mb.x <= (width / 2 - 100) + 220 && mb.y >= height / 2 - 50 && mb.y <= height / 2)
          {
            SDL_Log("play");
            return 1;
          }

          // button 2 box : quit
          if (mb.x >= width / 2 - 100 && mb.x <= (width / 2 - 100) + 220 && mb.y >= height / 2 - 25 + play_rect.h && mb.y <= height / 2 + 25 + play_rect.h)
          {
            SDL_Log("quit");
            return 2; // quit
          }
        }
      }
      if (event.type == SDL_EVENT_MOUSE_MOTION)
      {
        int x = event.motion.x;
        int y = event.motion.y;
        // button 1 box : play (hover)
        if (x >= width / 2 - 100 && x <= (width / 2 - 100) + 220 && y >= height / 2 - 50 && y <= height / 2)
        {
          choice = 1;
        }

        // button 2 box : quit (hover)
        if (x >= width / 2 - 100 && x <= (width / 2 - 100) + 220 && y >= height / 2 - 25 + play_rect.h && y <= height / 2 + 25 + play_rect.h)
        {
          choice = 2;
        }
      }
    }
  }
  TTF_CloseFont(font);
  return 2; // default quit
}

int main()
{
  SDL_Event event;
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    puts("init video failed");
    return 1;
  }

  if (!TTF_Init())
  {
    puts("failed to init text rendering");
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Tetris Classic", WIDTH + LEFT_PANNEL_WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);

  SDL_Surface *surface = SDL_GetWindowSurface(window);

  TTF_Font *font = TTF_OpenFont("PressStart2P.ttf", 20);

  GameState gamestate = {0};
  int game = 1;
  // main menu play/quit
  int menu_choice = main_menu(window, surface, WIDTH + LEFT_PANNEL_WIDTH, HEIGHT);
  if (menu_choice == 2) // Quit selected
  {
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
  }
  int fps = 60;
  bool is_pressed_space = false;
  bool is_pressed_up = false;
  bool is_pressed_f = false;
  bool is_window_maxed = false;
  int cells[COLS][ROWS] = {0};
  // Piece current = create_piece("I");
  Piece current = get_random_piece();
  Piece next_piece = get_random_piece();

  Uint32 last_tick = SDL_GetTicks();
  Uint32 delay = 150;

  int score = 0;
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
        case SDLK_UP:
          if (!is_pressed_up)
          {
            rotate_piece(&current, cells);
          }
          break;
        case SDLK_F:
          if (!is_pressed_f)
          {
            if (!is_window_maxed)
            {
              SDL_MaximizeWindow(window);
              surface = SDL_GetWindowSurface(window);
              SDL_UpdateWindowSurface(window);
              is_window_maxed = true;
            }
            else
            {
              SDL_RestoreWindow(window);
              surface = SDL_GetWindowSurface(window);
              SDL_UpdateWindowSurface(window);
              is_window_maxed = false;
            }
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
        case SDLK_UP:
          if (is_pressed_up)
          {
            is_pressed_up = false;
          }
          break;
        case SDLK_F:
          if (is_pressed_f)
          {
            is_pressed_f = false;
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
      tick(surface, &current, &next_piece, cells, &gamestate);
    }

    draw_cells(surface, cells);
    draw_next_piece(surface, &next_piece, cells);
    draw_piece(surface, &current, cells);
    SDL_Color white = {255, 255, 255, 255};

    DRAW_GRID;

    draw_text(surface, font, WIDTH / 4, HEIGHT / 4 - 50, white, "Score : %d", gamestate.score);
    draw_text(surface, font, WIDTH / 4, HEIGHT / 4, white, "Highscore : %d", read_high_score());

    SDL_UpdateWindowSurface(window);
  }

  if (gamestate.score > read_high_score())
  {
    write_high_score(gamestate.score);
  }

  TTF_CloseFont(font);
  TTF_Quit();

  SDL_Quit();
  return 0;
}