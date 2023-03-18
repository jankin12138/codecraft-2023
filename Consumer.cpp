#include "Consumer.hpp"


Task Consumer::get_task(Producer &p) {
    this->task_queue.push_back(p.creat_task());
    Task tmp = this->task_queue.front();
    this->task_queue.pop_front();
    return tmp;
}