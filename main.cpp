// 필요한 헤더파일 불러오기
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>


const int WIDTH_MAX = 20;           // 최대 가로 길이 전역변수로 선언 / 초기화
const int HEIGHT_MAX = 20;          // 최대 세로 길이 전역변수로 선언 / 초기화

int start[2];                       // 시작 지점 전역 변수로 선언
int goal[2];                        // 도착 지점 전역 변수로 선언
char map[HEIGHT_MAX][WIDTH_MAX];    // 맵 전역 변수로 선언

int came_from[HEIGHT_MAX][WIDTH_MAX][2];    // 길 추적 배열 선언

int calc_cost(int* pos);                                        // 비용 계산 함수 선어
int is_valid(int* pos, void* is_visit, int width, int height);  // 갈 수 있는 길인지 계산하는 함수 선언
int cmpfunc(const void* a, const void* b);                      // 정렬을 위한 크기 비교 함수 선언
void search(int width, int height);                             // 탐색 함수 선언

void _print_map(int width, int height, int (*path)[2], int path_len);   // 맵 출력 함수 선언


int main() {
    // 가로 세로 크기 변수 선언
    int width, height;

    // 가로 세로 크기 입력
    system("cls");
    printf("가로와 세로 크기를 입력하세요.\n최소 크기는 1 1, 최대 크기는 %d %d 입니다. \n\n 입력: ", WIDTH_MAX, HEIGHT_MAX);
    scanf("%d %d", &width, &height);
    system("cls");
    
    // 가로 세로 크기에 맞게 맵 입력
    printf("맵을 입력해주세요.\n");
    printf("\t.: 빈 공간\n");
    printf("\tO: 출발지점\n");
    printf("\tX: 도착지점\n");
    printf("\t@: 막힌 벽\n\n");
    printf("맵 : \n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            scanf(" %c", &map[i][j]);

            // 만약 입력 문자가 O라면 시작 지점 설정
            if (map[i][j] == 'O') {
                start[0] = i;
                start[1] = j;
            }
            // 만약 입력 문자가 X라면 도착 지점 설정
            if (map[i][j] == 'X') {
                goal[0] = i;
                goal[1] = j;
            }
        }
    }
    system("cls");
    
    // 입력받은 가로, 세로, 맵을 이용하여 탐색
    search(width, height);

    // 길을 찾기 위한 배열 선언, 해당 배열의 길이 선언 / 초기화
    int path[WIDTH_MAX * HEIGHT_MAX][2];
    int path_len = 0;

    // came_from 배열로 부터 길 찾기
    // 도착 지점을 가기 위한 위치, 해당 위치를 가기 위한 위치... 반복
    int temp[2] = { came_from[goal[0]][goal[1]][0], came_from[goal[0]][goal[1]][1] };

    // temp가 시작위치가 될 때까지 반복
    while ((temp[0] != start[0]) || (temp[1] != start[1])) {
        // 임시 변수 선언 / 초기화
        int y = temp[0];
        int x = temp[1];

        // 찾은 길을 path 배열에 저장, 배열의 길이 += 1
        path[path_len][0] = y;
        path[path_len][1] = x;
        path_len += 1;

        // temp를 가기 위한 위치로 temp 변경
        temp[0] = came_from[y][x][0];
        temp[1] = came_from[y][x][1];
    }

    // 찾은 길을 기반으로 맵 출력
    _print_map(width, height, path, path_len);
    
    // 일시정지
    system("pause");
    return 0;
}


// 비용 계산 함수 정의
/*
    파라미터:
        현재위치

    함수정보:
        현재위치에 대한 시작지점과의 거리와 도착지점과의 거리의 합을 반환
        거리를 구할 때 유클리드 거리가 아닌 L1 norm을 이용함
*/
int calc_cost(int* pos) {
    return abs(start[0] - pos[0]) + abs(start[1] - pos[1]) + abs(goal[0] - pos[0]) + abs(goal[1] - pos[1]);
}


// 갈 수 있는 길인지 판단하는 함수 정의
/*
    파라미터:
        다음위치, 방문정보, 가로크기, 세로크기

    함수정보:
        다음위치가 0보다 작거나 가로크기, 세로크기를 벗어나면 False(0) 반환
        다음위치에서 방문정보가 True(1) 이라면 False(0) 반환
        다음위치에 벽이 있으면 False(0) 반환
        위 조건을 모두 만족하지 않는다면 True(1) 반환
*/
int is_valid(int* pos, void* is_visit, int width, int height) {
    if (pos[0] < 0 || pos[0] >= height) {
        return 0;
    }
    
    if (pos[1] < 0 || pos[1] >= width) {
        return 0;
    }
    
    if (*((int*)is_visit + pos[0]*WIDTH_MAX + pos[1])) {
        return 0;
    }
    
    if (*((char*)map + pos[0]*WIDTH_MAX + pos[1]) == '@') {
        return 0;
    }
    
    return 1;
}


// 정렬을 위한 크기 비교 함수 정의
/*
    파라미터:
        첫번째 숫자 a, 두번째 숫자 b

    함수정보:
        내림차순 정렬을 위해 a가 크다면 음수를 b가 크다면 양수를 반환
        자세한 정보는 https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/reference/qsort?view=msvc-170 참고
*/
int cmpfunc(const void* a, const void* b) {
    return calc_cost((int*)b) - calc_cost((int*)a);
}


