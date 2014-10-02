#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define CNUM 256

typedef unsigned char uchar;

int KmpTable[1024] = { 0 };    // KMP法のテーブル
int BmTable[CNUM] = { 0 };     // RK法のテーブル

//=================================================
// *_search関数は、テキストとパターンが引数
// 返却値は、出現した場所の配列添字
//-------------------------------------------------
// *_init関数は、照合テーブルを作成
// それぞれのパターンを引数
// 照合テーブルはグローバル変数として宣言
//=================================================

int brute_force_search(uchar *text, uchar *pattern);
int kmp_search(uchar *text, uchar *pattern);
void kmp_init(uchar *pattern);
int bm_search(uchar *text, uchar *pattern);
void bm_init(uchar *pattern);
int rk_search(uchar *text, uchar *pattern);
unsigned long long rk_hash(uchar *string, int len);  // もしかしたらunsigned long longじゃないときついかも
unsigned long long rk_rhash(uchar *string, int len, unsigned long long hash);

int main(void) {
  uchar text[2048];      // 被探索文字列
  uchar pattern[128];   // 探索文字列
  int pos;             // 発見位置

  //-- 入力処理
  scanf("%s", pattern);
  scanf("%s", text);

  // pos = brute_force_search(text, pattern);
  // pos = kmp_search(text, pattern);
  // pos = bm_search(text, pattern);
  pos = rk_search(text, pattern);

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
int brute_force_search(uchar *text, uchar *pattern) {
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
int kmp_search(uchar *text, uchar *pattern) {
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
void kmp_init(uchar *pattern) {
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
  // debug
  for ( i = 0; i < strlen(pattern); i++ ) {
    printf("%-2d ", KmpTable[i]);
  }
  puts("");
}

//-- BM法での探索
int bm_search(uchar *text, uchar *pattern) {
  int tpos;
  int ppos;
  int tlength;
  int plength;

  tlength = strlen(text);
  plength = strlen(pattern);

  bm_init(pattern);

  // 最初の注目位置
  tpos = tlength-1;

  while ( (tpos < tlength) && (ppos < plength )) {
    ppos = plength-1;   //  パターンの中もう位置をセット
    while (  ppos >= 0 ) {
      if ( text[tpos] != pattern[ppos] ) {
        tpos += BmTable[text[tpos]];
      } else {
        tpos--;
        ppos--;
      }
    }
    if ( ppos < 0 ) {
      return tpos+plength;
    }
  }

  return -1;
}

// BM法の作業表を作成
void bm_init(uchar *pattern) {
  int plength = strlen(pattern);
  int i;

  // 移動量の初期化(パターンの長さ)
  for ( i = 0; i < CNUM; i++ ){
    BmTable[i] = plength;
  }
  for ( i = 0; i < plength-1; i++ ) {
    BmTable[pattern[i]] = plength - i - 1;
  }

  // debug
  for ( i = 0; i < CNUM; i++ ) {
    printf("%-2d ", BmTable[i]);
  }

}

//-- ラビン-カープ法での探索
int rk_search(uchar *text, uchar *pattern) {
  int plength = strlen(pattern);
  int tlength = strlen(text);
  unsigned long long thash;
  unsigned long long phash;
  int i;

  phash = rk_hash(pattern, strlen(pattern));
  for ( i = 0; i < tlength-plength+1; i++ ){

    if ( ! i ) {
      thash = rk_hash(&text[i], plength);
    } else {
      thash = rk_rhash(&text[i], plength, thash);
    }

    if ( thash == phash ) {
      return i;
    }
  }

  return -1;
}

// ラビンカープで利用するハッシュ関数
unsigned long long rk_hash(uchar *string, int len) {
  int i;  // 反復変数
  unsigned long long hash = 0;

  for ( i = 0; i < len; i++ ) {
    hash <<= 1;
    hash += string[i];
  }

  return hash;
}

// ラビンカープで利用するローリングハッシュ関数
unsigned long long rk_rhash(uchar *string, int len, unsigned long long hash) {
  int i=0;  // 反復変数

  hash -= (string[-1] << len-1);
  printf("%c %c %c \n", string[i-1], string[i], string[len-1]);

  hash <<= 1;
  hash += string[len-1];

  printf("%d, %d\n", i, len);

  return hash;
}
