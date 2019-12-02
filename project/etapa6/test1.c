//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409
#include <stdio.h>
int a = 50;
int c = 10;
int v[4] = {10, 20, 30, 1};

int fun(int e,int f)
{
	c = e - f;
	return c;
}

int main()
{	
	v[2] = 8;
	while(a>0)
	{
		a = fun(a, v[2]);
		printf("%d",a);
	}
	return 1;
}

