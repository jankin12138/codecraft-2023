#include "Robot.hpp"
#include "util.hpp"

using namespace std;

namespace {
    constexpr static double v_max = 6;
    constexpr static double v_min = -2;
    constexpr static double v_rad_max = 3.14159;
    constexpr static double operate_distance = 0.4;// 机器人操作工作台的最远距离
}

void Robot::print_forward(double v) {
    cout << "forward" << ' ' << id << ' ' << v << endl;
}

void Robot::print_rotate(double v) {
    cout << "rotate" << ' ' << id << ' ' << v << endl;
}

void Robot::print_buy() {
    cout << "buy" << ' ' << id << endl;
}

void Robot::print_sell() {
    cout << "sell" << ' ' << id << endl;
}

void Robot::print_destroy() {
    cout << "destroy" << ' ' << id << endl;
}

void Robot::buy(Stage &stage) {
    assert(distance(pos_x, pos_y, stage.pos_x, stage.pos_y) < operate_distance); // 距离足够近
    assert(object_id == no_object); // 购买前机器人需没有其他物品
    assert(stage.product_status); // 工作台产品格需已有物品
    object_id = stage.product_object_id();
    assert(object_id != no_object); // 工作台需为生产型
    stage.product_status = 0; // 工作台产品格清空
    print_buy();
}

void Robot::sell(Stage &stage) {
    assert(distance(pos_x, pos_y, stage.pos_x, stage.pos_y) < operate_distance); // 距离足够近
    assert(object_id != no_object); // 购买前机器人需持有一样物品
    assert(stage.is_raw_material(object_id)); // 机器人持有物品需为工作台原料
    assert(stage.rcv_raw_material(object_id)); // 可能工作台原料格已占用无法接受
    object_id = no_object;
    print_sell();
}

void Robot::destroy() {
    assert(object_id != no_object);
    object_id = no_object;
    print_destroy();
}

void Robot::tick() {
    if (!is_busy) {
        if (todo.empty())
            return;
        doing = &todo.front();
        todo.erase(todo.begin());
    }
    Stage &stage = *doing->stage;
    double dist = distance(pos_x, pos_y, stage.pos_x, stage.pos_y);
    double target_rad, dist_rad;
    switch (doing->actionType) {
        case ActionType::Goto:
            target_rad = atan((stage.pos_y - pos_y) / (stage.pos_x - pos_x));
            dist_rad = target_rad - pos_rad;
            if (dist_rad) {
                // 转动
                if (dist_rad > 0)
                    print_rotate(min(pi, dist_rad / seconds_per_frame));
                else
                    print_rotate(max(-pi, dist_rad / seconds_per_frame));
                print_forward(0);
            } else if (dist >= operate_distance) {
                // 前进
                print_rotate(0);
                print_forward(min(6.0, dist / seconds_per_frame));
            } else {
                // 已到达目标工作台附近
                is_busy = false;
                print_forward(0);
                print_rotate(0);
            }
            break;
        case ActionType::Buy:
            buy(stage);
            break;
        case ActionType::Sell:
            sell(stage);
            break;
    }
}

std::istream &operator>>(std::istream &in, Robot &robot) {
    in >> robot.stage_id >> robot.object_id >> robot.time_value_coef >> robot.crash_value_coef >> robot.v_rad
       >> robot.v_x >> robot.v_y >> robot.pos_rad >> robot.pos_x >> robot.pos_y;
    return in;
}

void Robot::RcvTask(const Task &task) {
    todo.push_back(Action(ActionType::Goto, task.from_stage));
    todo.push_back(Action(ActionType::Buy, task.from_stage));
    todo.push_back(Action(ActionType::Goto, task.to_stage));
    todo.push_back(Action(ActionType::Sell, task.to_stage));
}
