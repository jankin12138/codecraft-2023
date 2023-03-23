#include "Consumer.hpp"
#include "Robot.hpp"
#include "Map.hpp"
#include "Stage.hpp"
#include "Task.hpp"
#include "Producer.hpp"

// Consumer匹配任务
Task *Consumer::get_task(Producer &p, Map &map, Robot &robot) {
    Task *tmp = p.get_task();
    tmp->to_stage = find_nearest_pos(map, tmp->from_stage);
    return tmp;
}

bool Consumer::material_exist(Stage &stage) {
    int s_id = stage.stage_id;
    int material_tmp = stage.material_status;
    return ((material_tmp >> s_id) & 1) == 1;
}

//找到当前工作台的任务的最近关联工作台
Stage *Consumer::find_nearest_pos(Map &map, Stage *from_stage) {
    int target_stage_id;
    Stage *res=NULL; // 找到当前工作台的任务的最近关联工作台
    double dis; // 初始化存储dis
    double min_distance = INT32_MAX;
    // 保证产生任务的平衡（例如1->{4,5}）
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
    for (auto &arr: map.stage_arr[target_stage_id]) {
        // 当前工作台是否已经存在其他原料
        if (material_exist(arr)) {
            continue;
        }
        //�����������������ľ���
        dis = distance(from_stage->pos_x, from_stage->pos_y, arr.pos_x, arr.pos_y);
        if (dis < min_distance) {
            min_distance = dis;
            res = &arr;
        }
    }
    return res;
}