#include <wchar.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <bsd/stdlib.h>

#define N 10000
#define ALPHABET_LENGTH 42
#define PARALLELRUNS 1
#define ANALYZEDTRIGRAMS 1000
#define ALL_POSSIBLE_TRIGRAM_LAYOUTS 74088 // 42^3

const wchar_t alphabet[] = L"аәбвгғдеёжзийкқлмнңоөпрстуұүфхһцчшщъыіьэюя";
const char allPositions[] = "1234567890-=qwertyuiop[]asdfghjkl;'zxcvbnm";

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

TrigramEffort preCalcEfforts[ALL_POSSIBLE_TRIGRAM_LAYOUTS];

int getIndex(char* letters)
{
    int index = 0, finalIndex = 0;
    for(int i=0;i<3;i++)
    {
        int val = letters[i] - 0;
        if(val == 39) index = 1;
        else if(val == 45) index = 2;
        else if(val < 58 && val > 47) index = val - 45;
        else if(val == 59) index = 13;
        else if(val == 61) index = 14;
        else if(val == 91) index = 15;
        else if(val == 93) index = 16;
        else index = val - 80;
        if(i==0) finalIndex += index * 42 * 42; // first position
        else if(i==1) finalIndex += index * 42; // second position
        else finalIndex += index;
    }
    return finalIndex - 1807; // subtract 1807 (42 * 42 + 42 + 1) to avoid empty cells in the beginning of the array
}

void calcAllLayoutEffortsForTrigram()
{
    double result = 0.1;
    int i, j, k;
    int index = 0;
    double currentEffort;
    FILE *fp = fopen("preCalcEfforts.txt", "w");
    for(i=0;i<42;i++)
    {
        for(j=0;j<42;j++)
        {
            for(k=0;k<42;k++)
            {
                int currentPositionIndices[] = {i, j, k};
                char currentPositionTrigram[] = {allPositions[i], allPositions[j], allPositions[k], '\0'};
                currentEffort = calcTrigramTE(currentPositionIndices);
                index = getIndex(currentPositionTrigram);
                preCalcEfforts[index].effort = currentEffort;
                strcpy(preCalcEfforts[index].positionTrigram, currentPositionTrigram);
            }
        }
    }
    // qsort(preCalcEfforts, ALL_POSSIBLE_TRIGRAM_LAYOUTS, sizeof(TrigramEffort), compare);
    fprintf(fp, "Index   |   Effort  |  Positions\n------------------------------\n");
    for(int n=0;n<ALL_POSSIBLE_TRIGRAM_LAYOUTS;n++)
    {
        fprintf(fp, "%6d  |  %s  |  %.4f\n", n, preCalcEfforts[n].positionTrigram, preCalcEfforts[n].effort);
    }
    fclose(fp);
}

char * generateRandomLayout(char *layout)
{
    int lng = ALPHABET_LENGTH;
    char positionsLeft[43];
    strcpy(positionsLeft, allPositions);
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

void generateNeighbors(char pLayout[], char **neighbors)
{
    char tmp;
    char newLayout[43];
    int index1 = arc4random_uniform(ALPHABET_LENGTH);
    for(int i=0;i<ALPHABET_LENGTH;i++)
    {
        printf("I am here! i is: %d\n", i);
        if(i==index1)
        {
            printf("If case before copy\n");
            strcpy(neighbors[i], pLayout);
            printf("If case AFTER copy\n");
        }
        else
        {
            tmp = pLayout[index1];
            strcpy(newLayout, pLayout);
            newLayout[index1] = pLayout[i];
            newLayout[i] = tmp;
            printf("ELSE case before copy\n");
            strcpy(neighbors[i], newLayout);
            printf("ELSE case AFTER copy\n");
        }
    }
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
        char currentPositionTrigram[] = {first, second, third, '\0'};
        preCalcIndex = getIndex(currentPositionTrigram);
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
    // Optimization parameters
    const int p0 = 1, temp = 1000, cooling = 1, steps = N;
    char bestLayout[43], prevLayout[43], tempLayout[43], newLayout[43];
    char *neighbors = (char*)malloc(sizeof(char)*42*43);
    strcpy(bestLayout, startingLayout);
    double bestEffort = calcLayoutEffort(bestLayout);
    strcpy(prevLayout, bestLayout);
    double prevEffort = bestEffort;
    double t0 = temp; // temp - const declared in the beginning of the file
    for(int i=0;i<steps;i++)
    {
        strcpy(tempLayout, prevLayout);
        // char *newLayout = generateNewLayout(tempLayout);
        generateNeighbors(tempLayout, &neighbors);
        printf("Generated neighbors\n");
        double tempLocalEffort, newEffort;
        int neighborIndex;
        newEffort = calcLayoutEffort(&neighbors[0]);
        for(int i=1;i<ALPHABET_LENGTH;i++)
        {
            if(strcmp(tempLayout, &neighbors[i]) == 0) continue;
            tempLocalEffort = calcLayoutEffort(&neighbors[i]);
            if(newEffort > tempLocalEffort)
            {
                newEffort = tempLocalEffort;
                neighborIndex = i;
            }
        }
        strcpy(newLayout, &neighbors[neighborIndex]);
        double dE = newEffort - prevEffort;
        double ti = t0 * exp((-i * cooling) / (double)steps);
        double pi = p0 * exp(-dE / ti);
        t0 = ti;
        double r = arc4random() / (double) UINT32_MAX;
        if(r < pi)
        {
            if(newEffort < bestEffort)
            {
                strcpy(bestLayout, newLayout);
                bestEffort = newEffort;
            } else
            {
                prevEffort = newEffort;
            }
            strcpy(prevLayout, newLayout);
        } else {
            continue;
        }
    }
    OptimalLayout result;
    strcpy(result.bestLayout, bestLayout);
    result.bestEffort = bestEffort;
    free(neighbors);
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