#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) {
        return 0;
    }

    long long counts[26];
    for (int i = 0; i < 26; ++i) counts[i] = 0;

    long long total = 0;
    for (int i = 0; i < n; ++i) {
        char type;
        long long qty;
        if (scanf(" %c %lld", &type, &qty) != 2) {
            break;
        }
        if (type >= 'a' && type <= 'z') type = (char)(type - 'a' + 'A');
        if (type < 'A' || type > 'Z') continue;
        counts[type - 'A'] += qty;
        total += qty;
    }

    for (int i = 0; i < 26; ++i) {
        if (counts[i] <= 0) continue;
        long double r = (long double)counts[i] * 100.0L / (long double)total;
        long long centi = llroundl(r); // HALF UP for positives
        printf("%c(%.2f)\n", (char)('A' + i), centi / 100.0);
    }

    return 0;
}

