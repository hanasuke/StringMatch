#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define UCHAR 256

int KmpTable[1024] = { 0 };    // KMP法のテーブル
int BmTable[UCHAR] = { 0 };     // RK法のテーブル

//=================================================
// *_search関数は、テキストとパターンが引数
// 返却値は、出現した場所の配列添字
//-------------------------------------------------
// *_init関数は、照合テーブルを作成
// それぞれのパターンを引数
// 照合テーブルはグローバル変数として宣言
//=================================================

int brute_force_search(char *text, char *pattern);
int kmp_search(char *text, char *pattern);
void kmp_init(char *pattern);
int bm_search(char *text, char *pattern);
void bm_init(char *pattern);
int rabin_karp_search(char *text, char *pattern);
unsigned long make_hash(char *string);  // もしかしたらunsigned long longじゃないときついかも

int main(void) {
  char text[2048];      // 被探索文字列
  char pattern[128];   // 探索文字列
  int pos;             // 発見位置

  //-- 入力処理
  scanf("%s", pattern);
  printf("%s\n", pattern);
  scanf("%s", text);
  printf("%s\n", text);

  pos = brute_force_search(text, pattern);
  //  pos = kmp_search(text, pattern);

  kmp_init(pattern); // debug
  bm_init(pattern);  // debug

  if ( pos < 0 ) {
    puts("not found");
  } else {
    printf("found at %d\n", pos);
  }

  printf("pattern: %s\n", pattern);
  printf("text   : %-20s\n", text);

  return 0;
}

//-- 素朴法での探索
int brute_force_search(char *text, char *pattern) {
  int plength;  // パターンの文字列
  int tlength;  // テキストの文字列
  int pos;      // 探索の現在地
  int k1, k2;

  plength = strlen(pattern);
  tlength = strlen(text);

  printf("%d %d\n", plength, tlength);

  for ( k1 = 0; k1 < tlength; k1++ ) {
    pos = k1;
    for ( k2 = 0; k2 < plength; k2++ ) {
      if ( text[pos] != pattern[k2] ) {
        break;
      } else {
        pos++;
      }
    }
    if ( k2 == plength ) {  // 文字列が発見された場合
      return k1;
    }
  }

  return -1;
}

//-- KMP法での探索
int kmp_search(char *text, char *pattern) {
  int k1, k2;
  int plength;  // パターンの文字列
  int tlength;  // テキストの文字列
  int pos;      // 探索の現在地

  k1 = 0;
  k2 = 0;

  kmp_init(pattern);  // 作業表作成

  plength = strlen(pattern);
  tlength = strlen(text);

  while ( k1 < tlength && k2 < plength ) {
    if ( text[k1] == pattern[k2] ) {
      if ( ! pattern[k2+1] ) {
        return k1-k2;
      }
      k1++;
      k2++;
    } else if ( k2 > 0 ) {
      k2 = k2 - KmpTable[k2];
    } else {
      k1++;
    }
  }
  return -1;
}

// KMP法の作業表を作成
void kmp_init(char *pattern) {
  int i = 2;
  int j = 0;
  KmpTable[0] = -1;
  KmpTable[1] = 0;

  while ( i == 0 || pattern[i-1] ) {
    if ( pattern[i-1] == pattern[j] ) {
      KmpTable[i++] = ++j;
    } else if ( j > 0 ) {
      j = KmpTable[j];
    } else {
      KmpTable[i++] = 0;
    }
  }
  for ( i = 0; i < strlen(pattern); i++ ) {
    printf("%-2d ", KmpTable[i]);
  }
  puts("");
}

//-- BM法での探索
int bm_search(char *text, char *pattern) {

  return 0;
}

// BM法の作業表を作成
void bm_init(char *pattern) {
  int plength = strlen(pattern);
  int i;

  // 移動量の初期化(パターンの長さ)
  for ( i = 0; i < plength; i++ ){
    BmTable[i] = plength;
  }
  for ( i = 0; i < UCHAR; i++ ) {
    BmTable[(int)pattern[i]] = plength - i - 1;
  }

  // debug
  for ( i = 0; i < UCHAR; i++ ) {
    printf("%-2d ", BmTable[i]);
  }

}

//-- ラビン-カープ法での探索
int rabin_karp_search(char *text, char *pattern) {
  return 0;
}

// ラビンカープで利用するハッシュ関数
unsigned long make_hash(char *string) {

  return 0;
}
