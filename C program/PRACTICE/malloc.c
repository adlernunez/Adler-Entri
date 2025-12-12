#include <stdio.h>
#include <stdlib.h>
int main(){
int a=10;
int *ptr= &a;
int *p;
int n;

printf("enter the numbers of integer to be stored:");
scanf("%d" ,&n);

p=(int*)malloc(n * sizeof(int));

if (p==NULL){
    printf("mem allocation failed\n");
    return 1;
} 
for (int i=0; i<n;i++)
{
    printf("enter %dth integer:\n", i+1);
    scanf ("%d",p+i);
}
printf("the integers stored are:\n");
for (int i=0; i<n;i++)
{
    printf("%d\n", *(p+i));

}
free(p);
}

