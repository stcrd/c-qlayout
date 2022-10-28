#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <bsd/stdlib.h>

#define LIMIT 3.73000
#define ALPHABET_LENGTH 42
#define PARALLELRUNS 1
#define ANALYZEDTRIGRAMS 1000
// #define ALL_POSSIBLE_TRIGRAM_LAYOUTS 74088 // 42*42*42
#define MAX_TRIGRAM_INDEX 220454

const wchar_t alphabet[] = L"аәбвгғдеёжзийкқлмнңоөпрстуұүфхһцчшщъыіьэюя";
const char allPositions[] = "1234567890-=qwertyuiop[]asdfghjkl;'zxcvbnm";
// -----------
// Base factors
// -----------
// pr = row penalty
// pf = finger penalty
// bke = baseline key effort
// rows: 1.0 = number row; 2.0 = top row; 3.0 = home row; 4.0 = bottom row
// hands: 1.0 = left; 2.0 = right
// fingers: 0.0 = left pinky; 1.0 = left ring; 2.0 = left middle; 3.0 = left first;
// fingers: 6.0 = left first; 7.0 = left middle; 8.0 = left ring; 9.0 = left pinky;

// order of factors in the factor array: pr, pf, bke, row, hand, finger

// f - fingers, k - current positions
double calcFingerFactor(double *f, char *k)
{
    bool allFingersSame = (f[0] == f[1]) && (f[1] == f[2]) && (f[0] == f[2]);
    bool allFingersDifferent = (f[0] != f[1]) && (f[1] != f[2]) && (f[0] != f[2]);
    bool monotonicProgression = (f[0] <= f[1] && f[1] <= f[2]) || (f[0] >= f[1] && f[1] >= f[2]);
    bool allKeysDifferent = (k[0] != k[1]) && (k[1] != k[2]) && (k[0] != k[2]);
    if (allFingersSame) return allKeysDifferent ? 7.0 : 5.0;
    if (allFingersDifferent) return monotonicProgression ? 0.1 : 3.0;
    if (allKeysDifferent) return monotonicProgression ? 6.0 : 4.0;
    return monotonicProgression ? 1.0 : 2.0;
}

double calcTrigramTE(int *trigramPositions)
{
    const double baseFactors[][6] = {
                    {2.5, 0.5, 5.0, 1.0, 1.0, 1.0},
                    {2.5, 0.5, 4.0, 1.0, 1.0, 1.0},
                    {2.5, 0.1, 4.0, 1.0, 1.0, 2.0},
                    {2.5, 0.1, 4.0, 1.0, 1.0, 3.0},
                    {2.5, 0.1, 3.5, 1.0, 1.0, 3.0},
                    {2.5, 0.1, 4.5, 1.0, 1.0, 3.0},
                    {2.5, 0.1, 4.0, 1.0, 2.0, 6.0},
                    {2.5, 0.1, 4.0, 1.0, 2.0, 7.0},
                    {2.5, 0.1, 4.0, 1.0, 2.0, 7.0},
                    {2.5, 0.5, 4.0, 1.0, 2.0, 8.0},
                    {2.5, 1.0, 4.0, 1.0, 2.0, 9.0},
                    {2.5, 1.0, 4.5, 1.0, 2.0, 9.0},
                    {0.5, 1.0, 2.0, 2.0, 1.0, 0.1},
                    {0.5, 0.5, 2.0, 2.0, 1.0, 1.0},
                    {0.5, 0.1, 2.0, 2.0, 1.0, 2.0},
                    {0.5, 0.1, 2.0, 2.0, 1.0, 3.0},
                    {0.5, 0.1, 2.5, 2.0, 1.0, 3.0},
                    {0.5, 0.1, 3.0, 2.0, 2.0, 6.0},
                    {0.5, 0.1, 2.0, 2.0, 2.0, 6.0},
                    {0.5, 0.1, 2.0, 2.0, 2.0, 7.0},
                    {0.5, 0.5, 2.0, 2.0, 2.0, 8.0},
                    {0.5, 1.0, 2.0, 2.0, 2.0, 9.0},
                    {0.5, 1.0, 2.5, 2.0, 2.0, 9.0},
                    {0.5, 1.0, 4.0, 2.0, 2.0, 9.0},
                    {0.1, 1.0, 0.1, 3.0, 1.0, 0.1},
                    {0.1, 0.5, 0.1, 3.0, 1.0, 1.0},
                    {0.1, 0.1, 0.1, 3.0, 1.0, 2.0},
                    {0.1, 0.1, 0.1, 3.0, 1.0, 3.0},
                    {0.1, 0.1, 2.0, 3.0, 1.0, 3.0},
                    {0.1, 0.1, 2.0, 3.0, 2.0, 6.0},
                    {0.1, 0.1, 0.1, 3.0, 2.0, 6.0},
                    {0.1, 0.1, 0.1, 3.0, 2.0, 7.0},
                    {0.1, 0.5, 0.1, 3.0, 2.0, 8.0},
                    {0.1, 1.0, 0.1, 3.0, 2.0, 9.0},
                    {0.1, 1.0, 2.0, 3.0, 2.0, 9.0},
                    {1.0, 1.0, 2.0, 4.0, 1.0, 0.1},
                    {1.0, 0.5, 2.0, 4.0, 1.0, 1.0},
                    {1.0, 0.1, 2.0, 4.0, 1.0, 2.0},
                    {1.0, 0.1, 2.0, 4.0, 1.0, 3.0},
                    {1.0, 0.1, 3.5, 4.0, 1.0, 3.0},
                    {1.0, 0.1, 2.0, 4.0, 2.0, 6.0},
                    {1.0, 0.1, 2.0, 4.0, 2.0, 6.0},
                    };

    // Efforts factors
    const double k1 = 1.0, k2 = 0.367, k3 = 0.235, kb = 0.3555, kp = 0.6423, ks = 0.4268;
    // Weight factors
    const double wr = 1.3088, wf = 2.5948;
    // These were chosen for English in the carpalx model, maybe different for other languages
    const double fh = 1.0, fr = 0.3, ff = 0.3;
    double b[3]; // base efforts
    double p[3]; // position penalty factors
    double r[3]; // rows
    double h[3]; // hands
    double f[3]; // fingers
    char k[3]; // current positions for calculating finger factors

    for(int i=0;i<3;i++)
    {
        int positionIndex = trigramPositions[i];
        // record current positions for later use
        k[i] = allPositions[positionIndex];
        // get base efforts for each letter in a trigram
        b[i] = baseFactors[positionIndex][2];
        // get penalties for each letter in a trigram
        p[i] = wr * baseFactors[positionIndex][0] + wf * baseFactors[positionIndex][1];
        // get rows
        r[i] = baseFactors[positionIndex][3];
        // get hands
        h[i] = baseFactors[positionIndex][4];
        // get fingers
        f[i] = baseFactors[positionIndex][5];
    }
    double pr = fabs(r[1] - r[0]) + fabs(r[2] - r[1]);
    // hand factors
    // 0: both used, not alternating
    // 1: alternating
    // 2: same
    double ph;
    if(fabs(h[1] - h[0]) + fabs(h[2] - h[1]) == 0) ph = 2.0;
    else if(fabs(h[1] - h[0]) + fabs(h[2] - h[1]) == 1) ph = 0.1;
    else ph = 1.0;

    double pf = calcFingerFactor(f, k);

    double base = k1 * b[0] * (1 + k2 * b[1] * (1 + k3 * b[2]));
    double penalty = k1 * p[0] * (1 + k2 * p[1] * (1 + k3 * p[2]));
    double strokePath = pr * fr + ph * fh + pf * ff;
    double result = kb * base + kp * penalty + ks * strokePath;
    return result;
}

