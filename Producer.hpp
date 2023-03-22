#pragma once

#include <queue>

class Task;


class Producer {
public:
    Task get_task_front();      //获取目标工作台任务
    std::deque<Task> task_queue;//目标工作台任务队列
};

