#pragma once

#include <iostream>
#include <queue>

#include "Task.hpp"
#include "util.hpp"

class Stage;

class Producer;

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
    int id; // 机器人id[0, 3], 目前一共只有4个机器人
    Task task;//当前执行的任务
    double next_rotate; // 下一帧的角度
    bool is_busy();//空闲状态

    Robot() : stage_id(no_stage), object_id(no_object), time_value_coef(1), crash_value_coef(1),
              v_rad(0), v_x(0), v_y(0), pos_rad(0), pos_x(-1), pos_y(-1), id(-1) {}

    enum class ActionType {
//        Goto,
//        Buy,
//        Sell
        GotoBuy,
        GotoSell,
    };
    class Action {
    public:
        ActionType actionType;
        Stage *stage;

        Action(ActionType actionType, Stage *stage) : actionType(actionType), stage(stage) {}
    };

    std::queue<Action> todo;
    std::queue<Action> Temp_todo;
    Action *doing = NULL;

    void print_forward(double v);

    void print_rotate(double v);

    void print_buy();

    void print_sell();

    void print_destroy();

    void buy(Stage &stage, Producer &p);

    void sell(Stage &stage);

    void destroy();

    void rcv_task(Task const &task);

    void rcv_jump_task(const Task &task);
    
    /// 每帧调用，生成机器人行为对应输出
    void tick(Producer &p);

private:
    double delta_v_max();

    double delta_v_rad_max();

    double calc_v_rad(double target_rad);
};

std::istream &operator>>(std::istream &in, Robot &robot);