typedef struct KazTrigramStat
{
    wchar_t kazTrigram[4];
    int count;
} KazTrigramStat;

KazTrigramStat stats[ANALYZEDTRIGRAMS];

void getStats()
{
    FILE *fp = fopen("countResults.txt", "r");
    wchar_t trigram[4];
    int count;
    // collect analysis results from a file into stats struct
    if(fp == NULL) wprintf(L"Error opening countResults.txt, please check it exists.\n");
    else
    {
        for(int i=0;i<ANALYZEDTRIGRAMS+1;i++)
        {
            fwscanf(fp, L"%3ls: %d", trigram, &count);
            wcscpy(stats[i].kazTrigram, trigram);
            stats[i].count = count;
        }
    }
    fclose(fp);
}

typedef struct TrigramEffort
{
    double effort;
    char positionTrigram[4];
} TrigramEffort;

// int compare(const void *v1, const void *v2)
// {
//     const TrigramEffort *p1 = (TrigramEffort *)v1;
//     const TrigramEffort *p2 = (TrigramEffort *)v2;
//     if (p1->effort > p2->effort) return 1;
//     else if (p1->effort < p2->effort) return -1;
//     else return 0;
// }

TrigramEffort preCalcEfforts[MAX_TRIGRAM_INDEX];

void calcAllLayoutEffortsForTrigram()
{
    double result = 0.1;
    int i, j, k;
    int index = 0;
    double currentEffort;
    // FILE *fp = fopen("preCalcEfforts.txt", "w");
    for(i=0;i<42;i++)
    {
        for(j=0;j<42;j++)
        {
            for(k=0;k<42;k++)
            {
                int currentPositionIndices[] = {i, j, k};
                char currentPositionTrigram[] = {allPositions[i], allPositions[j], allPositions[k], '\0'};
                currentEffort = calcTrigramTE(currentPositionIndices);
                index = allPositions[i] * 42 * 42 + allPositions[j] * 42 + allPositions[k];
                preCalcEfforts[index].effort = currentEffort;
                strcpy(preCalcEfforts[index].positionTrigram, currentPositionTrigram);
            }
        }
    }
    // qsort(preCalcEfforts, ALL_POSSIBLE_TRIGRAM_LAYOUTS, sizeof(TrigramEffort), compare);
    // fprintf(fp, "Index   |   Effort  |  Positions\n------------------------------\n");
    // for(int n=0;n<MAX_TRIGRAM_INDEX;n++)
    // {
    //     fprintf(fp, "%6d  |  %s  |  %.4f\n", n, preCalcEfforts[n].positionTrigram, preCalcEfforts[n].effort);
    // }
    // fclose(fp);
}

