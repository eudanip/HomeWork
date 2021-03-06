#include<stdio.h>
#include<algorithm>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<time.h>

using namespace std;

#define NMAX 105

int lines, columns, answer, gameType;
int globalMap[NMAX][NMAX], dist[NMAX][NMAX];
char s[NMAX];
char viz[NMAX][NMAX];
int dx[4] = {-1,0,1,0};
int dy[4] = {0,-1,0,1};

int panda[6][6] = {{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,1,0,1,1,0},
{0,0,0,1,0,0},
{0,0,1,0,1,1},
{0,0,0,0,0,0}};


int vampire[12][11] = {{0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,1,0,0,0,0,0,0},
{0,0,0,1,0,1,0,0,1,1,1},
{0,1,0,1,0,0,1,0,0,0,0},
{0,0,0,1,1,0,0,1,0,1,0},
{0,1,0,1,0,0,1,0,0,1,0},
{0,0,0,0,0,1,0,0,1,0,0},
{0,0,1,1,1,1,0,1,1,1,0},
{0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,1,0,1,0,1,0,1},
{0,0,0,1,0,1,0,1,1,0,1},
{0,0,0,0,0,0,0,0,1,0,0}};

int nascar[15][16] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0},
{0,0,0,0,1,0,0,1,0,0,1,0,0,1,1,0},
{0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,1},
{0,1,0,0,0,0,1,0,0,1,0,1,0,0,0,0},
{0,0,0,0,1,0,0,1,0,0,1,1,1,0,1,0},
{0,1,1,1,0,1,0,0,1,0,0,1,0,0,1,0},
{0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0},
{0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0},
{0,0,1,0,0,1,1,0,1,0,1,0,0,1,1,0},
{0,0,0,1,0,0,0,0,0,0,1,1,0,1,0,0},
{0,1,0,1,1,1,0,1,1,1,0,0,0,1,0,1},
{0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0},
{0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,1},
{0,0,0,1,1,0,0,0,1,0,0,1,0,1,0,0}};



class Player {
public:
    int status, difficulty;
    int mymap[NMAX][NMAX];
    int enemyMap[NMAX][NMAX];
    pair<int, int> pos;
};

Player player1, player2;

void showExample() {
    printf("This is a 5 X 6 matrix:\n\n.xx...\n.x..x.\n...x..\n.xx..x\n...x..\n\n");
    printf("The dot represents a free cell and the x an occupied one.\n\n");
    printf("If you also want to see a path, press 1. Press any other digit to continue the rules.\n");

    scanf("%d",&answer);
    if(answer == 1) {
        printf("\nP is denoted for the Path:\n\nPxxPPP\nPxPPxP\nPPPxPP\n.xx.Px\n...xPP\n\n");
        printf("Press any digit to continue.\n");
        scanf("%d",&answer);
    }
}

void showRules() {
    printf("\nBrain Labyrint consists in an 1 vs 1 game, were each player has a matrix of a specific size.\n\n");
    printf("The matrix has 2 types of cells: free cells and occupied cells (you can imagine that there is a wall there). ");
    printf("The only condition that a matrix should have is to have a 4-way path (going only Nord, South, East and West) from the Top-Left Cell to the Down-Right one.\n");

    printf("\nDo you need an example? (press 1 for Yes and 2 for No)\n\n");
    scanf("%d",&answer);
    if(answer == 1)
        showExample();

    printf("The game consists of 2 contestans changing turns. First, you have to create your own map which will be a playground for the enemy. ");
    printf("Of course, the enemy will also prepare his own map. After you have created your map, the game starts.\n\n");
    printf("At each point, you know all the free cells and walls you have already discovered from the enemy's map, as well as your current position. ");
    printf("Your target is to reach the final destination of the enemy's map (Down-Right corner) before the enemy does on your map. ");
    printf("When it's your turn, you can press N, S, E, W for North, South, East and West. If it's a cell, you go there and you have one more try. ");
    printf("Otherwise, you stop there and it's the enemy's turn. Finally, ? will denote an unknown cell, while P (player) will be used for your current position.\n\n");

    printf("Press any digit to continue.\n");
    scanf("%d",&answer);
}

void setEnemyMatrix(int type) {
    if(type == 1) {
        lines = 5;
        columns = 5;
        for(int i = 1; i <= lines; i++)
            for(int j = 1; j <= columns; j++)
                player2.mymap[i][j] = panda[i][j];
    }
    else if(type == 2) {
        lines = 11;
        columns = 10;
        for(int i = 1; i <= lines; i++)
            for(int j = 1; j <= columns; j++)
                player2.mymap[i][j] = vampire[i][j];
    }
    else if(type == 3) {
        lines = 14;
        columns = 15;
        for(int i = 1; i <= lines; i++)
            for(int j = 1; j <= columns; j++)
                player2.mymap[i][j] = nascar[i][j];
    }
}

void customSize() {
    printf("Please write the number of rows for the matrix: \n");
    scanf("%d",&lines);
    printf("Please write the number of columns for the matrix: \n");
    scanf("%d",&columns);
}

