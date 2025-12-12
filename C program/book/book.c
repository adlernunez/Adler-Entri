#include <stdio.h>
#include <string.h>

typedef struct{

    char name[50];
    char author[25];
    float price;

}book;

int main()
{
    book b;
    book maxbook;
    book minbook;
    float maxprice = -1;
    float minprice;

    for(int i = 1;i<=3;i++)
    {

    printf("enter the title of book:");
    fgets(b.name,50,stdin);
    size_t len = strlen(b.name);
    if(len > 0 && b.name[len-1] == '\n') b.name[len-1] = '\0';

    printf("enter the name of the author:");
    fgets(b.author,50,stdin);
    len = strlen(b.author);
    if(len > 0 && b.author[len-1] == '\n') b.author[len-1] = '\0';

    printf("enter the price:");
    scanf("%f",&b.price);
    getchar();   

    if (i==1){

        maxbook=minbook=b;
        maxprice=b.price;
        minprice=b.price;

    }
        else{
            if (b.price>maxprice)
            {
            maxprice = b.price;
            maxbook =b;
            }

            if (b.price<minprice)
            {
            minprice = b.price;
            minbook =b;
            }
        }       

    }

    printf("The most expensive book : %s\n", maxbook.name);
    printf("Author : %s\n", maxbook.author);
    printf("Price : %.2f\n", maxbook.price);

    printf("The least expensive book : %s\n", minbook.name);
    printf("Author : %s\n", minbook.author);
    printf("Price : %.2f\n", minbook.price);
}