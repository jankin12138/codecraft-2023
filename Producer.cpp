#include "Producer.hpp"
#include "Task.hpp"

namespace {
//常量区
    const int ans_task[6][2] = {{3, 6},
                                {2, 6},
                                {3, 5},
                                {1, 5},
                                {2, 4},
                                {1, 4}};//固定任务分配顺序
}

//生产者创建任务队列
Task Producer::creat_task() {
    Task res = task_queue.front();
    task_queue.pop_front();
    return res;
};

