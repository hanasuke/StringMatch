#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

int KmpTable[1024] = { 0 };    // KMP法のテーブル
int RkTable[1024] = { 0 };     // RK法のテーブル

int brute_force_search(char *text, char *pattern);
int kmp_search(char *text, char *pattern);
void kmp_init(char *pattern);
int bm_search(char *text, char *pattern);
int rabin_karp_search(char *text, char *pattern);
unsigned long make_hash(char *string);

int main(void) {
  char text[2048];      // 被探索文字列
  char pattern[128];   // 探索文字列
  int pos;             // 発見位置

  //-- 入力処理
  scanf("%s", pattern);
  printf("%s\n", pattern);
  scanf("%s", text);
  printf("%s\n", text);

  //pos = brute_force_search(text, pattern);
  pos = kmp_search(text, pattern);
  if ( pos < 0 ) {
    puts("not found");
  } else {
    printf("found at %d\n", pos);
  }

  printf("pattern: %s\n", pattern);
  printf("text   : %-20s\n", text);

  return 0;
}

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

int bm_search(char *text, char *pattern) {
  return 0;
}

int rabin_karp_search(char *text, char *pattern) {
  return 0;
}

unsigned long make_hash(char *string) {

  return 0;
}
