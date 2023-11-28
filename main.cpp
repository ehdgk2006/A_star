// �ʿ��� ������� �ҷ�����
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>


const int WIDTH_MAX = 20;           // �ִ� ���� ���� ���������� ���� / �ʱ�ȭ
const int HEIGHT_MAX = 20;          // �ִ� ���� ���� ���������� ���� / �ʱ�ȭ

int start[2];                       // ���� ���� ���� ������ ����
int goal[2];                        // ���� ���� ���� ������ ����
char map[HEIGHT_MAX][WIDTH_MAX];    // �� ���� ������ ����

int came_from[HEIGHT_MAX][WIDTH_MAX][2];    // �� ���� �迭 ����

int calc_cost(int* pos);                                        // ��� ��� �Լ� ����
int is_valid(int* pos, void* is_visit, int width, int height);  // �� �� �ִ� ������ ����ϴ� �Լ� ����
int cmpfunc(const void* a, const void* b);                      // ������ ���� ũ�� �� �Լ� ����
void search(int width, int height);                             // Ž�� �Լ� ����

void _print_map(int width, int height, int (*path)[2], int path_len);   // �� ��� �Լ� ����


int main() {
    // ���� ���� ũ�� ���� ����
    int width, height;

    // ���� ���� ũ�� �Է�
    system("cls");
    printf("���ο� ���� ũ�⸦ �Է��ϼ���.\n�ּ� ũ��� 1 1, �ִ� ũ��� %d %d �Դϴ�. \n\n �Է�: ", WIDTH_MAX, HEIGHT_MAX);
    scanf("%d %d", &width, &height);
    system("cls");
    
    // ���� ���� ũ�⿡ �°� �� �Է�
    printf("���� �Է����ּ���.\n");
    printf("\t.: �� ����\n");
    printf("\tO: �������\n");
    printf("\tX: ��������\n");
    printf("\t@: ���� ��\n\n");
    printf("�� : \n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            scanf(" %c", &map[i][j]);

            // ���� �Է� ���ڰ� O��� ���� ���� ����
            if (map[i][j] == 'O') {
                start[0] = i;
                start[1] = j;
            }
            // ���� �Է� ���ڰ� X��� ���� ���� ����
            if (map[i][j] == 'X') {
                goal[0] = i;
                goal[1] = j;
            }
        }
    }
    system("cls");
    
    // �Է¹��� ����, ����, ���� �̿��Ͽ� Ž��
    search(width, height);

    // ���� ã�� ���� �迭 ����, �ش� �迭�� ���� ���� / �ʱ�ȭ
    int path[WIDTH_MAX * HEIGHT_MAX][2];
    int path_len = 0;

    // came_from �迭�� ���� �� ã��
    // ���� ������ ���� ���� ��ġ, �ش� ��ġ�� ���� ���� ��ġ... �ݺ�
    int temp[2] = { came_from[goal[0]][goal[1]][0], came_from[goal[0]][goal[1]][1] };

    // temp�� ������ġ�� �� ������ �ݺ�
    while ((temp[0] != start[0]) || (temp[1] != start[1])) {
        // �ӽ� ���� ���� / �ʱ�ȭ
        int y = temp[0];
        int x = temp[1];

        // ã�� ���� path �迭�� ����, �迭�� ���� += 1
        path[path_len][0] = y;
        path[path_len][1] = x;
        path_len += 1;

        // temp�� ���� ���� ��ġ�� temp ����
        temp[0] = came_from[y][x][0];
        temp[1] = came_from[y][x][1];
    }

    // ã�� ���� ������� �� ���
    _print_map(width, height, path, path_len);
    
    // �Ͻ�����
    system("pause");
    return 0;
}


// ��� ��� �Լ� ����
/*
    �Ķ����:
        ������ġ

    �Լ�����:
        ������ġ�� ���� ������������ �Ÿ��� ������������ �Ÿ��� ���� ��ȯ
        �Ÿ��� ���� �� ��Ŭ���� �Ÿ��� �ƴ� L1 norm�� �̿���
*/
int calc_cost(int* pos) {
    return abs(start[0] - pos[0]) + abs(start[1] - pos[1]) + abs(goal[0] - pos[0]) + abs(goal[1] - pos[1]);
}


// �� �� �ִ� ������ �Ǵ��ϴ� �Լ� ����
/*
    �Ķ����:
        ������ġ, �湮����, ����ũ��, ����ũ��

    �Լ�����:
        ������ġ�� 0���� �۰ų� ����ũ��, ����ũ�⸦ ����� False(0) ��ȯ
        ������ġ���� �湮������ True(1) �̶�� False(0) ��ȯ
        ������ġ�� ���� ������ False(0) ��ȯ
        �� ������ ��� �������� �ʴ´ٸ� True(1) ��ȯ
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


// ������ ���� ũ�� �� �Լ� ����
/*
    �Ķ����:
        ù��° ���� a, �ι�° ���� b

    �Լ�����:
        �������� ������ ���� a�� ũ�ٸ� ������ b�� ũ�ٸ� ����� ��ȯ
        �ڼ��� ������ https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/reference/qsort?view=msvc-170 ����
*/
int cmpfunc(const void* a, const void* b) {
    return calc_cost((int*)b) - calc_cost((int*)a);
}


