#include<stdio.h>


int main()
{

FILE*highs;

highs= fopen("high score.txt","w+");

fprintf(highs,"zo se la come");

fclose(highs);
return 0;

}