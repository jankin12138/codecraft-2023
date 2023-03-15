#pragma once

class Task {
public:
    Task() {};

    Task(int a, int b);

    int init_stage_id;  // 获取物品工作台类型
    int target_stage_id;// 出售物品工作台类型
    double target_x;    // 目标工作台x坐标
    double target_y;    // 目标工作台y坐标
};
