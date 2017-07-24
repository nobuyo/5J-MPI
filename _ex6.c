#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAPNUM 40//マップの辺 の 大 き さ
#define GNUM 20//世代数

int mpiCheck(int nsize);
void mapInit(int map[][MAPNUM]);
void mapDisplay(int map[][MAPNUM]);
void lifeJudge(int map[][MAPNUM], int temp[][MAPNUM], int nsize);
void mapRefresh(int oldmap[][MAPNUM], int newmap[][MAPNUM]);
// void send()

int main(int argc ,char ** argv) {
    int i;
    int count = 0;
    int map[MAPNUM][MAPNUM]; //現在のマップ
    int temp[MAPNUM][MAPNUM]; //作業用マップ

    int nsize;
    int myrank;
    int my_name_len;
    char my_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Get_processor_name(my_name, &my_name_len);

    int norimap[MAPNUM / (nsize - 1) + 1][MAPNUM]; //のりしろを付加したマップ
    int noritemp[MAPNUM / (nsize - 1) + 1][MAPNUM]; //作業用マップ

    //分割判定
    if (mpiCheck(nsize - 1) == -1){
        printf("cpu num must be even number.\n");
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    // CPU
    if (myrank == 0) {
        //マップの初期化
        mapInit(map);
        mapInit(temp);
    }


    while(1) {
        if (myrank == 0) {
            printf("Generation: %d\n", count);
            mapDisplay(map);

            //マップ分割送信
            for (i=1; i<nsize; i++) {
                MPI_Send(map[((i - 1) * (MAPNUM / (nsize - 1)) + MAPNUM - 1) % MAPNUM] , MAPNUM , MPI_INTEGER , i , i ,MPI_COMM_WORLD);
                MPI_Send(map[(i - 1) * ( MAPNUM /( nsize - 1))] , MAPNUM * (MAPNUM /(nsize - 1)) , MPI_INTEGER , i ,100 + i ,MPI_COMM_WORLD);
                MPI_Send(map[(i *( MAPNUM /( nsize - 1)))% MAPNUM] , MAPNUM ,MPI_INTEGER , i ,200 + i , MPI_COMM_WORLD);
            }

            //分割マップ受信
            for (i=1; i<nsize; i++) {
                MPI_Recv(temp[(i - 1)* MAPNUM /( nsize - 1)], MAPNUM * (MAPNUM / (nsize - 1)), MPI_INTEGER, i, 300 + i, MPI_COMM_WORLD, &status);
            }

            //マップ更新
            mapRefresh(map, temp);
        }
        else {
            MPI_Recv(norimap[0], MAPNUM, MPI_INTEGER, 0, myrank, MPI_COMM_WORLD, &status);
            MPI_Recv(norimap[1], MAPNUM * (MAPNUM/(nsize - 1)),MPI_INTEGER , 0, 100 + myrank , MPI_COMM_WORLD , &status);
            MPI_Recv(norimap[MAPNUM / (nsize - 1) + 1] , MAPNUM ,MPI_INTEGER , 0, 200 + myrank , MPI_COMM_WORLD , &status);
            lifeJudge(norimap, noritemp, nsize);

            MPI_Send(noritemp[1], MAPNUM *(MAPNUM/(nsize -1)), MPI_INTEGER, 0, 300 + myrank , MPI_COMM_WORLD);
        }

        if (count == GNUM) break;
        count++;
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

    /*
    * 分 割 可 能 か ど う か の チ ェ ッ ク 関 数
    */

int mpiCheck (int nsize) {
    if (MAPNUM % nsize == 0) {
        return 0;
    }
    else {
        return -1;
    }
}

    /* マ ッ プ の 初 期 化

    * 生 物. . . 
    * 何 も な し. . . 
    */

void mapInit(int map[][MAPNUM]) {
    int i, j;
    for (i=0; i<MAPNUM; i++) {
        for (j=0; j < MAPNUM; j++) {
            map[i][j] = 0;
        }
    }

    // グライダー
    map[0][1] = 1;
    map[1][2] = 1;
    map[2][0] = 1;
    map[2][1] = 1;
    map[2][2] = 1;
}


    /* マ ッ プ の 表 示 */

void mapDisplay(int map[][MAPNUM]) {
    int i, j;

    for (i=0; i<MAPNUM; i++) {
        for (j=0; j<MAPNUM; j++){
            if (map[i][j] == 1) printf("@");
            else printf ("-");
        }
        printf("\n");
    }
    printf("\n");
}


void lifeJudge(int map[][MAPNUM], int temp[][MAPNUM], int nsize) {
    int i, j;
    int k, l, n, m;
    int count;

    for (i = 0; i < MAPNUM/(nsize-1)+2; i++) {
        for (j = 0; j < MAPNUM; j++) {
            k = (MAPNUM + i - 1) % MAPNUM;
            l = (MAPNUM + j - 1) % MAPNUM;
            m = (i + 1) % MAPNUM;
            n = (j + 1) % MAPNUM;

            count = map[k][l]   +  map[k][j]   +  map[k][n]
                  + map[i][l]   /*map[i][j]*/  +  map[i][n]
                  + map[m][l]   + map[m][j]    +  map[m][n];

            if (count == 3) {
                temp[i][j] = 1;
            }
            else if (count == 2) {
                temp[i][j] = map[i][j];
            }
            else {
                temp[i][j] = 0;
            }
        }
    }
}


void mapRefresh(int oldmap[][MAPNUM], int newmap[][MAPNUM]) {
    int i, j;

    for (i=0; i<MAPNUM; i++){
        for (j=0; j < MAPNUM; j++){
            oldmap[i][j] = newmap[i][j];
        }
    }
}
























