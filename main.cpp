//
//  main.cpp
//  ThreadPool
//
//  Created by blost on 4/10/17.
//  Copyright © 2017 blost. All rights reserved.
//

#include"threadpool.h"

void* test(void* i){
    int *p = (int*) i;
    std::cout<<"this is :"<<*p<<" thread"<<std::endl;
    for (int i =0;i<1000000000;i++){
        ;
    }
    return NULL;
}
int main() {
    //insert code here...
    std::cout << "Hello, World!\n";
    InitThreadPool(10,1000);
    for (int i =0;i<60 ;i++){
        AddJob(test,&i);
    }
    std::cout<<"what \n";
    
    return 0;
}
