#include <wchar.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <bsd/stdlib.h>
#include "effort.h"

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