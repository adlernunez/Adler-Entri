/*Basic calculator by Adler Nunez
  In continuous development 
  version 1.0 - 9/10/2025*/

#include <stdio.h>
#include <string.h>

float add(float x, float y);
float sub(float x, float y);
float mul(float x, float y);
float div(float x, float y);
void out(float z, char state[20],float x, float y);

int main(){
    /*********************************/
    char operation[20];
    int c;
    float a,b, output, result;

    /*********************************Calc draw*/
  


    /*********************************Input integers*/

    printf("enter the first integer:\n");
    scanf("%f",&a);
    printf("enter the second integer:\n");
    scanf("%f",&b);

    /********************************option selection*/
    printf("Select the subsequent number for the options below:\n");
    printf("1. Add\n");
    printf("2. Subtract\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");
    scanf("%d",&c);

    /******************************/
    switch(c)
    
    {
    case 1:
    
        output=add(a,b);
        strcpy(operation, "sum");
        break;
    
    case 2:
        output=sub(a,b);
        strcpy(operation, "difference");
        break;

    case 3:
        output=mul(a,b);
        strcpy(operation, "multiplication");
        break;

    case 4:
        output=div(a,b);
        strcpy(operation, "division");
        break;
        

    }

    /*******************************Pushing answers to display*/
    out(output,operation,a,b);

}

/***************************addition*/
float add(float x, float y)
{
    float output;
    return output = x + y;
}  

/****************************subtraction*/
float sub(float x, float y)
{
    float output;
    return output = x - y; 
}

/****************************multiplication*/
float mul(float x, float y)
{
    float output;
    return output = x * y; 
}

/****************************Division*/
float div(float x, float y)
{
    float output;

    if (y==0)
    {
        printf("Error!, cannot divide by zero");
        return 1;
    }
    else
    {
    return output = x / y; 
    }
}
/****************************Result*/
void out(float z, char state[20],float x, float y)
{
    printf("The %s of two numbers %.2f and %.2f is %.2f",state,x,y,z);
    
}

/*****************************/

