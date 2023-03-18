#pragma once

#include <deque>

#include "Robot.hpp"
#include "Map.hpp"

class Consumer {
    Task get_task(Producer& p);
    std::deque<Task> task_queue;
    void find_nearest_pos(Map &map, Robot &robot, bool flag);
};