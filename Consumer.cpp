#include "Consumer.hpp"

Task Consumer::get_task(Producer &p,Map &map, Robot &robot) {
    Task tmp = p.creat_task();
    tmp.to_stage = find_nearest_pos(map,tmp.from_stage);
    this->task_queue.push_back(tmp);
    Task res = this->task_queue.front();
    this->task_queue.pop_front();
    return res;
}

bool Consumer::material_exist(Stage &stage) {
    int s_id = stage.stage_id;
    int material_tmp = stage.material_status;
    return ((material_tmp >> s_id) & 1) == 1;
}

//找到最近的出售目标点
Stage* Consumer::find_nearest_pos(Map &map,Stage* from_stage) {
    int target_stage_id;
    Stage* res;
    double dis;
    double min_distance = INT32_MAX;
    //为真是获取物品位置，为假是出售位置
    if (from_stage->count % 2) {
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
    for (auto arr: map.stage_arr[target_stage_id]) {
        //判断目标工作台原料格是否被占用
        if (material_exist(arr)) {
            continue;
        }
        //计算符合条件的最近的距离
        dis = distance(from_stage->pos_x, from_stage->pos_y, arr.pos_x, arr.pos_y);
        if (dis < min_distance) {
            min_distance = dis;
            res = &arr;
        }
    }
    return res;
}