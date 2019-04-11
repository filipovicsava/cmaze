#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
    #include <conio.h>
    #include <Windows.h>
#else
    #include <unistd.h>
    #include <curses.h>
#endif

void showMaze(const char *maze, int width, int height) {
    clear();
    refresh();
    int x, y;
    for(y = 0; y < height; y++) {
        for(x = 0; x < width; x++) {
            switch(maze[y * width + x]) {
                case 1:  printf("[]");  break;
                case 2:  printf("<>");  break;
                default: printf("  ");  break;
            }
        }
        printf("\r\n");
    }

}

void carveMaze(char *maze, int width, int height, int x, int y) {
    int x1, y1;
    int x2, y2;
    int dx, dy;
    int dir, count;

    dir = rand() % 4;
    count = 0;
    while(count < 4) {
        dx = 0; dy = 0;
        switch(dir) {
            case 0:  dx = 1;  break;
            case 1:  dy = 1;  break;
            case 2:  dx = -1; break;
            default: dy = -1; break;
        }
        x1 = x + dx;
        y1 = y + dy;
        x2 = x1 + dx;
        y2 = y1 + dy;
        if(   x2 > 0 && x2 < width && y2 > 0 && y2 < height
              && maze[y1 * width + x1] == 1 && maze[y2 * width + x2] == 1) {
            maze[y1 * width + x1] = 0;
            maze[y2 * width + x2] = 0;
            x = x2; y = y2;
            dir = rand() % 4;
            count = 0;
        } else {
            dir = (dir + 1) % 4;
            count += 1;
        }
    }

}

void generateMaze(char *maze, int width, int height) {
    int x, y;

    for(x = 0; x < width * height; x++) {
        maze[x] = 1;
    }
    maze[1 * width + 1] = 0;

    srand(time(0));

    for(y = 1; y < height; y += 2) {
        for(x = 1; x < width; x += 2) {
            carveMaze(maze, width, height, x, y);
        }
    }

    maze[0 * width + 1] = 2;
    maze[(height - 1) * width + (width - 2)] = 3;

}

void movePlayer(char *maze, int *x, int *y, int width, int height, int dx, int dy) {
    switch (maze[(*y + dy) * width + (*x + dx)]) {
        case 1: break;
        case 2:
            maze[(*y) * width + (*x)] = 3;
            *x += dx;
            *y += dy;
            break;
        default:
            maze[(*y + dy) * width + (*x + dx)] = 2;
            *x += dx;
            *y += dy;
            break;
    }
    showMaze(maze, width, height);
}

void startGame(int width, int height) {
    initscr();
    char *maze;
    width = width * 2 + 3;
    height = height * 2 + 3;

    maze = (char *) malloc(width * height * sizeof(char));
    if (maze == NULL) {
        printf("error: not enough memory\n");
        exit(EXIT_FAILURE);
    }

    generateMaze(maze, width, height);
    showMaze(maze, width, height);

    int x, y;

    x = 1;
    y = 0;
    while (x != width - 2 || y != height - 2) {
        if (getchar() == '\033') {
            getchar();
            switch (getchar()) {
                case 'A':
                    movePlayer(maze, &x, &y, width, height, 0, -1);
                    break;
                case 'B':
                    movePlayer(maze, &x, &y, width, height, 0, 1);
                    break;
                case 'C':
                    movePlayer(maze, &x, &y, width, height, 1, 0);
                    break;
                case 'D':
                    movePlayer(maze, &x, &y, width, height, -1, 0);
                    break;
                default:
                    printf("please use the arrow keys!");
                    break;
            }
        }
    }

    printf("Wow, you solved the maze! Press any key to exit");
    getch();

    endwin();
    free(maze);
}

void main() {
    printf("Hi, welcome to the game! Which mode would you like to play on?\n");
    printf("  1) Easy\n  2) Medium\n  3) Hard\n");
    printf("Enter mode (1, 2 or 3): ");
    int mode = 0;
    scanf("%d", &mode);

    switch(mode) {
        case 1: startGame(10, 10); break;
        case 2: startGame(20, 20); break;
        case 3: startGame(30, 30); break;
        default: printf("Bad input..."); exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
