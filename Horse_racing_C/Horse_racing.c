#include <stdio.h>
#include <stdlib.h>

// 比较函数，用于qsort排序
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int main() {
    int T; // 测试数据的组数
    printf("请输入测试数据的组数：");
    scanf("%d", &T);

    while (T--) {
        int n; // 每组测试数据的马的数量
        printf("请输入参赛马的数量：");
        scanf("%d", &n);

        int tianji[n], qiwang[n];
        printf("请输入田忌的马的速度：");
        for (int i = 0; i < n; i++) {
            scanf("%d", &tianji[i]);
        }

        printf("请输入齐王的马的速度：");
        for (int i = 0; i < n; i++) {
            scanf("%d", &qiwang[i]);
        }

        // 对田忌和齐王的马的速度进行排序
        qsort(tianji, n, sizeof(int), compare);
        qsort(qiwang, n, sizeof(int), compare);

        int win = 0, lose = 0, tie = 0;
        int tianji_start = 0, tianji_end = n - 1;
        int qiwang_start = 0, qiwang_end = n - 1;

        while (tianji_start <= tianji_end) {
            if (tianji[tianji_end] > qiwang[qiwang_end]) {
                win++;
                tianji_end--;
                qiwang_end--;
            } else if (tianji[tianji_start] > qiwang[qiwang_start]) {
                win++;
                tianji_start++;
                qiwang_start++;
            } else {
                if (tianji[tianji_start] < qiwang[qiwang_end]) {
                    lose++;
                } else {
                    tie++;
                }
                tianji_start++;
                qiwang_end--;
            }
        }

        int result = (win - lose) * 200;
        printf("田忌最多赢多少两黄金：%d\n", result);
    }

    return 0;
}