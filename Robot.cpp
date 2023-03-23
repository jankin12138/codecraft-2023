#include "Robot.hpp"
#include "util.hpp"
#include "Stage.hpp"
#include <cmath>

using namespace std;

namespace {
    constexpr static double v_max = 6;
    constexpr static double v_min = -2;
    constexpr static double v_rad_max = 3.14159;
    constexpr static double operate_distance = 0.4;// 机器人操作工作台的最远距离
}

void Robot::print_forward(double v) {
    cout << "forward" << ' ' << id << ' ' << v << endl;
#if DEBUG
    cerr << "forward" << ' ' << id << ' ' << v << endl;
#endif
}

void Robot::print_rotate(double v) {
    cout << "rotate" << ' ' << id << ' ' << v << endl;
#if DEBUG
    cerr << "rotate" << ' ' << id << ' ' << v << endl;
#endif
}

void Robot::print_buy() {
    cout << "buy" << ' ' << id << endl;
#if DEBUG
    cerr << "buy" << ' ' << id << endl;
#endif
}

void Robot::print_sell() {
    cout << "sell" << ' ' << id << endl;
#if DEBUG
    cerr << "sell" << ' ' << id << endl;
#endif
}

void Robot::print_destroy() {
    cout << "destroy" << ' ' << id << endl;
#if DEBUG
    cerr << "destroy" << ' ' << id << endl;
#endif
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

bool Robot::is_busy() {
    return !(doing == nullptr && todo.empty());
}

double Robot::delta_v_max() {
    constexpr static double delta_v_max_with_obj = 250 / (0.53 * 20 * 50);
    constexpr static double delta_v_max_without_obj = 250 / (0.45 * 20 * 50);
    return object_id == no_object ? delta_v_max_without_obj : delta_v_max_with_obj;
}

double Robot::delta_v_rad_max() {
    constexpr static double delta_v_rad_max_with_obj = 50 / (0.5 * 20 * 0.53 * 0.53 * 0.53);
    constexpr static double delta_v_rad_max_without_obj = 50 / (0.5 * 20 * 0.45 * 0.45 * 0.45);
    return object_id == no_object ? delta_v_rad_max_without_obj : delta_v_rad_max_with_obj;
}

double Robot::calc_v_rad(double target_rad) {
    // 简便起见，只考虑逆时针旋转
    double dist_rad = target_rad - pos_rad;
    double target_v_rad = dist_rad / seconds_per_frame;
    return target_v_rad;
}

void Robot::tick() {
    if (doing == nullptr) {
        if (todo.empty())
            return;
        doing = &todo.front();
        todo.pop();
#if DEBUG
        cerr << this->id << ": start new action" << static_cast<int>(doing->actionType) << endl;
#endif
    }
    Stage &stage = *doing->stage;
    double dist = distance(pos_x, pos_y, stage.pos_x, stage.pos_y);
    double dist_x = stage.pos_x - pos_x, dist_y = stage.pos_y - pos_y;
    double target_rad, dist_rad;
    switch (doing->actionType) {
        case ActionType::Goto:
            if(dist < operate_distance && v_x == 0 && v_y == 0) {
                if (v_rad)
                    print_rotate(0);
                else
                    doing = nullptr;
            }
            target_rad = atan((stage.pos_y - pos_y) / (stage.pos_x - pos_x));
            if(dist_x < 0)
                target_rad += pi;
            dist_rad = target_rad - pos_rad;
            if (fabs(dist_rad) > 1e-2/*允许角度偏差*/) {
                // 先停止前进再转动
                if (v_x || v_y) {
                    print_forward(0);
                    print_rotate(0);
                } else {
                    print_rotate(calc_v_rad(target_rad));
                }
            } else if (dist >= operate_distance) {
                // 先停止转动再前进
                if (v_rad) {
                    print_forward(0);
                    print_rotate(0);
                } else {
                    print_forward(min(6.0, dist / seconds_per_frame));
                }
            } else {
                // 已到达目标工作台附近
                print_forward(0);
                print_rotate(0);
                doing = nullptr;
            }
            break;
        case ActionType::Buy:
            buy(stage);
            print_forward(0);
            print_rotate(0);
            doing = nullptr;
            break;
        case ActionType::Sell:
            sell(stage);
            print_forward(0);
            print_rotate(0);
            doing = nullptr;
            break;
        default:
            assert(false);
    }
}

std::istream &operator>>(std::istream &in, Robot &robot) {
    in >> robot.stage_id >> robot.object_id >> robot.time_value_coef >> robot.crash_value_coef >> robot.v_rad
       >> robot.v_x >> robot.v_y >> robot.pos_rad >> robot.pos_x >> robot.pos_y;
    return in;
}

void Robot::rcv_task(const Task &task) {
    todo.push(Action(ActionType::Goto, task.from_stage));//定位：此处发生篡改？？？？？？？？
    todo.push(Action(ActionType::Buy, task.from_stage));
    todo.push(Action(ActionType::Goto, task.to_stage));
    todo.push(Action(ActionType::Sell, task.to_stage));
}
