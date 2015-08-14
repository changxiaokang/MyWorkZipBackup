#include <stdio.h>

struct tagGoogs
{
	char szName[20];
	float fPrice;
	int nNum;
	float fMoney;
};

void Calculate(struct tagGoogs* a, int nX);
void Sort(struct tagGoogs* a, int nX);
void OutPut(struct tagGoogs* a, int nX);

int main(void)
{
	struct tagGoogs stu[4] = 
	{
		{"XXX", 0.1f, 5, 0.2f}, 
		{"XXX", 0.1f, 5, 0.2f},
		{"XXX", 0.1f, 5, 0.2f},
		{"XXX", 0.1f, 5, 0.2f}
	};

	Calculate(stu, 4);
	Sort(stu, 4);
	OutPut(stu, 4);

	return 0;
}

void Calculate(struct tagGoogs* a, int nX) {}
void Sort(struct tagGoogs* a, int nX) {}
void OutPut(struct tagGoogs* a, int nX) {}
