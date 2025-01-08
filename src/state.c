#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state()
{
  // TODO: Implement this function.
  game_state_t *state = (game_state_t *)malloc(sizeof(struct game_state_t)); // pointer to game state
  state->num_rows = 18;
  char **board = (char **)malloc(state->num_rows * sizeof(char *));

  for (unsigned int r = 0; r < state->num_rows; r++)
  {
    char *row = malloc(21 * sizeof(char)); // 21 including null terminator
    for (unsigned int c = 0; c < 21; c++)
    {
      if (r == 0 || r == 17 || c == 0 || c == 19)
      {
        row[c] = '#';
      }
      else
      {
        row[c] = ' ';
      }
      if (c == 20)
      {
        row[c] = '\0';
      }
      if (r == 2 && c == 2)
      {
        row[c] = 'd';
      }
      if (r == 2 && c == 3)
      {
        row[c] = '>';
      }
      if (r == 2 && c == 4)
      {
        row[c] = 'D';
      }
      if (r == 2 && c == 9)
      {
        row[c] = '*';
      }
    }
    board[r] = row;
  }
  state->board = board;

  state->num_snakes = 1;
  struct snake_t *snakes = malloc(1 * sizeof(struct snake_t)); // pointer to snake

  snake_t snake;
  snake.tail_row = 2;
  snake.tail_col = 2;
  snake.head_row = 2;
  snake.head_col = 4;
  snake.live = true;

  snakes[0] = snake;

  state->snakes = snakes;

  return state;
}

