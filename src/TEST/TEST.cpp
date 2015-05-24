#include <stdafx.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

void EncryptFun();

int main()
{
  DWORD dwOld;
  VirtualProtect(EncryptFun, 0x1, PAGE_EXECUTE_READWRITE, &dwOld);
  char* PFNPROC = (char*)EncryptFun;

#ifdef _DEBUG
  PFNPROC = (char*)((int)EncryptFun + *(int*)(PFNPROC + 1) + 5);
#endif
  // *PFNPROC = '\0xCC';
  EncryptFun();

  return 0; 
}

void EncryptFun()
{
  int nRandom = 1;
  srand((unsigned)time(NULL));
  for(int i = 0; i < 10; i++)
  {
    nRandom = rand() % 11 + 10;
    printf("%d\r\n", nRandom);
  }
}
