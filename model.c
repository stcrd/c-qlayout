#include <stdio.h>

// const currentOfficialLayout = {
//   'а': 'f', 'ә': '2', 'б': ',', 'в': 'd', 'г': 'u', 'ғ': '5',
//   'д': 'l', 'е': 't', 'ё': '1', 'ж': ';', 'з': 'p', 'и': 'b',
//   'й': 'q', 'к': 'r', 'қ': '0', 'л': 'k', 'м': 'v', 'н': 'y',
//   'ң': '4', 'о': 'j', 'ө': '-', 'п': 'g', 'р': 'h', 'с': 'c',
//   'т': 'n', 'у': 'e', 'ұ': '9', 'ү': '8', 'ф': 'a', 'х': '[',
//   'һ': '=', 'ц': 'w', 'ч': 'x', 'ш': 'i', 'щ': 'o', 'ъ': ']',
//   'ы': 's', 'і': '3', 'ь': 'm', 'э': "'", 'ю': '.', 'я': 'z',
// };

// char validPositions[] = "1234567890-=qwertyuiop[]asdfghjkl;'zxcvbnm";
// char allPositions[] = "`1234567890-=qwertyuiop[]|asdfghjkl;'zxcvbnm,./";
int main()
{
  // array of ints representing kazakh alphabet "аәбвгғдеёжзийкқлмнңоөпрстуұүфхһцчшщъыіьэюя"
  int qaz[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
              10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
              20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
              30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
              40, 41, 42};
  // array of ints representing positions on a US qwerty keyboard "1234567890-=qwertyuiop[]asdfghjkl;'zxcvbnm"
  int positions[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
              10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
              20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
              30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
              40, 41, 42};
  char validPositions[] = "1234567890-=qwertyuiop[]asdfghjkl;'zxcvbnm";
  // bke = baseline key effort
  // pr = row penalty
  // pf = finger penalty
  // hands: 1.0 = left; 2.0 = right
  // rows: 1.0 = number row; 2.0 = top row; 3.0 = home row; 4.0 = bottom row
  // fingers: 0.0 = left pinky; 1.0 = left ring; 2.0 = left middle; 3.0 = left first;
  // fingers: 6.0 = left first; 7.0 = left middle; 8.0 = left ring; 9.0 = left pinky;
  // order of factors in the factor array: pr, pf, bke, row, hand, finger
  // allFactors[position]
  int allFactors[][6] = {
                        {1.5, 0.5, 5.0, 1.0, 1.0, 1.0},
                        {1.5, 0.5, 4.0, 1.0, 1.0, 1.0},
                        {1.5, 0.0, 4.0, 1.0, 1.0, 2.0},
                        {1.5, 0.0, 4.0, 1.0, 1.0, 3.0},
                        {1.5, 0.0, 3.5, 1.0, 1.0, 3.0},
                        {1.5, 0.0, 4.5, 1.0, 1.0, 3.0},
                        {1.5, 0.0, 4.0, 1.0, 2.0, 6.0},
                        {1.5, 0.0, 4.0, 1.0, 2.0, 7.0},
                        {1.5, 0.0, 4.0, 1.0, 2.0, 7.0},
                        {1.5, 0.5, 4.0, 1.0, 2.0, 8.0},
                        {1.5, 1.0, 4.0, 1.0, 2.0, 9.0},
                        {1.5, 1.0, 4.5, 1.0, 2.0, 9.0},
                        {0.5, 1.0, 2.0, 2.0, 1.0, 0.0},
                        {0.5, 0.5, 2.0, 2.0, 1.0, 1.0},
                        {0.5, 0.0, 2.0, 2.0, 1.0, 2.0},
                        {0.5, 0.0, 2.0, 2.0, 1.0, 3.0},
                        {0.5, 0.0, 2.5, 2.0, 1.0, 3.0},
                        {0.5, 0.0, 3.0, 2.0, 2.0, 6.0},
                        {0.5, 0.0, 2.0, 2.0, 2.0, 6.0},
                        {0.5, 0.0, 2.0, 2.0, 2.0, 7.0},
                        {0.5, 0.5, 2.0, 2.0, 2.0, 8.0},
                        {0.5, 1.0, 2.0, 2.0, 2.0, 9.0},
                        {0.5, 1.0, 2.5, 2.0, 2.0, 9.0},
                        {0.5, 1.0, 4.0, 2.0, 2.0, 9.0},
                        {0.0, 1.0, 0.0, 3.0, 1.0, 0.0},
                        {0.0, 0.5, 0.0, 3.0, 1.0, 1.0},
                        {0.0, 0.0, 0.0, 3.0, 1.0, 2.0},
                        {0.0, 0.0, 0.0, 3.0, 1.0, 3.0},
                        {0.0, 0.0, 2.0, 3.0, 1.0, 3.0},
                        {0.0, 0.0, 2.0, 3.0, 2.0, 6.0},
                        {0.0, 0.0, 0.0, 3.0, 2.0, 6.0},
                        {0.0, 0.0, 0.0, 3.0, 2.0, 7.0},
                        {0.0, 0.5, 0.0, 3.0, 2.0, 8.0},
                        {0.0, 1.0, 0.0, 3.0, 2.0, 9.0},
                        {0.0, 1.0, 2.0, 3.0, 2.0, 9.0},
                        {1.0, 1.0, 2.0, 4.0, 1.0, 0.0},
                        {1.0, 0.5, 2.0, 4.0, 1.0, 1.0},
                        {1.0, 0.0, 2.0, 4.0, 1.0, 2.0},
                        {1.0, 0.0, 2.0, 4.0, 1.0, 3.0},
                        {1.0, 0.0, 3.5, 4.0, 1.0, 3.0},
                        {1.0, 0.0, 2.0, 4.0, 2.0, 6.0},
                        {1.0, 0.0, 2.0, 4.0, 2.0, 6.0},
                        };
  double temp = allFactors[46][5];
  printf("yes");

  // p0: initial probability
  // t: initial 'temperature'
  // k: cooling factor
  // N: total number of steps
  // optimization parameters in following order: p0, t, k, N, swappedPairs
  int optParams[] = {1, 10, 10, 10000, 1};

  // factors from model 01 (http://mkweb.bcgsc.ca/carpalx/?model_parameters)
  // efforts factors in the following order: k1, k2, k3, kb, kp, ks
  double effortFactors[] = {1.0, 0.367, 0.235, 0.3555, 0.6423, 0.4268};

  // h = hand
  // r = row
  // f = finger
  // w = weight
  // w0 = baseline penalty
  // p = penalty

  // no baseline penalty
  // const wh = 1;
  // weights from model 01 (http://mkweb.bcgsc.ca/carpalx/?model_parameters)

  // no hand penalty
  // general formula: penalty = w0 + wh * ph + wr * pr + wf * pf
  // since w0 = 0 and ph = 0, the formula can be shortened as below
  // penalty = wr * pr + wf * pf;
  // weight factors in the following order: wr, wf
  double weightFactors[] = {1.3088, 2.5948};
}