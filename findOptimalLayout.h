#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <bsd/stdlib.h>

double calcFingerFactor(double *f, wchar_t *k);

double calcTrigramTE(wchar_t *trigram, wchar_t *layout);

wchar_t * generateRandomLayout(wchar_t *layout);

wchar_t * generateNewLayout(wchar_t pLayout[]);

void getStats();

double calcLayoutEffort(wchar_t *layout);

typedef struct _OptimalLayout_
{
    wchar_t bestLayout[43];
    double bestEffort;
}OptimalLayout;

OptimalLayout findOptimalLayout(wchar_t *startingLayout);
