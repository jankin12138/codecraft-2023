#pragma once

#include <queue>

#include "Task.hpp"

class Producer {
public:
    Task creat_task();//获取目标工作台任务
    std::deque<Task> task_queue;//目标工作台任务队列
};

