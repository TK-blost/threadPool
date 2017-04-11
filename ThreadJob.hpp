//
//  ThreadJob.hpp
//  ThreadPool
//
//  Created by blost on 4/10/17.
//  Copyright © 2017 blost. All rights reserved.
//

#ifndef ThreadJob_hpp
#define ThreadJob_hpp
#endif /* ThreadJob_hpp */

class ThreadJob{
    
public:
    void* (*callback_function)(void *arg);
    void *arg;
    //static void* ThreadFunction(void*);
    //void virtual run();
};
