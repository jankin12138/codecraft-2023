#pragma once

#include <queue>

class Task;


class Producer {
public:
    Task creat_task();//获取目标工作台任务
    std::deque<Task> task_queue;//目标工作台任务队列
};

