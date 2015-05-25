#include <stdafx.h>
#include <stdio.h>

/*
  ary表示数组第0个元素地址的指针常量，指针类型为元素类型
  type ary[M];

  ary[n] <==> *(type*)((int)ary + sizeof(type)*n)
  type *prt = xxxx;
  type *ptrTest = xxxx;
  int n = xxxx;

  (ptr + n) is (int)ptr + sizeof(type)*n;
  ptr + n <==> (type* const)((int)ptr + sizeof(type)*n);
  ptr - ptrTest <==> ((int)ptr - (int)ptrTest) / sizeof(type)

  ary[n] == *(ary + n);
  &ary[n] == ary + n;

  type *ptr = ary;
  ptr[n] == ary[n];
  ptr++; // ok
  ary++; // err, ary is pointer const
*/

void exchange(int* pa, int* pb);

int main()
{
  int ary[5] = 
  {
    1, 2, 3, 4, 5
  };

  int* pAry = ary;
  int i = 3;

  printf("&pAry[3] = %p \t &ary[3] = %p\r\n", &pAry[3], &ary[3]);
  printf("pAry + 3 = %p \t ary + 3 = %p\r\n", pAry + 3, ary + 3);

  if(*ary == *pAry)
    puts("*ary == *pAry");

  pAry++;
  printf("%d\r\n", pAry - ary);

  int a = 2; 
  int b = 8;

  int* pa = &a;
  int* pb = &b;
  exchange(pa, pb);

  printf("a = %d, b = %d\r\n", a, b);

  return 0; 
}

void exchange(int* pa, int* pb)
{
  int nTemp = *pa;
  *pa = *pb;
  *pb = nTemp;
}
