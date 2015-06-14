#include <stdafx.h>
#include <stdio.h>

int MyStrLen(const char* str)
{
  const char* eos = str;

  while(*eos++) ;

  return ((int)(eos - str - 1));
}

int main()
{
  MyStrLen("changxiaokang");

  return 0;
}