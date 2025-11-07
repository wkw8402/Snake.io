# Snake.io: A Playable Snake Game in C

Welcome to **Snake.io**! With this repo, you can a fully functional snake game using C.

---

## 🎮 Game Overview
Snake.io is a classic snake game where snakes move around a grid, eating fruit to grow longer while avoiding collisions with walls or their own bodies. If you're not familiar with snake, you can try out a demo at [this link](https://raw.org/demo/html5-snake/).

### Grid Characters:
- **`#`**: Wall  
- **`*`**: Fruit  
- **` `**: Empty space  
- **`wasd`**: Snake tail  
- **`^<v>`**: Snake body  
- **`WASD`**: Snake head  
- **`x`**: Dead snake head  

### Rules:
1. Snakes move in the direction of their heads.
2. Eating a fruit grows the snake.
3. Crashing into walls or bodies results in death.

---

## How the Code Works

### Key Components

1. **Game State Management (`state.c`, `state.h`)**
   - **`create_default_state`**: Initializes a default game state with a predefined board layout, a single snake, and fruit.
   - **`update_state`**: Handles snake movement, fruit consumption, collision detection, and updates the game board.
   - **`free_state`**: Cleans up allocated memory for the game state.
   - **`load_board` and `initialize_snakes`**: Load a custom game board from a file and configure snakes based on the board.

2. **Utility Functions (`snake_utils.c`, `snake_utils.h`)**
   - **Randomized Functions**: `det_rand` generates deterministic random numbers for gameplay consistency. 
   - **Food Placement**: `deterministic_food` places fruits randomly on empty spaces, while `corner_food` places fruits in specific corners.
   - **Snake Control**: `redirect_snake` changes the snake's direction based on player input, and `random_turn` adds randomized turns for autonomous snakes.

3. **Interactive Gameplay (`interactive_snake.c`)**
   - Provides real-time controls using keyboard inputs (e.g., `w`, `a`, `s`, `d`) to direct the snake.
   - Supports adjustable game speed using `[ ]` keys.

4. **Assertions and Testing (`asserts.c`, `asserts.h`, `custom_tests.c`, `unit_tests.c`)**
   - Includes utilities for unit tests (`assert_true`, `assert_equals_*`) to validate game logic.
   - Custom test cases verify individual functions like `is_tail`, `update_state`, and `next_square`.

5. **Main Program (`snake.c`)**
   - Parses command-line arguments for input/output file handling.
   - Integrates all components to simulate and save the Snake game.

### Features
- Modular architecture for easy testing and debugging.
- Unit tests validate each component independently.
- Memory management ensures no leaks during execution (validated with tools like Valgrind).

### Compilation and Execution
- Use `make` to compile the code. Example:
  ```bash
  make snake
  ./snake -i input_board.snk -o output_board.snk
  ```
- For interactive mode:
  ```bash
  make interactive-snake
  ./interactive-snake
  ```

---
