# 2048 Game for STM32F429ZIT6

A complete implementation of the classic 2048 puzzle game for STM32F429ZIT6 microcontroller with LCD touchscreen, featuring TouchGFX framework for modern and smooth graphical interface.

## Project Description

This is an embedded version of the famous 2048 game, designed and optimized to run on STM32F429ZIT6 development kit. The game utilizes the integrated LCD display with TouchGFX GUI framework and is controlled by 4 physical push buttons (UP, DOWN, LEFT, RIGHT).

## Key Features

- **Complete game logic**: Full implementation of 2048 game algorithm with move, merge, and random tile spawn capabilities
- **TouchGFX interface**: Beautiful UI with custom color scheme for each tile value
- **Physical button control**: Uses 4 physical buttons with debouncing mechanism
- **High score saving**: Automatically saves best score to RTC backup register
- **Win/Game Over screens**: Dedicated UI notifications for winning (reaching 2048) or losing
- **Continue option**: Allows continuing gameplay after reaching 2048
- **Custom random generator**: Uses Linear Congruential Generator (LCG) instead of standard library

## Hardware Requirements

- **Microcontroller**: STM32F429ZIT6
- **Display**: Built-in LCD on STM32F429I-DISC1 kit
- **Buttons**: 4 push buttons connected to pins:
  - UP_Pin (EXTI1)
  - DOWN_Pin (EXTI2) 
  - LEFT_Pin (EXTI15_10)
  - RIGHT_Pin (EXTI15_10)
- **Peripherals used**:
  - LTDC (LCD-TFT Display Controller)
  - DMA2D (Chroma Art Accelerator)
  - RTC (Real-Time Clock)
  - UART1 (optional for debugging)
  - TIM6 (Timer)

## Technologies Used

- **Framework**: STM32 HAL Library
- **GUI**: TouchGFX Designer
- **IDE**: STM32CubeIDE
- **Languages**: C/C++
- **Build system**: ARM GCC

## How to Play

1. **Objective**: Merge numbered tiles to create a tile with value 2048
2. **Controls**: 
   - UP button: Move up
   - DOWN button: Move down  
   - LEFT button: Move left
   - RIGHT button: Move right
3. **Game Rules**:
   - Each move slides all tiles in that direction
   - Two tiles with the same value merge into one tile with double value
   - After each move, 2 new tiles (value 2 or 4) appear randomly
   - Game ends when no valid moves are possible

## Game Tile Colors

The game uses a custom color palette for each tile value:

- **2**: Purple (150, 0, 255)
- **4**: Deep Pink (240, 20, 90)
- **8**: Yellow (255, 201, 27)
- **16**: Light Orange (245, 149, 99)
- **32**: Blue (0, 149, 214)
- **64**: Dark Purple (206, 0, 123)
- **128**: Dark Orange (255, 85, 24)
- **256**: Green (41, 215, 165)
- **512**: Red (255, 0, 36)
- **1024**: Dark Violet (95, 6, 155)

## 🔧 Main API

### Game Logic Functions
```c
void initGame(void);                              // Initialize new game
void handleInputDirection(int direction);         // Handle button input
bool moveLeft/Right/Up/Down(int board[][4]);     // Move in direction
void spawnRandomTile(int board[][4]);            // Spawn random tile
bool checkWin/GameOver(int board[][4]);          // Check win/lose condition
void SaveBestScore(int score);                   // Save high score
int LoadBestScore(void);                         // Load high score
```

### TouchGFX Interface
```cpp
void updateTile(int row, int col, int value);    // Update tile display
void NewGame();                                  // Start new game
void WinGame();                                  // Show win screen
void GameOver();                                 // Show game over screen
void Continue();                                 // Continue after winning
```

## Contact

- **Name**: Nguyen The Anh (nthea)
- **Email**: Ntheanh0104@gmail.com
