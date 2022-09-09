#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

#define ALPHABET_LENGTH 42
#define TRIGRAMS_TO_WRITE 3000

const wchar_t *allLetters = L"аәбвгғдеёжзийкқлмнңоөпрстуұүфхһцчшщъыіьэюяАӘБВГҒДЕЁЖЗИЙКҚЛМНҢОӨПРСТУҰҮФХҺЦЧШЩЪЫІЬЭЮЯ";
int trigramIndex = 0;

typedef struct TrigramWithCount
{
    wchar_t trigram[4];
    unsigned int count;
} TrigramWithCount;

TrigramWithCount stats[71000];

int compare(const void *v1, const void *v2)
{
    const TrigramWithCount *p1 = (TrigramWithCount *)v1;
    const TrigramWithCount *p2 = (TrigramWithCount *)v2;
    if (p1->count < p2->count) return 1;
    else if (p1->count > p2->count) return -1;
    else return 0;
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
            break;
        }
    }
}

int main()
{
    setlocale(LC_CTYPE,"en_US.UTF-8");
    FILE *pFile;
    FILE *destFile;
    pFile = fopen("sample.txt", "r");
    destFile = fopen("countResults.txt", "w");

    wchar_t currentChar = L'\0';
    wchar_t first = L'!';
    wchar_t second = L'!';
    wchar_t third = L'!';
    wchar_t currentTrigram[4] = {L'\0'};
    while((currentChar = fgetwc(pFile)) != WEOF)
    {
        if((wcschr(allLetters, first) != NULL) && (wcschr(allLetters, second) != NULL) && (wcschr(allLetters, third) != NULL))
        {
            currentTrigram[0] = toLowerCase(first);
            currentTrigram[1] = toLowerCase(second);
            currentTrigram[2] = toLowerCase(third);
            addToStats(currentTrigram);
        }
        first = second;
        second = third;
        third = currentChar;
    };
    qsort(stats, trigramIndex + 1, sizeof(TrigramWithCount), compare);
    for(int i = 0; i < TRIGRAMS_TO_WRITE; i++)
    {
        fwprintf(destFile, L"%ls: %d\n", stats[i].trigram, stats[i].count);
    }
    fclose(pFile);
    fclose(destFile);
    return 0;
}