/* Task 2 */
void free_state(game_state_t *state)
{
  // TODO: Implement this function.
  for (unsigned int i = 0; i < state->num_rows; i++)
  {
    free(state->board[i]);
  }

  free(state->board);
  free(state->snakes);
  free(state);

  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp)
{
  // TODO: Implement this function.
  for (unsigned int i = 0; i < state->num_rows; i++)
  {
    fprintf(fp, "%s\n", (char *)state->board[i]);
  }

  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename)
{
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col)
{
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch)
{
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c)
{
  // TODO: Implement this function.
  return (c == 'w' || c == 'a' || c == 's' || c == 'd');
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c)
{
  // TODO: Implement this function.
  return (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x');
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c)
{
  // TODO: Implement this function.
  return (is_tail(c) || is_head(c) || c == '^' || c == '<' || c == '>' || c == 'v');
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c)
{
  // TODO: Implement this function.
  if (c == '^')
  {
    return 'w';
  }
  else if (c == '<')
  {
    return 'a';
  }
  else if (c == 'v')
  {
    return 's';
  }
  else if (c == '>')
  {
    return 'd';
  }
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c)
{
  // TODO: Implement this function.
  if (c == 'W')
  {
    return '^';
  }
  else if (c == 'A')
  {
    return '<';
  }
  else if (c == 'S')
  {
    return 'v';
  }
  else if (c == 'D')
  {
    return '>';
  }
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c)
{
  // TODO: Implement this function.
  if (c == 'v' || c == 's' || c == 'S')
  {
    return cur_row + 1;
  }
  else if (c == '^' || c == 'w' || c == 'W')
  {
    return cur_row - 1;
  }
  else
  {
    return cur_row;
  }
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c)
{
  // TODO: Implement this function.
  if (c == '>' || c == 'd' || c == 'D')
  {
    return cur_col + 1;
  }
  else if (c == '<' || c == 'a' || c == 'A')
  {
    return cur_col - 1;
  }
  else
  {
    return cur_col;
  }
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum)
{
  // TODO: Implement this function.
  char head = get_board_at(state, state->snakes[snum].head_row, state->snakes[snum].head_col);

  return get_board_at(state, get_next_row(state->snakes[snum].head_row, head), get_next_col(state->snakes[snum].head_col, head));
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum)
{
  // TODO: Implement this function.
  char head = state->board[state->snakes[snum].head_row][state->snakes[snum].head_col];

  unsigned int next_head_row = get_next_row(state->snakes[snum].head_row, head);
  unsigned int next_head_col = get_next_col(state->snakes[snum].head_col, head);

  state->board[next_head_row][next_head_col] = head;
  state->board[state->snakes[snum].head_row][state->snakes[snum].head_col] = head_to_body(head);

  state->snakes[snum].head_row = next_head_row;
  state->snakes[snum].head_col = next_head_col;

  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum)
{
  char tail = state->board[state->snakes[snum].tail_row][state->snakes[snum].tail_col];

  unsigned int next_tail_row = get_next_row(state->snakes[snum].tail_row, tail);
  unsigned int next_tail_col = get_next_col(state->snakes[snum].tail_col, tail);

  state->board[state->snakes[snum].tail_row][state->snakes[snum].tail_col] = ' ';
  state->board[next_tail_row][next_tail_col] = body_to_tail(state->board[next_tail_row][next_tail_col]);

  state->snakes[snum].tail_row = next_tail_row;
  state->snakes[snum].tail_col = next_tail_col;

  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
  for (unsigned int s = 0; s < state->num_snakes; s++)
  {

    if (next_square(state, s) == '#' || is_snake(next_square(state, s)))
    { // hit the wall or body of another snake

      state->board[state->snakes[s].head_row][state->snakes[s].head_col] = 'x';
      state->snakes[s].live = false;
    }
    else if (next_square(state, s) == '*')
    { // ate the fruit

      update_head(state, s);
      add_food(state);
    }
    else
    { // simply moving forward

      update_head(state, s);
      update_tail(state, s);
    }
  }

  return;
}

/* Task 5 */
game_state_t *load_board(char *filename)
{
  // TODO: Implement this function.
  FILE *in_file = fopen(filename, "r");

  if (in_file == NULL)
  {
    return NULL;
  }

  game_state_t *state = (game_state_t *)malloc(1 * sizeof(struct game_state_t));

  unsigned int row = 0;
  unsigned int col = 0;
  unsigned int curr_line_memory = 50; // arbitrarily set to 50 lines at first

  state->board = malloc(curr_line_memory * sizeof(char *));
  state->board[0] = malloc(1 * sizeof(char));

  while (true)
  {

    char c = fgetc(in_file); // read in one char

    if (c == EOF)
    {

      break;
    }
    else
    {

      if (c == '\n')
      { // new line

        col++;

        state->board[row] = realloc(state->board[row], col * sizeof(char));

        state->board[row][col - 1] = '\0'; // put the null terminator at the end of each row
        row++;

        if (row == curr_line_memory)
        { // if row reaches the limit, double the limit and reallocate

          curr_line_memory *= 2;
          state->board = realloc(state->board, curr_line_memory * sizeof(char *));
        }

        state->board[row] = malloc(1 * sizeof(char)); // make a new row
        col = 0;                                      // restart col for the new row
      }
      else
      {
        col++; // new character same row

        if (col % 2 == 1) // every other char, increase the limit of space
        {
          state->board[row] = realloc(state->board[row], (col + 1) * sizeof(char));
        }

        state->board[row][col - 1] = c; // load the char into board
      }
    }
  }

  state->num_rows = row;
  state->board = realloc(state->board, state->num_rows * sizeof(char *)); // reallocate so that memory exactly fits the number of rows
  state->num_snakes = 0;
  state->snakes = NULL;

  fclose(in_file);

  return state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum)
{
  // TODO: Implement this function.
  unsigned int row = state->snakes[snum].tail_row;
  unsigned int col = state->snakes[snum].tail_col;

  while (!is_head(get_board_at(state, row, col)))
  {

    col = get_next_col(col, get_board_at(state, row, col));
    row = get_next_row(row, get_board_at(state, row, col));
  }

  state->snakes[snum].head_col = col;
  state->snakes[snum].head_row = row;

  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state)
{
  state->num_snakes = 0;
  state->snakes = malloc(1 * sizeof(snake_t));

  for (unsigned int row = 0; row < state->num_rows; row++)
  {
    for (unsigned int col = 0; col < strlen(state->board[row]); col++)
    {

      if (is_tail(get_board_at(state, row, col)))
      {

        state->num_snakes++;

        state->snakes = realloc(state->snakes, state->num_snakes * sizeof(struct snake_t));

        state->snakes[state->num_snakes - 1].tail_row = row;
        state->snakes[state->num_snakes - 1].tail_col = col;

        find_head(state, state->num_snakes - 1);

        state->snakes[state->num_snakes - 1].live = true;
      }
    }
  }
  return state;
}