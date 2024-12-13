#ifndef _THREADPOOL_
#define _THREADPOOL_

#include <iostream>
#include <pthread.h>
#include <cstring>
#include "TaskQueue.h"

class ThreadPool
{
public:
    ThreadPool(int min,int max);//创建线程池并初始化
    ~ThreadPool();//销毁线程池

    void addTask(Task task);//给线程池添加任务

    int getBusyNum();//获取线程池中工作的线程的个数

    int getAliveNum();//获取线程池中活着的线程的个数

    static void* worker(void* arg);//工作的线程(消费者线程)任务函数

    static void* manager(void* arg);//管理者线程任务函数

    void threadExit();//单个线程退出
private:
    TaskQueue* taskQ;

    pthread_t managerID;//管理者线程ID
    pthread_t* threadIDs;//工作的线程ID
    int minNum;//最小线程数量
    int maxNum;//最大线程数量
    int busyNum;//忙的线程的个数
    int liveNum;//存活的线程的个数
    int exitNum;//要销毁的线程个数
    pthread_mutex_t mutexPool;//锁整个的线程池
    pthread_cond_t notEmpty;//任务队列是不是空了
    bool shutdown;  //是不是要销毁线程池，销毁为1，不销毁为0

    static const int NUMBER = 10;
};





#endif