char * generateRandomLayout(char *layout)
{
    int lng = ALPHABET_LENGTH;
    char positionsLeft[] = "1234567890-=qwertyuiop[]asdfghjkl;'zxcvbnm";
    for(int i=0;i<ALPHABET_LENGTH;i++)
    {
        int r = arc4random_uniform(lng);
        layout[i] = positionsLeft[r];
        for(int j=r+1;j<lng && r != lng - 1;j++) positionsLeft[j-1] = positionsLeft[j];
        lng--;
    }
    return layout;
}

char * generateNewLayout(char pLayout[])
{
    char tmp;
    int index1 = arc4random_uniform(ALPHABET_LENGTH);
    int index2 = arc4random_uniform(ALPHABET_LENGTH);
    tmp = pLayout[index1];
    pLayout[index1] = pLayout[index2];
    pLayout[index2] = tmp;
    return pLayout;
}

double calcLayoutEffort(char *layout)
{
    double result = 0.0;
    int totalTrigrams = 0;
    double trigramTE;
    char first, second, third;
    int preCalcIndex;
    
    for(int n=0;n<ANALYZEDTRIGRAMS;n++)
    {
        totalTrigrams += stats[n].count;
        first = layout[(wcschr(alphabet, stats[n].kazTrigram[0]) - alphabet)];
        second = layout[(wcschr(alphabet, stats[n].kazTrigram[1]) - alphabet)];
        third = layout[(wcschr(alphabet, stats[n].kazTrigram[2]) - alphabet)];
        preCalcIndex = first * 42 * 42 + second * 42 + third;
        trigramTE = preCalcEfforts[preCalcIndex].effort;
        result += trigramTE * stats[n].count;
    }
    return result / totalTrigrams;
}

typedef struct OptimalLayout
{
    char bestLayout[43];
    double bestEffort;
} OptimalLayout;

OptimalLayout findOptimalLayout(char *startingLayout)
{
    char emptyLayout[ALPHABET_LENGTH + 1] = {L'0'};
    // Optimization parameters
    const int p0 = 1, temp = 10, cooling = 10, target = 2.0;
    int cycles = 0, attempts = 0;
    double epsilon = 0.1;
    char bestLayout[43], prevLayout[43], tempLayout[43];
    strcpy(bestLayout, startingLayout);
    double bestEffort = calcLayoutEffort(bestLayout);
    strcpy(prevLayout, bestLayout);
    double prevEffort = bestEffort;
    char *newLayout;
    double t0 = temp; // temp - const declared in the beginning of the file
    while(fabs(bestEffort - target) > epsilon)
    {
        if(attempts > 1000) break;
        strcpy(tempLayout, prevLayout);
        if(cycles > 10000)
        {
            newLayout = generateRandomLayout(emptyLayout);
            cycles = 0;
            attempts++;
        }
        else
        {
            newLayout = generateNewLayout(tempLayout);
            cycles++;
        }
        double newEffort = calcLayoutEffort(newLayout);
        double dE = newEffort - prevEffort;
        // double ti = t0 * exp((-i * cooling));
        // double pi = p0 * exp(-dE / ti);
        // t0 = ti;
        double r = arc4random() / (double) UINT32_MAX;
        // if(r < pi)
        // {
            if(newEffort < bestEffort)
            {
                strcpy(bestLayout, newLayout);
                bestEffort = newEffort;
            } else
            {
                prevEffort = newEffort;
            }
            strcpy(prevLayout, newLayout);
        // } else {
        //     continue;
        // }
        cycles++;
    }
    OptimalLayout result;
    strcpy(result.bestLayout, bestLayout);
    result.bestEffort = bestEffort;
    printf("Cycles: %d\n", cycles);
    return result;
}

int main()
{
    setlocale(LC_CTYPE,"en_US.UTF-8");
    getStats();
    calcAllLayoutEffortsForTrigram();
    char randomLayouts[PARALLELRUNS][ALPHABET_LENGTH+1];
    char emptyLayout[ALPHABET_LENGTH + 1] = {L'0'};
    OptimalLayout optimalLayouts[PARALLELRUNS];
    fclose(fopen("optimalLayout.txt", "w"));
    FILE* fp = fopen("optimalLayout.txt", "a");
    for(int i=0;i<PARALLELRUNS;i++)
    {
        // wprintf(L"Entered the cycle\n");
        strcpy(randomLayouts[i], generateRandomLayout(emptyLayout));
        // wprintf(L"Generated a random layout\n");
        optimalLayouts[i] = findOptimalLayout(randomLayouts[i]);
        // wprintf(L"Found the optimal layout\n");
        fprintf(fp, "%03d | %s: %.2f\n", i, optimalLayouts[i].bestLayout, optimalLayouts[i].bestEffort);
        // wprintf(L"Completed the current cycle\n");
    }
    fclose(fp);
    exit(0);
}