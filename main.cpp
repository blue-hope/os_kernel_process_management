//
//  main.cpp
//  os2
//
//  Created by bluehope on 26/03/2019.
//  Copyright © 2019 bluehope. All rights reserved.
//

#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <time.h>
using namespace std;

long double f(long double x, int len, long double *arr) {
    //function to calculate the f(x) return value
    long double sum = 0;
    for(int i = 0; i <= len; i++){
        sum += (arr[i] * (pow(x, len-i)));
    }
    return sum;
}

int main(int argc, const char * argv[]) {
    //init timer variable
    struct timeval startTime, endTime;
    long total_time_ms;
    
    //process starts
    gettimeofday(&startTime, NULL);//-----------------------------------------------------------------
    
    //get arguments
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    
    long double coefs[m+1];
    for(int i = 0; i <= m; i++){
        coefs[i] = stold(argv[3 + i]);
    }
    
    //set variable
    long double b = 1000;
    long double a = 0;
    long double dx = (b-a)/n;
    long double riemann_sum = 0;
    
    //calculate riemann_sum
    for(int i = 0; i < n; i++){
        riemann_sum += f(a + i * dx, m, coefs) * dx;
    }
    
    //process ends
    gettimeofday(&endTime, NULL);//---------------------------------------------------------------------
    
    //caculate timer
    total_time_ms = ( long(endTime.tv_sec) - long(startTime.tv_sec) ) * 1000 + (( long(endTime.tv_usec) - long(startTime.tv_usec) ) / 1000.0);
    
    cout << fixed;
    cout.precision(4);
    
    //output
    cout<<"Riemann Sum: "<<riemann_sum<<"\nTotal Time: "<<total_time_ms<<endl;
    return 0;
}
