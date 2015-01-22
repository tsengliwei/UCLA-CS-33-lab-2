//
//  main.c
//  lab2
//
//  Created by Li-Wei Tseng on 11/5/14.
//  Copyright (c) 2014 Li-Wei Tseng. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

char *min_ptr = NULL , *max_ptr ;
char* storageArray;
int arraySize;

void spray_paint( char *x, int sz, char tok, char *t )
{
    printf( "%p %3d %x %s\n", x,sz,(unsigned char) tok,t ) ;
    
    if (min_ptr == NULL || min_ptr > x) {
        min_ptr = x;
    }
    int i;
    for (i = 0; i < sz; i++) {
        // place sz bytes with tok starting with the address of x
        x[i] = tok;
    }
    
    if ((max_ptr == NULL) || max_ptr < x + sz -1) {
        max_ptr = x + sz -1;
    }
//    printf("min_ptr: %p\n", min_ptr);
//    printf("max_ptr: %p\n", max_ptr);

}

void dumper( char *x, int n )
{
    
    unsigned long a = (unsigned long)x % 16; // make sure the address ends with 0
    x -= a;
    /////test code to show little endian//////
//    for (int i = 0; i < 4; i++) {
//        printf("%02x", (unsigned char)*(x+i)); // print
//    }
//    x -= (int)x % 16;
//    
//    printf("\n");
    /////////test code END///////////////
    int i, j, k;
    for (i = 0; i < n; i++) {   // loop for n
        printf("%p", x); // print address
        printf(" ");
        for (j = 0; j < 4; j++) { // loop for 16 bytes
            for (k = 3; k >= 0; k--) {   // invert to big endian
                printf("%.2x", (unsigned char)*(x+k)); // print 4 bytes
            }
            x += 4;
            printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}

void sub2()
{
    int i ;
    char* a ;
    char x[20] ;
    
    spray_paint( (char *) &i, sizeof(i), 0xf1, "sub2.i" ) ;
    spray_paint( (char *) &a, sizeof(a), 0xf2, "sub2.a" ) ;
    spray_paint( (char *) &x, sizeof(x), 0xf3, "sub2.x" ) ;
    //printf ( "Min= %p Max= %p\n", min_ptr, max_ptr ) ;
    dumper( min_ptr,(int) (max_ptr-min_ptr)/16+1 ) ;
    
    
    arraySize = (int)(max_ptr - min_ptr + 1);
    storageArray = (char *)malloc(arraySize);
    for (i = 0; i < arraySize; i++) {
        // store stack into a dynamic array
        storageArray[i] = min_ptr[i];
        // destroy the stack/ replace the stack with tokens
       // min_ptr[i] = 0xff;
    }
    //
    //   save/destroy the stack here (don't forget to use an external save area)
    //
    printf( "destroyed stack\n" ) ;
    spray_paint(min_ptr, arraySize, 0xff, "destroy.y");
    
    dumper( min_ptr,(int) (max_ptr-min_ptr)/16+1 ) ;
    
    
    for (int i = 0; i < arraySize; i++) {
        min_ptr[i] = storageArray[i];
    }
    
    //
    //    restore the stack here
    //
    printf("array size: %d\n", arraySize);
    printf( "restored stack\n" ) ;

    for (i = 0; i < arraySize; i++) {
        min_ptr[i] = storageArray[i];
    }

    dumper( min_ptr,(int) (max_ptr-min_ptr)/16+1 ) ;
}

void sub1()
{
    float i ;
    short a ;
    int x[20] ;
    
    spray_paint( (char *) &i, sizeof(i), 0xe1, "sub1.i" ) ;
    spray_paint( (char *) &a, sizeof(a), 0xe2, "sub1.a" ) ;
    spray_paint( (char *) &x, sizeof(x), 0xe3, "sub1.x" ) ;
    sub2() ;
}
int main()
{
    struct mine {
        char* a ;
        char x[20] ;
        float z ;
    } i;
    
    union crazy {
        float a ;
        char b ;
        int s ;
    } a ;
    
    char x[50] ;
    
    spray_paint( (char *) &i, sizeof(i), 0xd1, "main.i" ) ;
    spray_paint( (char *) &a, sizeof(a), 0xd2, "main.a" ) ;
    spray_paint( (char *) &x, sizeof(x), 0xd3, "main.x" ) ;
    
    sub1() ;
    
//
//    dumper((char*)&i, n);
//    spray_paint( (char *) &a, sizeof(a), 0xd2, "main.a" ) ;
//    dumper((char*)&a, n);
    return 0 ;
}