void chooseSize() {

    if(gameType == 2) {
        customSize();
        return ;
    }
    printf("Choose the map you want to play agains:\n 1 - Panda Map\n 2 - Vampire Map\n 3 - Nascar Map\n");
    scanf("%d",&answer);
    setEnemyMatrix(answer);

    return ;
}

int dfs(int pLine, int pcolumn) {
    if(pLine == lines && pcolumn == columns)
        return 1;
    viz[pLine][pcolumn] = 1;
    for(int dir = 0; dir < 4; dir++) {
        int sLine = pLine + dx[dir];
        int scolumn = pcolumn + dy[dir];
        if(sLine < 1 || sLine > lines || scolumn < 1 || scolumn > columns || viz[sLine][scolumn] || globalMap[sLine][scolumn])
            continue;
        int verdict = dfs(sLine, scolumn);
        if(verdict) {
            viz[pLine][pcolumn] = 0;
            return verdict;
        }
    }
    return 0;
}

int checkMatrix(int matrix[NMAX][NMAX]) {
    if(matrix[1][1] || matrix[lines][columns]) {
        printf("Incorrect map. Please try again.\n");
        return 0;
    }
    for(int i = 1; i <= lines; i++)
        for(int j = 1; j <= columns; j++)
            globalMap[i][j] = matrix[i][j];
    int verdict = dfs(1,1);
    for(int i = 1; i <= lines; i++)
        for(int j = 1; j <= columns; j++)
            viz[i][j] = 0;
    if(!verdict) {
        printf("Incorrect map. Please try again.\n");
    }
    return verdict;
}

void customMap(int index) {
    int ok;
    char car;
    if(index == 1) {
        printf("Player1, please write the map which you will use for the game. Write an %d x %d matrix using . for free cells and x for occupied cells. You should type %d lines, each line containing %d characters.\n", lines, columns, lines, columns);
        scanf("\n");
        do {
            ok = 1;
            for(int i = 1; i <= lines; i++)
                for(int j = 1; j <= columns + 1 && ok; j++) {
                    scanf("%c", &car);
                    if(j <= columns && car != '.' && car != 'x') {
                        printf("Incorrect map. Please try again.\n");
                        ok = 0;
                    }
                    player1.mymap[i][j] = (car == '.' ? 0 : 1);
                }
            if(!ok)
                continue;
            ok = checkMatrix(player1.mymap);
        }while(!ok);
    }
    else {
        printf("Player2, please write the map which you will use for the game. Write an %d x %d matrix using . for free cells and x for occupied cells. You should type %d lines, each line containing %d characters.\n", lines, columns, lines, columns);
        scanf("\n");
        do {
            ok = 1;
            for(int i = 1; i <= lines; i++)
                for(int j = 1; j <= columns + 1 && ok; j++) {
                    scanf("%c", &car);
                    if(j <= columns && car != '.' && car != 'x') {
                        printf("Incorrect map. Please try again.\n");
                        ok = 0;
                    }
                    player2.mymap[i][j] = (car == '.' ? 0 : 1);
                }
            if(!ok)
                continue;
            ok = checkMatrix(player2.mymap);
        }while(!ok);
    }
    for(int i = 1; i <= 100; i++) {
        printf("BLA BLA BLA\n");
    }
}

void selectDifficulty() {
    int diff = 0;
    printf("Please choose difficulty level. Press 1 for Easy, 2 for Medium and 3 for Hard.\n");
    scanf("%d",&diff);
    player2.difficulty = diff;
}

void setInformationAboutEnemy() {
    player1.pos.first = player1.pos.second = 1;
    player2.pos.first = player2.pos.second = 1;

    for(int i = 1; i <= lines; i++)
        for(int j = 1; j <= columns; j++) {
            player1.enemyMap[i][j] = 2;
            player2.enemyMap[i][j] = 2;
        }
    player1.enemyMap[1][1] = 0;
    player2.enemyMap[1][1] = 0;
}

void beforeStartGame() {
    printf("Welcome to Brain Labyrint!!!!\n\n");
    Sleep(500);

    printf("Do you need to read the rules?  (press 1 for Yes and 2 for NO)\n");
    scanf("%d",&answer);
    if(answer == 1)
        showRules();

    printf("Select type of game:\n\n* Press 1 for Player vs Computer\n* Press 2 for Player vs Player.\n");
    do {
        scanf("%d",&gameType);
        if(gameType != 1 && gameType != 2)
            printf("Please press 1 or 2 :).\n");
    }while(gameType != 1 && gameType != 2);
    if(gameType == 1) {
        selectDifficulty();
    }
    player1.status = 1; // 1 - player, 0 - computer
    player2.status = (gameType == 1 ? 0 : 1);

    chooseSize();
    if(gameType == 2) {
        customMap(1);
        customMap(2);
    }
    else
        customMap(1);

    setInformationAboutEnemy();
}