// 탐색 함수 정의
/*
    파라미터:
        가로크기, 세로크기

    함수정보:
        a* 알고리즘을 활용하여 길을 찾는 함수
        탐색한 길은 came_from 배열에 저장하여 도착지점까지 가는 길을 찾을 수 있게 해줌
*/
void search(int width, int height) {
    // 다음에 방문 할 위치를 저장할 배열 선언, 해당 배열의 길이 선언 / 초기화
    int memory[width * height][2];
    int memory_len = 0;

    // 해당 위치를 방문했는지 저장 할 배열 선언 / 초기화
    int is_visit[HEIGHT_MAX][WIDTH_MAX] = { 0, };

    // 현재 위치 선언
    int pos[2];

    // 현재 위치에서 갈 수 있는 방향 선언 / 초기화
    const int dir_x[4] = { 0, 0, -1, 1 };
    const int dir_y[4] = { -1, 1, 0, 0 };

    // 다음 방문 할 위치에 시작 위치를 넣고 배열 길이 += 1
    memory[0][0] = start[0];
    memory[0][1] = start[1];
    memory_len = 1;

    // 다음에 방문 할 위치가 없거나 현재위치가 도착지점일 때까지 반복
    while (memory_len != 0 && (pos[0] != goal[0] || pos[1] != goal[1])) {
        // 현재위치에 다음 방문할 위치의 마지막 값을 저장, 배열의 길이 -= 1
        pos[0] = memory[memory_len-1][0];
        pos[1] = memory[memory_len-1][1];
        memory_len -= 1;

        // 현재위치에 방문 표시
        is_visit[pos[0]][pos[1]] = 1;

        // 모든 방향 반복
        for (int i = 0; i < 4; i++) {
            // 다음 위치 초기화 / 선언
            int next_pos[2] = { pos[0] + dir_x[i], pos[1] + dir_y[i] };

            // 만약 다음 위치가 갈 수 있는 위치라면
            if (is_valid(next_pos, is_visit, width, height)) {
                // 비용 선언 / 초기화
                int cost = calc_cost(next_pos);
                
                // 방문 할 위치에 다음 위치 추가, 배열의 길이 += 1
                memory[memory_len][0] = next_pos[0];
                memory[memory_len][1] = next_pos[1];
                memory_len += 1;

                // 비용이 작은 순서대로 탐색해야 하므로 memory를 비용에 따라 내림차순 정렬, 퀵정렬 사용
                qsort(memory, memory_len, 2*sizeof(int), cmpfunc);
                
                // 길을 추적 할 수 있도록 came_from 배열에서 현재위치를 다음위치에 저장
                came_from[next_pos[0]][next_pos[1]][0] = pos[0];
                came_from[next_pos[0]][next_pos[1]][1] = pos[1];
            }
        }
    }
}


// 맵 출력 함수 정의
/*
    파라미터:
        가로크기, 세로크기, 표시 할 길, 길 배열의 길이

    함수정보:
        시각적으로 잘 보일 수 있게 텍스트 대신 색으로 구별지어 출력함
        벽은 흰색, 시작지점과 도착지점은 빨간색, 길은 초록색으로 표시
*/
void _print_map(int width, int height, int (*path)[2], int path_len) {
    // 맵의 모든 요소 접근
    for (int i = -1; i <= height; i++) {
        // 띄어쓰기만 출력하면 줄 바꿈 시 출력이 사라지므로 .을 검은색 배경에 흰색 글자로 출력
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        printf(".");

        for (int j = -1; j <= width; j++) {
            // 현재위치가 길인지 판단
            int is_path = 0;

            // 현재위치가 path배열에 포함되어 있다면 초록색 출력
            for (int k = 0; k < path_len; k++) {
                if (path[k][0] == i && path[k][1] == j) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xAA);
                    printf("  ");
                    is_path = 1;
                    break;
                }
            }

            // 만약 현재위치가 길이 아니라면
            if (!is_path) {
                // 아래 조건을 만족하면 벽 출력
                if (i < 0 || j < 0 || i >= height || j >= width) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xFF);
                    printf("  ");
                }
                // 아래 조건을 만족하면 벽 출력
                else if (map[i][j] == '@') {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xFF);
                    printf("  ");
                }
                // 아래 조건을 만족하면 검은 배경 출력
                else if (map[i][j] == '.') {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00);
                    printf("  ");
                }
                // 아래 조건을 만족하면 출발지, 도착지 출력
                else if (map[i][j] == 'O' || map[i][j] == 'X') {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xCC);
                    printf("  ");
                }
            }
            
        }

        // 띄어쓰기만 출력하면 줄 바꿈 시 출력이 사라지므로 .을 검은색 배경에 흰색 글자로 출력
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        printf(".\n");
    }
}



// 예시 입력:
//
// 20 20
//
// O@.....@.......@....
// .@.@@@.@.@.@.@.@@@@.
// .@.@...@.@.@.@....@.
// .@.@@@@@.@.@.@@@@...
// .........@.@....@@@@
// .@.@@@@@@@@@@.@.@...
// .@....@.......@@@.@.
// @@.@@.@.@@@@@.@.....
// ...@..@.@...@.@@@@@.
// @@@@.@@...@@@.......
// ...@..@@@@@.@.@@@@@@
// .@@@.@@.....@.......
// .@......@@@.@@@@@@@.
// .@@@@@@@@.........@.
// ........@@.@@@.@.@@@
// .@@@@.@....@.@.@@@..
// ..@.@@@@@@.@.@...@@.
// .....@.....@.@@@....
// .@@@@@@.@@@@.@.@@@@@
// ....@..............X

// comment : 예외처리 넣을려고 했는데 너무 귀찮아서 그냥 잘못 입력하면 알아서 꺼지겠지 하고 냅둠