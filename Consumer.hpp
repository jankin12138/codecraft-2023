#pragma once

#include <queue>

#include "Robot.hpp"

class Consumer {
public:
    //void distribution_task(); // 任务分配
    std::queue<Robot> robot_queue; // 空闲机器人
};
