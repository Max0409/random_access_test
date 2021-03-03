#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <random>
#include <sys/time.h>

using namespace std;
int* memory;
void generate_rand_address(int scope,int access_num,int** rand_address)
{

    uniform_int_distribution<unsigned> u(0, scope);
    default_random_engine e;
    for(int i=0;i<access_num;i++){
        rand_address[i]=memory+e()%scope;
    }
}
void random_access(int thread_id,int access_num,int** rand_address)
{
    for(int i=thread_id*access_num;i<(thread_id+1)*access_num;i++){
        int num=*rand_address[i];
    }
}

void random_access_CPU(int thread_num,int block_size,int access_num){
    int** rand_address;
    rand_address=(int **)malloc(access_num*sizeof(int *));
    memory=(int *)malloc(block_size);
    int scope=block_size>>2;
    generate_rand_address(scope,access_num,rand_address);
    struct timeval start_time, end_time;
    gettimeofday(&start_time, 0);

    for(int i=0;i<thread_num;i++){
        thread t(random_access,i,access_num/thread_num,rand_address);
        t.join();
    }

    gettimeofday(&end_time, 0);
    double time_diff = (end_time.tv_sec - start_time.tv_sec) +
                       (end_time.tv_usec - start_time.tv_usec)/1000000.0;
    printf("The time CPU execution %d random access with %d thread in %d B memory  is: %.8f ms\n",
            access_num,thread_num,block_size,time_diff
            *1000);
}



int main() {
    random_access_CPU(1,1024*1024*1024,1000000);
    return 0;
}

