#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep() on Linux/Unix systems
#include <termios.h> // For terminal I/O control (getch alternative)
#include <fcntl.h>   // For non-blocking input

#define WIDTH 20
#define HEIGHT 10

// Directions
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

// Game Variables
int x, y, fruitX, fruitY, score, direction, gameOver;
int tailX[100], tailY[100]; // Tail of the snake
int tailLength;

// Function Declarations
void Setup();
void Draw();
void Input();
void Logic();
int Collision();

// Platform-independent kbhit and getch functions
int _kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

char _getch(void) {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        usleep(100000); // Control the speed of the game (100ms)
    }
    return 0;
}

void Setup() {
    gameOver = 0;
    direction = RIGHT; // Start moving right
    x = WIDTH / 2; // Start position of snake
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH; // Random position for fruit
    fruitY = rand() % HEIGHT;
    score = 0;
    tailLength = 0;
}

void Draw() {
    system("clear"); // Clear console (Linux/Mac). Use system("cls") for Windows.
    for (int i = 0; i < WIDTH + 2; i++) printf("#"); // Top wall
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) printf("#"); // Left wall
            if (i == y && j == x) printf("O"); // Snake head
            else if (i == fruitY && j == fruitX) printf("F"); // Fruit
            else {
                int isTail = 0;
                for (int k = 0; k < tailLength; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o"); // Tail segment
                        isTail = 1;
                    }
                }
                if (!isTail) printf(" ");
            }
            if (j == WIDTH - 1) printf("#"); // Right wall
        }
        printf("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++) printf("#"); // Bottom wall
    printf("\nScore: %d\n", score);
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a': direction = LEFT; break;
            case 'd': direction = RIGHT; break;
            case 'w': direction = UP; break;
            case 's': direction = DOWN; break;
            case 'x': gameOver = 1; break; // Exit the game
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < tailLength; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (direction) {
        case UP: y--; break;
        case DOWN: y++; break;
        case LEFT: x--; break;
        case RIGHT: x++; break;
    }

    if (x >= WIDTH) x = 0; else if (x < 0) x = WIDTH - 1;
    if (y >= HEIGHT) y = 0; else if (y < 0) y = HEIGHT - 1;

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        tailLength++;
    }

    // Check for collision with itself
    if (Collision()) gameOver = 1;
}

int Collision() {
    for (int i = 0; i < tailLength; i++) {
        if (tailX[i] == x && tailY[i] == y) return 1; // Collision detected
    }
    return 0;
}
