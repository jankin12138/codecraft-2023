#include <iostream>

#include "Consumer.hpp"
#include "Robot.hpp"
#include "Map.hpp"
#include "Producer.hpp"
#include "Stage.hpp"
#include "Task.hpp"

// Consumer匹配任务
Task *Consumer::get_task(Producer &p, Map &map, Robot &robot) {
    Task *tmp = p.get_task();
    assert(tmp->from_stage != nullptr);
    tmp->to_stage = find_nearest_pos(map, tmp->from_stage);
    while (tmp->to_stage == nullptr && !p.is_empty()) {
        task_queue.push_back(tmp);
        tmp = p.get_task();
        tmp->to_stage = find_nearest_pos(map, tmp->from_stage);
    }
    while (task_queue.size()) {
        task_queue.front()->from_stage->notify_producer(p);
        // 修复已完成工作的数量统计
        p.count_task_ans[task_queue.front()->from_stage->stage_id]--;
        task_queue.pop_front();

    }
    return tmp;
}


bool Consumer::material_exist(Stage &stage, Stage &from_stage) {
    int s_id = from_stage.stage_id;
    int material_tmp = stage.material_status;
    //std::cerr<<"stage_id     "<<stage.stage_id<<"material_status    "<<material_tmp<<"stage.is_material_task:"<<stage.is_material_task[from_stage.stage_id]<<"        x:"<<stage.pos_x<<stage.pos_y<<"\n";
    return (((material_tmp >> s_id) & 1) | (stage.is_material_task[from_stage.stage_id])); //& stage.product_status);
}

//找到当前工作台的任务的最近关联工作台
Stage *Consumer::find_nearest_pos(Map &map, Stage *from_stage) {
    int target_stage_id;
    Stage *res = nullptr; // 找到当前工作台的任务的最近关联工作台
    //int nums = 0;
    //while(res == nullptr ){
    double dis; // 初始化存储dis
    double min_distance = INT32_MAX;
    // 保证产生任务的平衡（例如1->{4,5}）
    if (stage_id_count[from_stage->stage_id] % 2) {
        switch (from_stage->stage_id) {
            case 1:
                target_stage_id = 4;
                break;
            case 2:
                target_stage_id = 4;
                break;
            case 3:
                target_stage_id = 5;
                break;
            case 4:
                target_stage_id = 7;
                break;
            case 5:
                target_stage_id = 7;
                break;
            case 6:
                target_stage_id = 7;
                break;
            case 7:
                target_stage_id = 8;
                break;
        }
    } else {
        switch (from_stage->stage_id) {
            case 1:
                target_stage_id = 5;
                break;
            case 2:
                target_stage_id = 6;
                break;
            case 3:
                target_stage_id = 6;
                break;
            case 4:
                target_stage_id = 7;
                break;
            case 5:
                target_stage_id = 7;
                break;
            case 6:
                target_stage_id = 7;
                break;
            case 7:
                target_stage_id = 8;
                break;
        }
    }
    for (auto &arr: map.stage_arr[target_stage_id - 1]) {
        // 当前工作台是否已经存在其他原料
        if (material_exist(arr, *from_stage)) {
            continue;
        }
        // 寻找距离from的最近to_stage
        // 增加一个逻辑：优先找已经被分配过的工作台
        dis = distance(from_stage->pos_x, from_stage->pos_y, arr.pos_x, arr.pos_y) +
              (3 - countBits(arr.material_status)) * 50;
        if (dis < min_distance) {
            min_distance = dis;
            res = &arr;
        }
    }
    if(res == nullptr && target_stage_id == 7){
        target_stage_id = 9;
        for (auto &arr: map.stage_arr[target_stage_id - 1]) {
            // 当前工作台是否已经存在其他原料
            if (material_exist(arr, *from_stage)) {
                continue;
            }
            // 寻找距离from的最近to_stage
            // 增加一个逻辑：优先找已经被分配过的工作台
            dis = distance(from_stage->pos_x, from_stage->pos_y, arr.pos_x, arr.pos_y) +
                  (3 - countBits(arr.material_status)) * 50* sqrt(2); //关闭惩罚试试
            if (dis < min_distance) {
                min_distance = dis;
                res = &arr;
            }
        }
    }
    if (res != nullptr) {
        res->is_material_task[from_stage->stage_id] = 1;
        stage_id_count[from_stage->stage_id]++; // 维护正确取得的任务
    }
    return res;
}

Task *Consumer::get_task3(Producer &p, Map &map, Robot &robot) {
    Task *tmp = p.get_task(robot.id);
    assert(tmp->from_stage != nullptr);
    tmp->to_stage = find_nearest_pos3(map, tmp->from_stage,robot.id);
    while (tmp->to_stage == nullptr && !p.is_empty()) {
        task_queue.push_back(tmp);
        tmp = p.get_task(robot.id);
        tmp->to_stage = find_nearest_pos3(map, tmp->from_stage,robot.id);
    }
    while (task_queue.size()) {
        task_queue.front()->from_stage->notify_producer(p);
        // 修复已完成工作的数量统计
        p.count_task_ans[task_queue.front()->from_stage->stage_id]--;
        task_queue.pop_front();

    }
    //std::cerr<<"robot:"<<robot.id<<
    return tmp;
}

Stage *Consumer::find_nearest_pos3(Map &map, Stage *from_stage,int robot_id) {
    std::cerr<<"map3.txt\n";
    int target_stage_id;
    Stage *res = nullptr; // 找到当前工作台的任务的最近关联工作台
    //int nums = 0;
    //while(res == nullptr ){
    double dis; // 初始化存储dis
    double min_distance = INT32_MAX;
    // 保证产生任务的平衡（例如1->{4,5}）
    switch (robot_id) {
        case 0:
            if(from_stage->stage_id == 5) target_stage_id = 9;
            else target_stage_id = 5;
            break;
        case 1:
            if(from_stage->stage_id == 5) target_stage_id = 9;
            else target_stage_id = 5;
            break;
        case 2:
            if(from_stage->stage_id == 6) target_stage_id = 9;
            else target_stage_id = 6;
            break;
        case 3:
            if(from_stage->stage_id == 6) target_stage_id = 9;
            else target_stage_id = 6;
            break;
    }
    for (auto &arr: map.stage_arr[target_stage_id - 1]) {
        // 当前工作台是否已经存在其他原料
        if (material_exist(arr, *from_stage)) {
            continue;
        }
        // 寻找距离from的最近to_stage
        // 增加一个逻辑：优先找已经被分配过的工作台
        dis = distance(from_stage->pos_x, from_stage->pos_y, arr.pos_x, arr.pos_y) +
              (3 - countBits(arr.material_status)) * 50;
        if (dis < min_distance) {
            min_distance = dis;
            res = &arr;
        }
    }
    if(res == nullptr && target_stage_id == 7){
        target_stage_id = 9;
        for (auto &arr: map.stage_arr[target_stage_id - 1]) {
            // 当前工作台是否已经存在其他原料
            if (material_exist(arr, *from_stage)) {
                continue;
            }
            // 寻找距离from的最近to_stage
            // 增加一个逻辑：优先找已经被分配过的工作台
            dis = distance(from_stage->pos_x, from_stage->pos_y, arr.pos_x, arr.pos_y) +
                  (3 - countBits(arr.material_status)) * 50;
            if (dis < min_distance) {
                min_distance = dis;
                res = &arr;
            }
        }
    }
    if (res != nullptr) {
        res->is_material_task[from_stage->stage_id] = 1;
        stage_id_count[from_stage->stage_id]++; // 维护正确取得的任务
    }
    return res;
}