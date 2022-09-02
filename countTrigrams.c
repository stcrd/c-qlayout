#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

const wchar_t *allLetters = L"аәбвгғдеёжзийкқлмнңоөпрстуұүфхһцчшщъыіьэюяАӘБВГҒДЕЁЖЗИЙКҚЛМНҢОӨПРСТУҰҮФХҺЦЧШЩЪЫІЬЭЮЯ";
const int ALPHABET_LENGTH = 42;
const int TRIGRAMS_TO_WRITE = 1000;
int trigramIndex = 0;

typedef struct _TrigramWithCount_
{
    wchar_t trigram[4];
    unsigned int count;
}TrigramWithCount;

TrigramWithCount stats[74090];

int comparator(const void *v1, const void *v2)
{
    const TrigramWithCount *p1 = (TrigramWithCount *)v1;
    const TrigramWithCount *p2 = (TrigramWithCount *)v2;
    if (p1->count < p2->count)
        return 1;
    else if (p1->count > p2->count)
        return -1;
    else
        return 0;
}

int areValidLetters(wchar_t first, wchar_t second, wchar_t third)
{
    return (wcschr(allLetters, first) && wcschr(allLetters, second) && wcschr(allLetters, third));
}

wchar_t toLowerCase(wchar_t letter)
{
    return allLetters[(wcschr(allLetters, letter) - allLetters) % ALPHABET_LENGTH];
}

void addToStats(wchar_t *trigram)
{
    for(int i = 0; i <= trigramIndex; i++)
    {
        if(wcscmp(stats[i].trigram, trigram) == 0)
        {
            stats[i].count++;
            break;
        }
        if(i == trigramIndex)
        {
            wcscpy(stats[i + 1].trigram, trigram);
            stats[i + 1].count = 1;
            trigramIndex++;
        }
    }
}

int main()
{
    FILE *pFile;
    FILE *destFile;
    wchar_t currentChar;
    
    setlocale(LC_CTYPE,"en_US.UTF-8");
    pFile = fopen("sample.txt", "r");
    destFile = fopen("countResults.txt", "w");
    wchar_t first, second, third;
    wchar_t currentTrigram[4];
    while((currentChar = fgetwc(pFile)) != WEOF)
    {
        if(areValidLetters(first, second, third) == 0)
        {
            wchar_t currentTrigram[4] = {toLowerCase(first), toLowerCase(second), toLowerCase(third), L'\0'};
            addToStats(currentTrigram);
        }
        first = second;
        second = third;
        third = currentChar;
    };
    qsort(stats, trigramIndex + 1, sizeof(TrigramWithCount), comparator);
    for(int i = 0; i < TRIGRAMS_TO_WRITE; i++)
    {
        fwprintf(destFile, L"%ls: %d\n", stats[i].trigram, stats[i].count);
    }
    fclose(pFile);
    fclose(destFile);
    return 0;
}