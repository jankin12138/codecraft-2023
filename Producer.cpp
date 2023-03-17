#include "Producer.hpp"

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
    if (task_queue.empty()) {
        //按固定顺序放入任务队列
        for (auto ans: ans_task) {
            task_queue.emplace_back(Task(ans[0], ans[1]));
        }
    }
    Task res = task_queue.front();
    task_queue.pop_front();
    return res;
};

Task Consumer::get_task(Producer &p) {
    this->task_queue.push_back(p.creat_task());
    Task tmp = this->task_queue.front();
    this->task_queue.pop_front();
    return tmp;
}