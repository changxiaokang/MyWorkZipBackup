#include <stdafx.h>
#include <stdio.h>

void fun1();
void fun2();
void fun3();
void fun4();

typedef void (*PFNPROC)(); // void (*g_fnAry[4])() = {}
PFNPROC g_fnAry[4] = 
{
  fun1,
  fun2,
  fun3,
  fun4
};

void InitAry(PFNPROC pfnAry[], int nCount);

int main()
{
  // void (*PFNaRY[4])() = {NULL};
  PFNPROC pfnAry[4] = {0};

  InitAry(pfnAry, sizeof(pfnAry) / sizeof(pfnAry[0]));

  for(int i = 0; i < sizeof(pfnAry) / sizeof(pfnAry[0]); i++)
  {
    pfnAry[i]();
  }

  return 0; 
}

void fun1()
{
  puts("³Ô·¹");
}
void fun2()
{
  puts("Ë¯¾õ");
}
void fun3()
{
  puts("À­Êº");
}
void fun4()
{
  puts("ÈöÄò");
}
void InitAry(/*void (*pfnAry[])()*/PFNPROC pfnAry[], int nCount)
{
  int nId;
  puts("Make you choice 1:³Ô·¹;2:Ë¯¾õ;3:À­Êº;4:ÈöÄò");
  for(int i = 0; i < 4; i++)
  {
    scanf("%d", &nId);
    pfnAry[i] = g_fnAry[nId-1];
  }
};