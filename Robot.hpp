#pragma once

#include <iostream>
#include <vector>

#include "Task.hpp"
#include "util.hpp"

class Stage;

class Robot {
public:
    int stage_id;//所处工作台, -1 表示当前没有处于任何工作台附近, [0,工作台总数-1] 表示某工作台的下标。当前机器人的所有购买、出售行为均针对该工作台进行。
    int object_id;//物品id, 0 表示未携带物品, 1-7 表示对应物品
    double time_value_coef;//时间价值系数
    double crash_value_coef;//碰撞价值系数
    double v_rad;//角速度, 逆时针为正, 单位弧度每秒, [-pi, pi]
    double v_x;//线速度
    double v_y;//线速度
    double pos_rad;//朝向
    double pos_x;//x坐标
    double pos_y;//y坐标
    bool is_busy;//空闲状态
    int id; // 机器人id[0, 3], 目前一共只有4个机器人
    Task task;//当前执行的任务

    Robot() : id(-1), pos_x(-1), pos_y(-1), stage_id(no_stage), object_id(no_object) {}

    enum class ActionType {
        Goto,
        Buy,
        Sell
    };

    class Action {
    public:
        ActionType actionType;
        Stage *stage;

        Action(ActionType actionType, Stage *stage) : actionType(actionType), stage(stage) {}
    };

    std::vector<Action> todo;
    Action *doing;

    void print_forward(double v);

    void print_rotate(double v);

    void print_buy();

    void print_sell();

    void print_destroy();

    void buy(Stage &stage);

    void sell(Stage &stage);

    void destroy();

    void rcv_task(Task const &task);

    /// 每帧调用，生成机器人行为对应输出
    void tick();
};

std::istream &operator>>(std::istream &in, Robot &robot);
