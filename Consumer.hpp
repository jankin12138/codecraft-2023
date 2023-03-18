#pragma once

#include <deque>

#include "Robot.hpp"

class Consumer {
    Task get_task(Producer& p);
    std::deque<Task> task_queue;
};