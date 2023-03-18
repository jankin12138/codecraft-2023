#pragma once

#include <deque>

#include "Robot.hpp"
#include "Map.hpp"

class Consumer {
    Task get_task(Producer &p,Map &map, Robot &robot);
    std::deque<Task> task_queue;
    Stage* find_nearest_pos(Map &map,Stage* from_stage);
    bool material_exist(Stage &stage);
};