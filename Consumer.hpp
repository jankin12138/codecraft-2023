#pragma once

#include <deque>

class Task;

class Robot;

class Map;

class Producer;

class Stage;

class Consumer {
public:
    Task *get_task(Producer &p, Map &map, Robot &robot);

    Task *get_task3(Producer &p, Map &map, Robot &robot);//3号地图特化

    std::deque<Task *> task_queue;

    Stage *find_nearest_pos(Map &map, Stage *from_stage);
    Stage *find_nearest_pos3(Map &map, Stage *from_stage,int robot_id);
    int stage_id_count[10];

    bool material_exist(Stage &stage, Stage &from_stage);
};