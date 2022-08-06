#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

const int LETTER_COUNT = 42;
int main()
{
    static wchar_t *allLetters = L"аәбвгғдеёжзийкқлмнңоөпрстуұүфхһцчшщъыіьэюяАӘБВГҒДЕЁЖЗИЙКҚЛМНҢОӨПРСТУҰҮФХҺЦЧШЩЪЫІЬЭЮЯ";
    FILE *pFile;
    wchar_t currentChar;
    int *freq = malloc(sizeof(int) * LETTER_COUNT * 2);
    for(int i = 0; i < LETTER_COUNT * 2; i++)
    {
        freq[i] = 0;
    }
    int index = LETTER_COUNT * 2;
    setlocale(LC_CTYPE,"en_US.UTF-8");
    pFile = fopen("sample.txt", "r");

    while((currentChar = fgetwc(pFile)) != WEOF)
    {
        if(wcschr(allLetters, currentChar)) index = wcschr(allLetters, currentChar) - allLetters;
        if(index < LETTER_COUNT * 2) freq[index]++;
    };
    for(int i = 0; i < LETTER_COUNT; i++)
    {
        wprintf(L"%lc:  %7d times\n", allLetters[i], freq[i] + freq[i + LETTER_COUNT]);
    }
    free(freq);
    fclose(pFile);
    return 0;
}