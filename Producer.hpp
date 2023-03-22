#pragma once

#include <queue>
#include "unordered_map"
#include "Stage.hpp"

class Task;

class Stage;


class Producer {
public:
    Task *get_task();      //获取目标工作台任务，主要调度逻辑函数
    std::unordered_map<Stage *, Task *> task_map;//目标工作台哈希map
    bool is_empty();

    int count_task_ans[10];//记录目前获取的工作数量
    int stage_id_ans[10];//记录工作台类型个数
    //std::deque<Task> task_queue;//目标工作台任务队列
};

