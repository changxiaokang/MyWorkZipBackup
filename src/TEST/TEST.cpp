#include <stdafx.h>
#include <stdio.h>

void Add()
{
  puts("Add");
}
void Del()
{
  puts("Del");
}
void Set()
{
  puts("Set");
}
void Find()
{
  puts("Find");
}

typedef void (*PFNPROC)(); // void (*g_fnAry[4])() = {}
PFNPROC g_fnAry[4] = 
{
  Add,
  Del,
  Set,
  Find
};

void InitAry(/*void (*pfnAry[])()*/PFNPROC pfnAry[], int nCount)
{
  int nId;
  puts("Make you choice 1:Add;2:Del;3:Set;4:Find");
  for(int i = 0; i < 4; i++)
  {
    scanf("%d", &nId);
    pfnAry[i] = g_fnAry[nId-1];
  }
};

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

