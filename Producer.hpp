#pragma once

#include <queue>
#include "unordered_map"
#include "Stage.hpp"

class Task;

class Stage;


class Producer {
public:
    Task *get_task();      //获取目标工作台任务，主要调度逻辑函数
    std::unordered_map<Stage *, Task *> task_map;//目标工作台哈希map，<工作台,任务>类似二维数据
    bool is_empty();

    int p_size();

    int count_task_ans[10] = {0};//记录目前已经stage通知的任务数量，永远增加，<1:几个任务，2：几个任务，...>
    int stage_id_ans[10] = {0};//记录当前交付消费者的各类型工作台的任务总数，<1：几个任务，2：几个任务，...>
    //std::deque<Task> task_queue;//目标工作台任务队列
};

