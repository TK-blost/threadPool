//
//  threadpool.cpp
//  ThreadPool
//
//  Created by blost on 4/10/17.
//  Copyright © 2017 blost. All rights reserved.
//

//#include "threadpool.hpp"
#include <pthread.h>
#include <deque>
#include "ThreadJob.hpp"
#include "errorCode.h"
//#include <stdlib.h>
#include <iostream>



pthread_t *pthread;
pthread_cond_t busyThreadCond;
pthread_cond_t avaliableThreadCond;
pthread_mutex_t busyThreadMutex,avaliableThreadMutex;
int poolsize;
long keepAlivetime;
std::deque<pthread_t> busyThread;
std::deque<pthread_t> avaliableThread;
std::deque<ThreadJob> busyJob;
std::deque<ThreadJob> waitJob;

int  AddJob(void* (*callback_function)(void *arg), void *arg){
    pthread_mutex_lock(&busyThreadMutex);
    if (busyJob.size() >= poolsize){
        pthread_cond_wait(&avaliableThreadCond, &busyThreadMutex);
    }
    ThreadJob job;
    job.callback_function = callback_function;
    job.arg = arg;
    busyJob.push_back(job);
    if (busyJob.size() !=0)
        pthread_cond_signal(&busyThreadCond);
    pthread_mutex_unlock(&busyThreadMutex);
    return 0;
}
int TerminatePool(){
    for (int i =0;i<poolsize;i++){
        pthread_join(pthread[i], NULL);
        pthread_kill(pthread[i], 0);
    }
    pthread_mutex_destroy(&busyThreadMutex);          //清理资源
    pthread_cond_destroy(&avaliableThreadCond);
    pthread_cond_destroy(&busyThreadCond);
    delete pthread;
    return 0;
}
void* ThreadFunction(void*){
    ThreadJob *job = NULL;
    while(1){
        pthread_mutex_lock(&busyThreadMutex);
        while(busyJob.size() == 0){
            pthread_cond_wait(&busyThreadCond, &busyThreadMutex);
        }
        job = &busyJob.front();
        busyJob.pop_front();
        if (busyJob.size()<poolsize)
            pthread_cond_signal(&avaliableThreadCond);
        pthread_mutex_unlock(&busyThreadMutex);
        (*(job->callback_function))(job->arg);   //线程真正要做的工作，回调函数的调用
        job = NULL;
       
    }
}
int InitThreadPool(int poolsize1,long keepAliveTime1=1000){
    poolsize = poolsize1;
    keepAlivetime = keepAliveTime1;
    if (pthread_mutex_init(&busyThreadMutex,NULL)!=0 || pthread_mutex_init(&avaliableThreadMutex,NULL)!=0){
        return INITMUTEXERROR;
    }
    if (pthread_cond_init(&busyThreadCond, NULL)!=0 || pthread_cond_init(&avaliableThreadCond, NULL)!=0){
        return INITCONDERROR;
    }
    pthread = (pthread_t *)malloc(sizeof(pthread_t) * poolsize);
    for (int i =0;i<poolsize;i++){
        if (pthread_create(&pthread[i], NULL, ThreadFunction, NULL)!=0){
            return CREATETHREADERROR;
        }
        avaliableThread.push_back(pthread[i]);
    }
    return 0;
}

int GetBusyThread(){
    return busyThread.size();
}
int GetAvaliableThread(){
    return avaliableThread.size();
}
int GetPoolSize(){
    return poolsize;
}
int UpdatePoolSize(int newPoolSize){
    poolsize = newPoolSize;
    return 0;
}
int UpdateKeepAliveTime(long newKeepAliveTime){
    keepAlivetime = newKeepAliveTime;
    return 0;
}
void PrintPoolErrorMsg(int type){
    switch(type){
        case (INITMUTEXERROR):
            std::cout<<"init threadpool init mutex error \n";
        case (INITCONDERROR):
            std::cout<<"init threadpool init cond error \n";
        case (CREATETHREADERROR):
            std::cout<<"init threadpool create thread error \n";
        case (UPDATEPOOLSIZEERROR):
            std::cout<<"update pool size error \n";
        default:
            std::cout<<"unknow error \n";
    }
}

