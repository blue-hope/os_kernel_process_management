//
//  main3.cpp
//  os2
//
//  Created by bluehope on 26/03/2019.
//  Copyright © 2019 bluehope. All rights reserved.
//
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;

long double f(long double x, int len, long double *arr) {
    //function to calculate the f(x) return value
    long double sum = 0;
    for(int i = 0; i <= len; i++){
        sum += (arr[i] * (pow(x, len-i)));
    }
    return sum;
}

long double *thread_sum;
int n;
int m;
// int running_thread;
int thread_num;
long double *coefs;

//int rng_trd, int trd_num, int n, int m, long double *coefs

void *thread_function(void *arg){
    long double adds = 0;
    
    //set variable
    long double b = 1000;
    long double a = 0;
    long double dx = (b-a)/n;

    // int n = (int)args[2];
    // int m = (int)args[3];

    // int rng_trd = (int)args[0];
    // int trd_num = (int)args[1];
    // cout<<(int)(size_t)arg<<endl;
    int running_thread = (int)(size_t)arg;
    // int running_thread = 0;
    int num_in_each_thread;

    //allocate calc range on each threads
    if(n % thread_num == 0){
        num_in_each_thread = n / thread_num;
    } else {
        num_in_each_thread = n / thread_num + 1;
    }
    
    
    if(running_thread == thread_num - 1){
        //last_ordered_process(can be different from any other process)

        for(int i = running_thread * num_in_each_thread; i < n; i++){
            //calculate riemann_sum and store it in memory address 'riemann_sum'
            //add running_process_number * sizeof(long double) to 'riemann_sum'
            //to access the proper array
            // cout << i << endl;
            adds += f(a + i * dx, m, coefs) * dx;
        }
    } else {
        for(int i = running_thread * num_in_each_thread; i < (running_thread + 1) * num_in_each_thread; i++){
            adds += f(a + i * dx, m, coefs) * dx;
        }
    }
    // cout<<"thread: "<<running_thread<<", adds: "<<adds<<endl;
    thread_sum[running_thread] = adds;
    
    pthread_exit(NULL);
};

int main(int argc, const char * argv[]) {
    //init timer variable
    struct timeval startTime, endTime;
    long total_time_ms;
    
    //process starts
    gettimeofday(&startTime, NULL);//-----------------------------------------------------------------
    
    //get arguments
    thread_num = atoi(argv[1]);
    n = atoi(argv[2]);
    
    thread_sum = new long double[thread_num];
    
    m = atoi(argv[3]);
    
    coefs = new long double[m+1];
    for(int i = 0; i <= m; i++){
        coefs[i] = stold(argv[4 + i]);
    }
   
    
    //set process threads
    pthread_t thread_handle[thread_num];
    void *thread_result;
    int res;
    
    int running_thread = 0;
   
    //multi threading
    while(running_thread < thread_num){
        //thread_create() to create new thread  , n, m, coefs}
        // int args[4] = {running_thread, thread_num, n, m}
        res = pthread_create(&(thread_handle[running_thread]), NULL, thread_function, (void *)running_thread);
        
        if(res != 0){//thread failed handling
            perror("error in pthread_create");
            return -1;
        }
        running_thread++;
    }
    
    running_thread = 0;
    
    while(running_thread < thread_num){
        res = pthread_join(thread_handle[running_thread], &thread_result);
        
        if(res != 0){
            return -1;
        }
        running_thread++;
    }
    
    //set the whole sum variable
    long double whole_sum = 0;
    
    //when finish, calculate whole_sum
    for(int i = 0; i < thread_num; i++){
        whole_sum += thread_sum[i];
    }
    
    //process ends
    gettimeofday(&endTime, NULL);//---------------------------------------------------------------------

    
    //calculate timer
    total_time_ms = ( long(endTime.tv_sec) - long(startTime.tv_sec) )*1000.0 + (( long(endTime.tv_usec) - long(startTime.tv_usec) ) / 1000.0);

    //free dynamic allocated variable and shared_memory
    delete[] thread_sum;
    delete[] coefs;

    cout << fixed;
    cout.precision(4);

    //output
    cout<<"Riemann Sum: "<<whole_sum<<"\nTotal Time: "<<total_time_ms<<endl;
    return 0;
}
