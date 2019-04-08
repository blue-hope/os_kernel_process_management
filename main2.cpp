//
//  main2.cpp
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
#include <typeinfo>
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
    int process_num = atoi(argv[1]);
    int n = atoi(argv[2]);
    
    int num_in_each_proc;
    
    //allocate calc range on each proccess
    if(n % process_num == 0){
        num_in_each_proc = n / process_num;
    } else {
        num_in_each_proc = n / process_num + 1;
    }
    
    int m = atoi(argv[3]);
    
    long double coefs[m+1];
    for(int i = 0; i <= m; i++){
        coefs[i] = stold(argv[4 + i]);
    }
    
    //set variable
    long double b = 1000;
    long double a = 0;
    long double dx = (b-a)/n;

    long double *riemann_sum;
    
    //set process pids
    pid_t pids[process_num];
    
    int running_proc = 0;
    
    //set shared_memory
    //-> to use each process's result with array, memory size is sizeof(long double) * (process number)
    //-> use empty_arr to get the array size
    int shmid;

    void *shared_mem = (void *)0;
    
    //set empty_arr to calculate memory size
    long double *empty_arr = new long double[process_num];

    //use shmget() and shmat() to get memory with error handling
    shmid = shmget((key_t)1234, 100000000, IPC_CREAT | 0666);
    if(shmid == -1){
        perror("shmget1 failed : ");
        exit(0);
    }
    
    shared_mem = shmat(shmid, (void *)0, 0);
    if (shared_mem == (void *)-1){
        perror("shmat failed : ");
        exit(0);
    }
    
    //allocate shared_memory on riemann_sum variable
    riemann_sum = (long double *)shared_mem;
    for(int i = 0; i < process_num; i++){
        *(riemann_sum + sizeof(long double) * i) = 0;
    }
    //multi proccessing
    while(running_proc < process_num){
        //fork() to create new process
        pids[running_proc] = fork();
        
        if(pids[running_proc] < 0){//fork failed handling
            return -1;
        }
        else if(pids[running_proc] == 0){//when process child is running
            //get shared memory made above
            shmid = shmget((key_t)1234, 100000000, 0);
            if (shmid == -1){
                perror("shmget failed : ");
                exit(0);
            }
            
            shared_mem = shmat(shmid, (void *)0, IPC_CREAT | 0666);
            if (shared_mem == (void *)-1){
                perror("shmat failed : ");
                exit(0);
            }
            
            //process
            if(running_proc == process_num - 1){
                //last_ordered_process(can be different from any other process)

                for(int i = running_proc * num_in_each_proc; i < n; i++){
                    //calculate riemann_sum and store it in memory address 'riemann_sum'
                    //add running_process_number * sizeof(long double) to 'riemann_sum'
                    //to access the proper array
                    *(riemann_sum + sizeof(long double) * running_proc) += f(a + i * dx, m, coefs) * dx;
                }
            }
            else {
                //general process(non last_ordered_process

                for(int i = running_proc * num_in_each_proc; i < (running_proc + 1) * num_in_each_proc; i++){
                    //calculate riemann_sum and store it in memory address 'riemann_sum'
                    //add running_process_number * sizeof(long double) to 'riemann_sum'
                    //to access the proper array
                    *(riemann_sum + sizeof(long double) * running_proc) += f(a + i * dx, m, coefs) * dx;
                }
            }
            exit(0);//exit child process when done
        }
        running_proc++;
    }
    //set the whole sum variable
    long double whole_sum = 0;
    
    int status;
    pid_t pid;
    
    //iterate wait system call until all child processes be done
    int ppn = process_num;
    while(ppn > 0){
        pid = wait(&status);
        ppn--;
    }
    
    //when wait finish, calculate whole_sum
    for(int i = 0; i < process_num; i++){
        whole_sum += *(riemann_sum + sizeof(long double) * i);
    }
    
    //process ends
    gettimeofday(&endTime, NULL);//---------------------------------------------------------------------

    //free dynamic allocated variable and shared_memory
    delete[] empty_arr;
    shmctl(shmid, IPC_RMID, NULL);
    
    //calculate timer
    total_time_ms = ( long(endTime.tv_sec) - long(startTime.tv_sec) ) * 1000.0 + (( long(endTime.tv_usec) - long(startTime.tv_usec) ) / 1000.0);

    cout << fixed;
    cout.precision(4);

    //output
    cout<<"Riemann Sum: "<<whole_sum<<"\nTotal Time: "<<total_time_ms<<endl;
    return 0;
}
