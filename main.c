#include <stdio.h>
#include <windows.h>
#define XMAX 79
#define YMAX 24
#define BALL_DEFAULT_X 40
#define BALL_DEFAULT_Y 12
#define RACKET_DEFAULT_Y 12 

void render_screen(int* ball_y, int* ball_x, int* score_1, int* score_2, int* racket_1_y, int* racket_2_y);
void clear_screen(HANDLE hConsole);
void step(int* ball_y, int* ball_x, int* step_y, int* step_x, int* score_1, int* score_2);

int main() {
    HANDLE hStdout;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    int ball_x = BALL_DEFAULT_X;
    int ball_y = BALL_DEFAULT_Y;
    int racket_1_y = RACKET_DEFAULT_Y;
    int racket_2_y = RACKET_DEFAULT_Y;
    int step_x = 1;
    int step_y = 1;
    int score_1 = 11;
    int score_2 = 0;
    int game = 0;
    printf("Press any key to start\n");
    if (getchar()) {
        game = 1;
        clear_screen(hStdout);
    }
    while (game == 1) {
        step(&ball_y, &ball_x, &step_y, &step_x, &score_1, &score_2);
        clear_screen(hStdout);
        render_screen(&ball_y, &ball_x, &score_1, &score_2, &racket_1_y, &racket_2_y);
        printf("%d  %d\n", ball_x, ball_y);
    }
    return 0;
}

void render_screen(int* ball_y, int* ball_x, int* score_1, int* score_2, int* racket_1_y, int* racket_2_y) {
    for (int i = 0; i <= YMAX; i++) {
        for (int j = 0; j <= XMAX; j++) {
            if (i == *ball_y && j == *ball_x) {
                printf("O");
            } else if ((i == 0 && j == 0) || (i == YMAX && j == XMAX)) {
                printf("/");
            } else if ((i == 0 && j == XMAX) || (i == YMAX && j == 0)) {
                printf("\\");
            } else if (i == 0 || i == YMAX) {
                printf("-");
            } else if (j == 0 || j == 39 || j == 40 || j == XMAX) {
                printf("|");
            } else if ((i == *racket_1_y || i == *racket_1_y-1 || i == *racket_1_y+1 || 
                        i == *racket_2_y || i == *racket_2_y-1 || i == *racket_2_y+1) 
                        && (j == 1 || j == 78)) {
                printf("#");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("Player 1 %30d  %-30d Player 2", *score_1, *score_2);
}

void step(int* y, int* x, int* step_y, int* step_x, int* score_1, int* score_2) {
    char c = getchar();
    if (c == ' ') {
        *y = *y + *step_y;
        *x = *x + *step_x;
        if (*y == YMAX-1 || *y == 1) {
            *step_y = -*step_y;
        }
        if (*x == XMAX) {
            *score_1 = *score_1 + 1;
            *x = BALL_DEFAULT_X;
            *y = BALL_DEFAULT_Y;
            *step_x = -*step_x;
        }
        if (*x == 0) {
            *score_2 = *score_2 + 1;
            *x = BALL_DEFAULT_X-1;
            *y = BALL_DEFAULT_Y;
            *step_x = -*step_x;
        }
    }
}

void clear_screen(HANDLE hConsole) {
    COORD coordScreen = { 0, 0 };    // home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks.
    if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
                                    (TCHAR)' ',      // Character to write to the buffer
                                    dwConSize,       // Number of cells to write
                                    coordScreen,     // Coordinates of first cell
                                    &cCharsWritten)) // Receive number of characters written
    {
        return;
    }

    // Get the current text attribute.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    // Set the buffer's attributes accordingly.
    if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
                                    csbi.wAttributes, // Character attributes to use
                                    dwConSize,        // Number of cells to set attribute
                                    coordScreen,      // Coordinates of first cell
                                    &cCharsWritten))  // Receive number of characters written
    {
        return;
    }

    // Put the cursor at its home coordinates.
    SetConsoleCursorPosition(hConsole, coordScreen);
}