#include <iostream>
#include <iomanip>
#include <algorithm>
#include "windows.h"
#pragma warning(disable:4996)
unsigned int m,n;
struct seg_addr
{
	unsigned int addr;
	char name[32];
};
int f1(int a)
{
	a += a;
	int f1_b, f1_c;
	char f1_d;
	printf("<--------- In f1(temp var) ---------->\n");
	printf("%08x %08x %08x\n",&f1_b,&f1_c,&f1_d);
	return a;
}
unsigned int f2()
{
	char a;
	printf("Hello, world.\n");
	printf("<--------- In f2(temp var) --------->\n");
	printf("%08x\n", &a);
	return (unsigned int)&a;
}
void f3()
{
	printf("<--------- In f3 --------->\n");
	f2();
	f1(1);
}
bool cmp(struct seg_addr s1, struct seg_addr s2)
{
	return (s1.addr < s2.addr);
}
void mysort(struct seg_addr* segaddr)
{
	std::sort(segaddr, segaddr + 4, cmp);
	std::cout << "<--------- 各个段按顺序如下：--------->" << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);
	for (int i = 0; i < 4; i++)
		std::cout <<"->"<< segaddr[i].name;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
}
using namespace std;

int main()
{
begin_main:
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
	static int main_a, main_b;
	_asm
	{
		mov m, offset begin_main
		mov n, offset f3
	}
	puts("<--------- Function address: --------->");
	cout << "f1:" << f1 << endl << "f2:" << f2 << endl << "f3:" << f3 << endl;
	struct seg_addr seg_addr[4];
	unsigned int maina = (unsigned int)f3 - n + m;
	seg_addr[0].addr = (unsigned int)f3 - n + m; strcpy(seg_addr[0].name, "function main");
	seg_addr[1].addr = (unsigned int)f3; strcpy(seg_addr[1].name, "other functions");
	seg_addr[2].addr = f2(); strcpy(seg_addr[2].name, "stack_heap seg");
	seg_addr[3].addr = (unsigned int)&main_a; strcpy(seg_addr[3].name, "data seg");
	puts("<--------- In main: --------->");
	cout << "main:" << setfill('0')<<setw(8) << hex <<maina << endl;
	cout << "In main(static var):" << endl << setfill('0') << setw(8) << hex << &main_a << " " << &main_b << endl;;
	f3();
	mysort(seg_addr);
}
