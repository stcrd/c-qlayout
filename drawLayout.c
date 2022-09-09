#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>

#define ALPHABET_LENGTH 42
#define WIDTH 1528
#define HEIGHT 528

void svgText(FILE* fp, int x, int y, wchar_t* text, wchar_t* fill, int fontSize);

typedef struct _Coordinate_
{
    wchar_t position;
    int x;
    int y;
} Coordinate;

Coordinate coordinates[42][3] = {
    {L'1', 170, 100},
    {L'2', 270, 100},
    {L'3', 370, 100},
    {L'4', 470, 100},
    {L'5', 570, 100},
    {L'6', 670, 100},
    {L'7', 770, 100},
    {L'8', 870, 100},
    {L'9', 970, 100},
    {L'0', 1070, 100},
    {L'-', 1170, 100},
    {L'=', 1270, 100},
    {L'q', 220, 200},
    {L'w', 320, 200},
    {L'e', 420, 200},
    {L'r', 520, 200},
    {L't', 620, 200},
    {L'y', 720, 200},
    {L'u', 820, 200},
    {L'i', 920, 200},
    {L'o', 1020, 200},
    {L'p', 1120, 200},
    {L'[', 1220, 200},
    {L']', 1320, 200},
    {L'a', 245, 300},
    {L's', 345, 300},
    {L'd', 445, 300},
    {L'f', 545, 300},
    {L'g', 645, 300},
    {L'h', 745, 300},
    {L'j', 845, 300},
    {L'k', 945, 300},
    {L'l', 1045, 300},
    {L';', 1145,300},
    {L'\'', 1245, 300},
    {L'z', 295, 400},
    {L'x', 395, 400},
    {L'c', 495, 400},
    {L'v', 595, 400},
    {L'b', 695, 400},
    {L'n', 795, 400},
    {L'm', 895, 400},
};

void drawLayout(wchar_t* layout, FILE* fp)
{
    wchar_t uppercaseAlphabet[] = L"АӘБВГҒДЕЁЖЗИЙКҚЛМНҢОӨПРСТУҰҮФХҺЦЧШЩЪЫІЬЭЮЯ";

    for(int i=0;i<ALPHABET_LENGTH;i++)
    {
        wchar_t kazLetter[2];
        int x, y;
        for(int j=0;j<ALPHABET_LENGTH;j++)
        {
            if(coordinates[j]->position == layout[i])
            {
                x = coordinates[j]->x;
                y = coordinates[j]->y;
            }
        }
        kazLetter[0] = uppercaseAlphabet[i];
        svgText(fp, x, y, kazLetter, L"brown", 34);
    }
}

void drawColumns(FILE* fp, int count, int x, int y)
{
    const int margin = 10;
    for(int i=1;i<count+1;i++)
    {
        fwprintf(fp, L"    <line stroke='black' stroke-width='2px' y2='%d' x2='%d' y1='%d' x1='%d' />\n", 100+y+margin, i*100+x+margin, y+margin, i*100+x+margin);
    }
}

void svgHeader(FILE* fp, int width, int height)
{
    fwprintf(fp, L"<svg width='%dpx' height='%dpx' xmlns='http://www.w3.org/2000/svg' version='1.1' xmlns:xlink='http://www.w3.org/1999/xlink'>\n", width, height);
}

void svgLine(FILE* fp, int x1, int y1, int x2, int y2)
{
    fwprintf(fp, L"    <line stroke='black' stroke-width='2px' y2='%d' x2='%d' y1='%d' x1='%d' />\n", y2, x2, y1, x1);
}

void svgText(FILE* fp, int x, int y, wchar_t* text, wchar_t* fill, int fontSize)
{
    fwprintf(fp, L"    <text x='%d' y = '%d' font-family='sans-serif' stroke='#000000' fill='%ls' font-size='%dpx'>%ls</text>\n", x, y, fill, fontSize, text);
}

