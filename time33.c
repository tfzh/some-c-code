#include "stdio.h"

unsigned int time33(char *);
int main(){
    char str[3] = "c语言";
   int res;
    
    res = time33(str);
    printf("%d", res);
}

/**
* time33算法
*/
unsigned int time33(char *str){
    unsigned int hash = 5381;
    while(*str){
        hash += (hash << 5 ) + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}