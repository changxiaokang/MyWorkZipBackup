#include <stdafx.h>
#include <stdio.h>

int PartionSort(int nArray[], int nStart, int nEnd);
void  QuickSort(int nArray[], int nStart, int nEnd);

int main()
{
  int nTemp[5] = {0};
  int nArray[] = {2, 5, 4, 6, 0, 1, 3, 7, 2};
  int nCount = sizeof(nArray)/sizeof(int);
  
  QuickSort(nArray, 0, nCount-1);
  
  for(int i = 0; i < nCount; i++)
    printf("%d\r\n", nArray[i]);
  
  return 0;
}

int PartionSort(int nArray[], int nStart, int nEnd)
{
  int i = 0;
  int j = 0;
  int nTemp = 0;
  int nMiddle = 0;
  nMiddle = nArray[nEnd];
  
  if(nStart < nEnd)
  {
    while(j < nEnd)
    { 
      if(nArray[j] <= nMiddle)
      {
        nTemp = nArray[i];
        nArray[i] = nArray[j];
        nArray[j] = nTemp;
        i++;
      }
      j++;
    }
    nTemp = nArray[i];
    nArray[nEnd] = nTemp;
    nArray[i] = nArray[nEnd];
  }
  
  return i;
}

void QuickSort(int nArray[], int nStart, int nEnd)
{
  int nIndex = 0;
  if(nStart < nEnd)
  {
    nIndex = PartionSort(nArray, nStart, nEnd);
    QuickSort(nArray, nStart, nIndex-1);
    QuickSort(nArray, nIndex+1, nEnd);
  }
}