#include "Consumer.hpp"


Task Consumer::get_task(Producer &p) {
    this->task_queue.push_back(p.creat_task());
    Task tmp = this->task_queue.front();
    this->task_queue.pop_front();
    return tmp;
}

//�ҵ����������Ŀ���
void Consumer::find_nearest_pos(Map &map, Robot &robot, bool flag) {
    int target_stage_id;
    double dis;
    double min_distance = INT32_MAX;
    //Ϊ���ǻ�ȡ��Ʒλ�ã�Ϊ���ǳ���λ��
    if (flag) {
        target_stage_id = robot.task.init_stage_id;
    } else {
        target_stage_id = robot.task.target_stage_id;
    }
    for (auto arr: map.stage_arr[target_stage_id]) {
        //�ж��Ƿ�����Ʒ
        if (flag && arr.product_status != 1) {
            continue;
        }
        //�ж�Ŀ�깤��̨ԭ�ϸ��Ƿ�ռ��
        if (!flag && !robot.material_exist(arr)) {
            continue;
        }
        //�����������������ľ���
        dis = distance(robot.pos_x, robot.pos_y, arr.pos_x, arr.pos_y);
        if (dis < min_distance) {
            min_distance = dis;
            robot.task.target_x = arr.pos_x;
            robot.task.target_y = arr.pos_y;
        }
    }
}