void drawKeyboard(FILE* fp)
{
    svgHeader(fp, WIDTH, HEIGHT);
    fwprintf(fp, L"    <rect fill='gray' stroke='gray' stroke-width='0px' width='1528' height='528' y='0' x='0' ry='0' rx='0' />\n");
    fwprintf(fp, L"    <rect fill='white' stroke='black' stroke-width='4px' width='1500' height='500' y='10' x='10' ry='4' rx='4' />\n");
    // draw row lines
    svgLine(fp, 10, 110, 1510, 110);
    svgLine(fp, 10, 210, 1510, 210);
    svgLine(fp, 10, 310, 1510, 310);
    svgLine(fp, 10, 410, 1510, 410);
    // draw columns on the first 4 rows
    drawColumns(fp, 13, 0, 0);
    drawColumns(fp, 13, 50, 100);
    drawColumns(fp, 12, 75, 200);
    drawColumns(fp, 11, 125, 300);
    // columns on the last row drawn individually
    svgLine(fp, 160, 410, 160, 510);
    svgLine(fp, 260, 410, 260, 510);
    svgLine(fp, 410, 410, 410, 510);
    svgLine(fp, 1010, 410, 1010, 510);
    svgLine(fp, 1160, 410, 1160, 510);
    svgLine(fp, 1260, 410, 1260, 510);
    svgLine(fp, 1360, 410, 1360, 510);
    // first row keys: upper symbols
    svgText(fp, 20, 50, L"~", L"black", 24);
    svgText(fp, 120, 50, L"!", L"black", 24);
    svgText(fp, 220, 50, L"@", L"black", 24);
    svgText(fp, 320, 50, L"#", L"black", 24);
    svgText(fp, 420, 50, L"$", L"black", 24);
    svgText(fp, 520, 50, L"%", L"black", 24);
    svgText(fp, 620, 50, L"^", L"black", 24);
    svgText(fp, 720, 50, L"＆", L"black", 24);
    svgText(fp, 820, 50, L"*", L"black", 24);
    svgText(fp, 920, 50, L"(", L"black", 24);
    svgText(fp, 1020, 50, L")", L"black", 24);
    svgText(fp, 1120, 50, L"_", L"black", 24);
    svgText(fp, 1220, 50, L"+", L"black", 24);
    svgText(fp, 1330, 80, L"⟵", L"black", 24);
    // first row keys: lower symbols
    svgText(fp, 20, 100, L"`", L"black", 24);
    svgText(fp, 120, 100, L"1", L"black", 24);
    svgText(fp, 220, 100, L"2", L"black", 24);
    svgText(fp, 320, 100, L"3", L"black", 24);
    svgText(fp, 420, 100, L"4", L"black", 24);
    svgText(fp, 520, 100, L"5", L"black", 24);
    svgText(fp, 620, 100, L"6", L"black", 24);
    svgText(fp, 720, 100, L"7", L"black", 24);
    svgText(fp, 820, 100, L"8", L"black", 24);
    svgText(fp, 920, 100, L"9", L"black", 24);
    svgText(fp, 1020, 100, L"0", L"black", 24);
    svgText(fp, 1120, 100, L"-", L"black", 24);
    svgText(fp, 1220, 100, L"=", L"black", 24);
    // second row keys
    svgText(fp, 50, 150, L"TAB", L"black", 24);
    svgText(fp, 170, 150, L"Q", L"black", 24);
    svgText(fp, 270, 150, L"W", L"black", 24);
    svgText(fp, 370, 150, L"E", L"black", 24);
    svgText(fp, 470, 150, L"R", L"black", 24);
    svgText(fp, 570, 150, L"T", L"black", 24);
    svgText(fp, 670, 150, L"Y", L"black", 24);
    svgText(fp, 770, 150, L"U", L"black", 24);
    svgText(fp, 870, 150, L"I", L"black", 24);
    svgText(fp, 970, 150, L"O", L"black", 24);
    svgText(fp, 1070, 150, L"P", L"black", 24);
    svgText(fp, 1170, 150, L"{", L"black", 24);
    svgText(fp, 1270, 150, L"}", L"black", 24);
    svgText(fp, 1370, 150, L"|", L"black", 24);
    svgText(fp, 1170, 200, L"[", L"black", 24);
    svgText(fp, 1270, 200, L"]", L"black", 24);
    svgText(fp, 1370, 200, L"\\", L"black", 24);
    // third row keys
    svgText(fp, 50, 250, L"CAPS", L"black", 24);
    svgText(fp, 195, 250, L"A", L"black", 24);
    svgText(fp, 295, 250, L"S", L"black", 24);
    svgText(fp, 395, 250, L"D", L"black", 24);
    svgText(fp, 495, 250, L"F", L"black", 24);
    svgText(fp, 595, 250, L"G", L"black", 24);
    svgText(fp, 695, 250, L"H", L"black", 24);
    svgText(fp, 795, 250, L"J", L"black", 24);
    svgText(fp, 895, 250, L"K", L"black", 24);
    svgText(fp, 995, 250, L"L", L"black", 24);
    svgText(fp, 1095, 240, L":", L"black", 24);
    svgText(fp, 1195, 250, L"\"", L"black", 24);
    svgText(fp, 1095, 290, L";", L"black", 24);
    svgText(fp, 1195, 300, L"\'", L"black", 24);
    svgText(fp, 1315, 250, L"Enter", L"black", 24);
    // fourth row keys
    svgText(fp, 50, 350, L"Shift", L"black", 24);
    svgText(fp, 245, 350, L"Z", L"black", 24);
    svgText(fp, 345, 350, L"X", L"black", 24);
    svgText(fp, 445, 350, L"C", L"black", 24);
    svgText(fp, 545, 350, L"V", L"black", 24);
    svgText(fp, 645, 350, L"B", L"black", 24);
    svgText(fp, 745, 350, L"N", L"black", 24);
    svgText(fp, 845, 350, L"M", L"black", 24);
    svgText(fp, 945, 350, L"＜", L"black", 24);
    svgText(fp, 1045, 350, L"＞", L"black", 24);
    svgText(fp, 1145, 350, L"?", L"black", 24);
    svgText(fp, 945, 390, L",", L"black", 24);
    svgText(fp, 1045, 390, L".", L"black", 24);
    svgText(fp, 1145, 400, L"/", L"black", 24);
    svgText(fp, 1265, 350, L"Shift", L"black", 24);
    // fifth row keys
    svgText(fp, 50, 450, L"Ctrl", L"black", 24);
    svgText(fp, 180, 450, L"Win", L"black", 24);
    svgText(fp, 310, 450, L"Alt", L"black", 24);
    svgText(fp, 1060, 450, L"Alt", L"black", 24);
    svgText(fp, 1180, 450, L"Win", L"black", 24);
    svgText(fp, 1295, 450, L"≣", L"black", 24);
    svgText(fp, 1400, 450, L"Ctrl", L"black", 24);
}

int main(void)
{
    setlocale(LC_CTYPE,"en_US.UTF-8");
    wchar_t layout[43]; // = L"d[u6n'hs8tr7aiwlygbecqfojvpx12-=]m09k;34z5";
    double effort;
    int number;
    FILE *layoutFP;
    FILE *fp;
    layoutFP = fopen("optimalLayout.txt", "r");
    fwscanf(layoutFP, L"%3d | %42ls: %.2f", &number, layout, effort);
    fclose(fopen("newtry-keyboard.svg", "w"));
    fp = fopen("newtry-keyboard.svg", "a");
    drawKeyboard(fp);

    drawLayout(layout, fp);
    fwprintf(fp, L"%ls", L"</svg>");
    fclose(fp);
    fclose(layoutFP);
    return EXIT_SUCCESS;
}
