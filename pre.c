#include<stdio.h>

int main()
{
	char name[10][10];
	int classes[10] = {0};
	int users = 0;
	int chars = 0;
	int c = 0;
	
	while (( c = getchar()) != EOF)
	{
		if(c >= '0' && c<= '9')
			if(classes[users] != 0)
				classes[users] = 10 + (c - '0');
				
			else
				classes[users] += (c - '0');
		else if (c == '\n')
		{
			users++;
			chars = 0;
		}
		else
		{
			name[users][chars] = c;
			chars++;
		}
	}
	for (int i  = 0; i < users; i++)
	{
		int j = 0;
		if(classes[i] > 5)
		{
			
			while(name[i][j] != ' ')
			{
				putchar(name[i][j]);
				j++;
			}
			printf("\n");
		}
	}
	return 0;
}