int isItDone() {
    if(player1.pos.first == lines && player1.pos.second == columns) {
        printf("Player 1 Wins.\n");
        return 1;
    }
    if(player2.pos.first == lines && player2.pos.second == columns) {
        printf("Player 2 Wins.\n");
        return 1;
    }
    return 0;
}

int validPosition(pair<int,int> pos, Player player) {
    return (!(pos.first < 1 || pos.first > lines || pos.second < 1 || pos.second > columns || player.mymap[pos.first][pos.second]));
}

void printMatrix(Player player) {
    for(int i = 1; i <= lines; i++, printf("\n"))
        for(int j = 1; j <= columns; j++)
            if(i == player.pos.first && j == player.pos.second)
                printf("P");
            else {
                printf("%c", (player.enemyMap[i][j] ? (player.enemyMap[i][j] == 1 ? 'x' : '?') : '.'));
            }
}

int playGame(Player &player1, Player &player2) {
    printf("Press a letter for one of the 4 directions: N, S, E and W.\n");
    char direction;
    pair<int,int> nextPos;
    while(1) {
        printMatrix(player1);
        do {
            scanf("%c", &direction);
        }while(direction != 'N' && direction != 'S' && direction != 'E' && direction != 'W');

        if(direction == 'N') {
            nextPos = make_pair(player1.pos.first - 1, player1.pos.second);
        } else if(direction == 'S') {
            nextPos = make_pair(player1.pos.first + 1, player1.pos.second);
        } else if(direction == 'E') {
            nextPos = make_pair(player1.pos.first, player1.pos.second + 1);
        } else {
            nextPos = make_pair(player1.pos.first, player1.pos.second - 1);
        }
        if(!validPosition(nextPos, player2)) {
            printf("Tzeapa. N-ai mai luat tzeapa? Fortza Steaua!!!!\n");
            player1.enemyMap[nextPos.first][nextPos.second] = 1;
            break;
        }

        player1.pos = nextPos;
        player1.enemyMap[nextPos.first][nextPos.second] = 0;
        if(isItDone()) {
            return 1;
        }
        printf("Good move. You have one more try.\n");
    }
    return 0;
}

int playAI() {
    while(1) {
        printMatrix(player2);
        Sleep(2000);
        int prob = rand() % 100 + 1;
        int diff = 50 + (player2.difficulty * 10);
        if(prob <= diff) {
            for(int i = 0; i < 4; i++) {
                int sx = player2.pos.first + dx[i];
                int sy = player2.pos.second + dy[i];
                if(sx < 1 || sx > lines || sy < 1 || sy > columns || player1.mymap[sx][sy] == 1) {
                    continue;
                }
                if(dist[sx][sy] == dist[player2.pos.first][player2.pos.second] - 1) {
                    player2.pos = make_pair(sx,sy);
                    player2.enemyMap[sx][sy] = 0;
                    if(isItDone())
                        return 1;
                    printf("Good move. You have one more try.\n");
                    break;
                }
            }
        }
        else {
            int randMove = rand() % 4;
            int sx = player2.pos.first + dx[randMove];
            int sy = player2.pos.second + dy[randMove];
            if(sx < 1 || sx > lines || sy < 1 || sy > columns || player1.mymap[sx][sy] == 1) {
                printf("Tzeapa. N-ai mai luat tzeapa? Fortza Steaua!!!!\n");
                player2.enemyMap[sx][sy] = 1;
                break;
            }
            player2.pos = make_pair(sx,sy);
            player2.enemyMap[sx][sy] = 0;
            if(isItDone())
                return 1;
            printf("Good move. You have one more try.\n");
        }
    }
    return 0;
}

void dfsDistance(int px, int py) {
    viz[px][py] = 1;

    for(int i = 0; i < 4; i++) {
        int sx = px + dx[i];
        int sy = py + dy[i];
        if(sx < 1 || sx > lines || sy < 1 || sy > columns || viz[sx][sy] || player1.mymap[sx][sy])
            continue;
        dist[sx][sy] = dist[px][py] + 1;
        dfsDistance(sx,sy);
    }
}

int main () {

    srand(time(0));

    beforeStartGame();

    printf("The game will start in ");
    Sleep(1000);
    printf("3...");
    Sleep(1000);
    printf("2...");
    Sleep(1000);
    printf("1...");
    Sleep(1000);
    printf("\nGO\n");
    Sleep(1000);

    if(gameType == 1) {
        for(int i = 1; i <= lines; i++)
            for(int j = 1; j <= columns; j++)
                dist[i][j] = 1000000000;
        dist[lines][columns] = 0;
        dfsDistance(lines, columns);
    }

    int turn = 1;
    while(1) {
        if(gameType == 1 && !turn) {
            printf("PLAYER 2:\n");
            if(playAI())
                break;
        }
        else if(turn) {
            printf("PLAYER 1:\n");
            if(playGame(player1, player2))
                break;
        }
        else {
            printf("PLAYER 2:\n");
            if(playGame(player2, player1))
                break;
        }
        turn = 1 - turn;
    }

    return 0;
}


