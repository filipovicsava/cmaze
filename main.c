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

/* Display the maze. */
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

/*  Carve the maze starting at x, y. */
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

/* Generate maze in matrix maze with size width, height. */
void generateMaze(char *maze, int width, int height) {

    int x, y;

    /* Initialize the maze. */
    for(x = 0; x < width * height; x++) {
        maze[x] = 1;
    }
    maze[1 * width + 1] = 0;

    /* Seed the random number generator. */
    srand(time(0));

    /* Carve the maze. */
    for(y = 1; y < height; y += 2) {
        for(x = 1; x < width; x += 2) {
            carveMaze(maze, width, height, x, y);
        }
    }

    /* Replace the entry and exit. */
    maze[(height - 2) * width + (width - 2)] = 2;
    maze[(height - 1) * width + (width - 2)] = 2;

    /* Set up the entry and exit. */
    maze[0 * width + 1] = 3;
    maze[(height - 1) * width + (width - 2)] = 3;

}

/* Solve the maze. */
void solveMaze(char *maze, int width, int height) {
    int dir, count;
    int x, y;
    int dx, dy;
    int forward;

    /* Remove the entry and exit. */
    maze[0 * width + 1] = 1;
    maze[(height - 1) * width + (width - 2)] = 1;

    forward = 1;
    dir = 0;
    count = 0;
    x = 1;
    y = 1;
    while(x != width - 2 || y != height - 2) {
        dx = 0; dy = 0;
        switch(dir) {
            case 0:  dx = 1;  break;
            case 1:  dy = 1;  break;
            case 2:  dx = -1; break;
            default: dy = -1; break;
        }
        if(   (forward  && maze[(y + dy) * width + (x + dx)] == 0)
              || (!forward && maze[(y + dy) * width + (x + dx)] == 2)) {
            maze[y * width + x] = forward ? 2 : 3;
            x += dx;
            y += dy;
            forward = 1;
            count = 0;
            dir = 0;
        } else {
            dir = (dir + 1) % 4;
            count += 1;
            if(count > 3) {
                forward = 0;
                count = 0;
            }
        }
        showMaze(maze, width, height);
        sleep(1);
    }

    /* Replace the entry and exit. */
    maze[(height - 2) * width + (width - 2)] = 2;
    maze[(height - 1) * width + (width - 2)] = 2;

}

/*int main(int argc,char *argv[]) {

    int width, height;
    char *maze;

    if(argc != 3 && argc != 4) {
        printf("usage: maze <width> <height> [s]\n");
        exit(EXIT_FAILURE);
    }

    width = atoi(argv[1]) * 2 + 3;
    height = atoi(argv[2]) * 2 + 3;
    if(width < 7 || height < 7) {
        printf("error: illegal maze size\n");
        exit(EXIT_FAILURE);
    }
    if(argc == 4 && argv[3][0] != 's') {
        printf("error: invalid argument\n");
        exit(EXIT_FAILURE);
    }

    maze = (char*)malloc(width * height * sizeof(char));
    if(maze == NULL) {
        printf("error: not enough memory\n");
        exit(EXIT_FAILURE);
    }

    generateMaze(maze, width, height);
    showMaze(maze, width, height);

    if(argc == 4) {
        solveMaze(maze, width, height);
        showMaze(maze, width, height);
    }

    free(maze);
    exit(EXIT_SUCCESS);

} */

void movePlayer(char *maze, int *x, int *y, int width, int height, int dx, int dy) {
    switch (maze[(*y + dy) * width + (*x + dx)]) {
        case 1: break;
        case 2:
            maze[(*y) * width + (*x)] = 3;
            *x += dx;
            *y += dy;
            break;
        default:
            maze[(*y) * width + (*x)] = 2;
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

    maze = (char*)malloc(width * height * sizeof(char));
    if(maze == NULL) {
        printf("error: not enough memory\n");
        exit(EXIT_FAILURE);
    }

    generateMaze(maze, width, height);
    showMaze(maze, width, height);
    printf("Do you want us to solve the maze? [y/n] ");
    char solve = 'n';
    scanf("%s", &solve);
    if (solve == 'y' || solve == 'Y') {
        solveMaze(maze, width, height);
    } else {
        int dir, count;
        int x, y;
        int dx, dy;
        int forward;

        forward = 1;
        dir = 0;
        count = 0;
        x = 1;
        y = 1;
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
    }

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
