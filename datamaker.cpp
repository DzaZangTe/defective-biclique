#include <cstdio>
#include <cstdlib>
#include <Windows.h>

int n1, n2, m, k;
bool b[20][20], s[1 << 20], t[1 << 20];

bool validate(const int x)
{
    int cnt = 0;
    for (int i = 0; i < n1; ++i)
        for (int j = 0; j < n2; ++j)
            if ((x & (1 << i)) != 0 && (x & (1 << (j + n1))) != 0 && !b[i][j])
                ++cnt;
    return cnt <= k;
}

int main(void)
{
    srand(GetTickCount());
    n1 = 40, n2 = 40, m = 0, k = 2;
    for (int i = 0; i < n1; ++i)
        for (int j = 0; j < n2; ++j) {
            m += (b[i][j] = (rand() % 5 == 0));
        }
    freopen("data.in", "w", stdout);
    printf("%d %d %d %d\n", n1, n2, m, k);
    for (int i = 0; i < n1; ++i)
        for (int j = 0; j < n2; ++j)
            if (b[i][j])
                printf("%d %d\n", i, j + n1);
    // for (int i = 0; i < (1 << (n1 + n2)); ++i) {
    //     s[i] = false;
    //     t[i] = false;
    //     if (validate(i)) {
    //         s[i] = true;
    //         for (int j = 0; j < n1 + n2; ++j)
    //             if ((i & (1 << j)) != 0)
    //                 s[i ^ (1 << j)] = false;
    //     }
    // }
    // freopen("1.ans", "w", stdout);
    // for (int i = 0; i < (1 << (n1 + n2)); ++i)
    //     if (s[i]) {
    //         for (int j = 0; j < n1 + n2; ++j)
    //             if ((i & (1 << j)) > 0)
    //                 printf("%d, ", j);
    //         printf("\n");
    //     }
    return 0;
}
