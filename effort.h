#ifndef _EFFORT_H_
#define _EFFORT_H_

    #define N 5000
    #define ALPHABET_LENGTH 42
    #define PARALLELRUNS 1
    #define ANALYZEDTRIGRAMS 3000
    #define ALL_POSSIBLE_TRIGRAM_LAYOUTS 74088 // 42*42*42

    extern const wchar_t alphabet[ALPHABET_LENGTH+1];
    extern const char allPositions[ALPHABET_LENGTH+1];

    double calcFingerFactor(double *f, char *k);
    double calcTrigramTE(int *trigramPositions);

#endif