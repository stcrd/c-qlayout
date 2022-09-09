#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <bsd/stdlib.h>

#define ALPHABET_LENGTH 42
#define ANALYZEDTRIGRAMS 3000
#define ALL_POSSIBLE_TRIGRAM_LAYOUTS 74088 // 42*42*42

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
// fingers: 0.1 = left pinky; 1.0 = left ring; 2.0 = left middle; 3.0 = left first;
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

int compare(const void *v1, const void *v2)
{
    const TrigramEffort *p1 = (TrigramEffort *)v1;
    const TrigramEffort *p2 = (TrigramEffort *)v2;
    if (p1->effort > p2->effort) return 1;
    else if (p1->effort < p2->effort) return -1;
    else return 0;
}

TrigramEffort allTrigramEfforts[ALL_POSSIBLE_TRIGRAM_LAYOUTS];

void calcAllLayoutEffortsForTrigram()
{
    double result = 0.1;
    int i, j, k;
    int index = 0;
    double currentEffort;
    FILE *fp = fopen("allTrigramEfforts.txt", "w");
    for(i=0;i<42;i++)
    {
        for(j=0;j<42;j++)
        {
            for(k=0;k<42;k++)
            {
                int currentPositionIndices[] = {i, j, k};
                char currentPositionTrigram[] = {allPositions[i], allPositions[j], allPositions[k], '\0'};
                currentEffort = calcTrigramTE(currentPositionIndices);
                allTrigramEfforts[index].effort = currentEffort;
                strcpy(allTrigramEfforts[index].positionTrigram, currentPositionTrigram);
                index++;
            }
        }
    }
    qsort(allTrigramEfforts, ALL_POSSIBLE_TRIGRAM_LAYOUTS, sizeof(TrigramEffort), compare);
    fprintf(fp, "Effort  |  Positions\n------------------------------\n");
    for(int n=0;n<ALL_POSSIBLE_TRIGRAM_LAYOUTS;n++)
    {
        fprintf(fp, "%.4f  |  %s\n", allTrigramEfforts[n].effort, allTrigramEfforts[n].positionTrigram);
    }
    fclose(fp);
}

void assignTrigrams(char *resultLayout)
{
    setlocale(LC_CTYPE,"en_US.UTF-8");
    getStats();
    for(int i=0;i<ANALYZEDTRIGRAMS;i++)
    {
        // break if all positions are set
        if(strchr(resultLayout, L'!') == NULL) break;
        char searchedPositions[4];
        for(int j=0;j<ALPHABET_LENGTH;j++)
        {
            for(int t=0;t<3;t++)
            {
                if(stats[i].kazTrigram[t] == alphabet[j]) searchedPositions[t] = resultLayout[j];
            }
        }
        for(int k=0;k<ALL_POSSIBLE_TRIGRAM_LAYOUTS;k++)
        {
            // if not already assigned, then any position not already taken
            // and if already assigned, then only the assigned position
            // order of the positions matters
            bool criterion1 = searchedPositions[0] == L'!' ?
                (strchr(resultLayout, allTrigramEfforts[k].positionTrigram[0]) == NULL)
                : (searchedPositions[0] == allTrigramEfforts[k].positionTrigram[0]);
            bool criterion2 = searchedPositions[1] == L'!' ?
                (strchr(resultLayout, allTrigramEfforts[k].positionTrigram[1]) == NULL)
                : (searchedPositions[1] == allTrigramEfforts[k].positionTrigram[1]);
            bool criterion3 = searchedPositions[2] == L'!' ?
                (strchr(resultLayout, allTrigramEfforts[k].positionTrigram[2]) == NULL)
                : (searchedPositions[2] == allTrigramEfforts[k].positionTrigram[2]);
            bool criterion4 = ((stats[i].kazTrigram[0] != stats[i].kazTrigram[1]) && (stats[i].kazTrigram[1] != stats[i].kazTrigram[2])) ?
                ((allTrigramEfforts[k].positionTrigram[0] != allTrigramEfforts[k].positionTrigram[1])
                && (allTrigramEfforts[k].positionTrigram[1] != allTrigramEfforts[k].positionTrigram[2]))
                : true;
            // when all of the criteria above are met, only then assign the trigram to positions on the layout
            if(criterion1 && criterion2 && criterion3 && criterion4)
            {
                for(int m=0;m<3;m++) resultLayout[(wcschr(alphabet, stats[i].kazTrigram[m]) - alphabet)] = allTrigramEfforts[k].positionTrigram[m];
                break;
            }
        }
    }
    printf("Result Layout: %s\n", resultLayout);
}

double calcLayoutEffort(char *layout)
{
    double result = 0.0;
    int totalTrigrams = 0;
    double trigramTE;
    int trigramIndices[3];
    
    for(int i=0;i<ANALYZEDTRIGRAMS;i++)
    {
        for(int j=0;j<3;j++)
        {
            char engLetter = layout[wcschr(alphabet, stats[i].kazTrigram[j]) - alphabet];
            trigramIndices[j] = strchr(allPositions, engLetter) - allPositions;
        }
        totalTrigrams += stats[i].count;
        trigramTE = calcTrigramTE(trigramIndices);
        result += trigramTE * stats[i].count;
    }
    return result / totalTrigrams;
}
int main()
{
    setlocale(LC_CTYPE,"en_US.UTF-8");
    char resultLayout[ALPHABET_LENGTH+1];
    for(int n=0;n<ALPHABET_LENGTH;n++) resultLayout[n] = L'!';
    getStats();
    calcAllLayoutEffortsForTrigram();
    assignTrigrams(resultLayout);
    double linearEffort = calcLayoutEffort(resultLayout);
    printf("Linearly optimized effort: %.2f\n", linearEffort);
    exit(0);
}