// Ž�� �Լ� ����
/*
    �Ķ����:
        ����ũ��, ����ũ��

    �Լ�����:
        a* �˰����� Ȱ���Ͽ� ���� ã�� �Լ�
        Ž���� ���� came_from �迭�� �����Ͽ� ������������ ���� ���� ã�� �� �ְ� ����
*/
void search(int width, int height) {
    // ������ �湮 �� ��ġ�� ������ �迭 ����, �ش� �迭�� ���� ���� / �ʱ�ȭ
    int memory[width * height][2];
    int memory_len = 0;

    // �ش� ��ġ�� �湮�ߴ��� ���� �� �迭 ���� / �ʱ�ȭ
    int is_visit[HEIGHT_MAX][WIDTH_MAX] = { 0, };

    // ���� ��ġ ����
    int pos[2];

    // ���� ��ġ���� �� �� �ִ� ���� ���� / �ʱ�ȭ
    const int dir_x[4] = { 0, 0, -1, 1 };
    const int dir_y[4] = { -1, 1, 0, 0 };

    // ���� �湮 �� ��ġ�� ���� ��ġ�� �ְ� �迭 ���� += 1
    memory[0][0] = start[0];
    memory[0][1] = start[1];
    memory_len = 1;

    // ������ �湮 �� ��ġ�� ���ų� ������ġ�� ���������� ������ �ݺ�
    while (memory_len != 0 && (pos[0] != goal[0] || pos[1] != goal[1])) {
        // ������ġ�� ���� �湮�� ��ġ�� ������ ���� ����, �迭�� ���� -= 1
        pos[0] = memory[memory_len-1][0];
        pos[1] = memory[memory_len-1][1];
        memory_len -= 1;

        // ������ġ�� �湮 ǥ��
        is_visit[pos[0]][pos[1]] = 1;

        // ��� ���� �ݺ�
        for (int i = 0; i < 4; i++) {
            // ���� ��ġ �ʱ�ȭ / ����
            int next_pos[2] = { pos[0] + dir_x[i], pos[1] + dir_y[i] };

            // ���� ���� ��ġ�� �� �� �ִ� ��ġ���
            if (is_valid(next_pos, is_visit, width, height)) {
                // ��� ���� / �ʱ�ȭ
                int cost = calc_cost(next_pos);
                
                // �湮 �� ��ġ�� ���� ��ġ �߰�, �迭�� ���� += 1
                memory[memory_len][0] = next_pos[0];
                memory[memory_len][1] = next_pos[1];
                memory_len += 1;

                // ����� ���� ������� Ž���ؾ� �ϹǷ� memory�� ��뿡 ���� �������� ����, ������ ���
                qsort(memory, memory_len, 2*sizeof(int), cmpfunc);
                
                // ���� ���� �� �� �ֵ��� came_from �迭���� ������ġ�� ������ġ�� ����
                came_from[next_pos[0]][next_pos[1]][0] = pos[0];
                came_from[next_pos[0]][next_pos[1]][1] = pos[1];
            }
        }
    }
}


// �� ��� �Լ� ����
/*
    �Ķ����:
        ����ũ��, ����ũ��, ǥ�� �� ��, �� �迭�� ����

    �Լ�����:
        �ð������� �� ���� �� �ְ� �ؽ�Ʈ ��� ������ �������� �����
        ���� ���, ���������� ���������� ������, ���� �ʷϻ����� ǥ��
*/
void _print_map(int width, int height, int (*path)[2], int path_len) {
    // ���� ��� ��� ����
    for (int i = -1; i <= height; i++) {
        // ���⸸ ����ϸ� �� �ٲ� �� ����� ������Ƿ� .�� ������ ��濡 ��� ���ڷ� ���
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        printf(".");

        for (int j = -1; j <= width; j++) {
            // ������ġ�� ������ �Ǵ�
            int is_path = 0;

            // ������ġ�� path�迭�� ���ԵǾ� �ִٸ� �ʷϻ� ���
            for (int k = 0; k < path_len; k++) {
                if (path[k][0] == i && path[k][1] == j) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xAA);
                    printf("  ");
                    is_path = 1;
                    break;
                }
            }

            // ���� ������ġ�� ���� �ƴ϶��
            if (!is_path) {
                // �Ʒ� ������ �����ϸ� �� ���
                if (i < 0 || j < 0 || i >= height || j >= width) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xFF);
                    printf("  ");
                }
                // �Ʒ� ������ �����ϸ� �� ���
                else if (map[i][j] == '@') {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xFF);
                    printf("  ");
                }
                // �Ʒ� ������ �����ϸ� ���� ��� ���
                else if (map[i][j] == '.') {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00);
                    printf("  ");
                }
                // �Ʒ� ������ �����ϸ� �����, ������ ���
                else if (map[i][j] == 'O' || map[i][j] == 'X') {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xCC);
                    printf("  ");
                }
            }
            
        }

        // ���⸸ ����ϸ� �� �ٲ� �� ����� ������Ƿ� .�� ������ ��濡 ��� ���ڷ� ���
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        printf(".\n");
    }
}



// ���� �Է�:
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

// comment : ����ó�� �������� �ߴµ� �ʹ� �����Ƽ� �׳� �߸� �Է��ϸ� �˾Ƽ� �������� �ϰ� ����