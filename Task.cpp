#include "Task.hpp"

//任务类构造函数
Task::Task(int a, int b) {
    init_stage_id = a;
    target_stage_id = b;
}