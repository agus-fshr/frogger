#include<stdio.h>


int main()
{

FILE*highs;
char num [100];

highs= fopen("high score.txt","r");
if(highs!= NULL)
{
    fgets(num,100,highs);
    puts(num);
}

printf("%c\n",num[2]);
fclose(highs);
return 0;

}