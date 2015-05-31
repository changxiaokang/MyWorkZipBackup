#include <stdafx.h>
#include <stdio.h>

#define A 97
#define Z 121

int main()
{
  for(int i = A; i < Z; i++)
  {
    for(int j = A; j < Z; j++)
    {
      for(int k = A; k < Z; k++)
      {
        if(i == 'm')
        {
          printf("i%c%c%c\t", i, j, k);
        }
      }
    }
  }

  return 0;
}