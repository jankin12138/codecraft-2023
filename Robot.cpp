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

void Robot::go_to_stage(Stage &stage) {
    assert(!is_busy); // 在空闲时才能前往下一个工作台
    target_stage = &stage;
    is_busy = true;
}

void Robot::buy() {
    assert(distance(pos_x, pos_y, target_stage->pos_x, target_stage->pos_y) < operate_distance); // 距离足够近
    assert(object_id == no_object); // 购买前机器人需没有其他物品
    assert(target_stage->product_status); // 工作台产品格需已有物品
    object_id = target_stage->product_object_id();
    assert(object_id != no_object); // 工作台需为生产型
    target_stage->product_status = 0; // 工作台产品格清空
    print_buy();
}

void Robot::sell() {
    assert(object_id != no_object); // 购买前机器人需持有一样物品
    assert(target_stage->is_raw_material(object_id)); // 机器人持有物品需为工作台原料
    assert(target_stage->rcv_raw_material(object_id)); // 可能工作台原料格已占用无法接受
    object_id = no_object;
    print_sell();
}

void Robot::destroy() {
    assert(object_id != no_object);
    object_id = no_object;
    print_destroy();
}

void Robot::tick() {
    if (!is_busy)
        return;

    double dist = distance(pos_x, pos_y, target_stage->pos_x, target_stage->pos_y);
    double target_rad = atan((target_stage->pos_y - pos_y) / (target_stage->pos_x - pos_x));
    double dist_rad = target_rad - pos_rad;
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
        return;
    }
}

std::istream &operator>>(std::istream &in, Robot &robot) {
    in >> robot.stage_id >> robot.object_id >> robot.time_value_coef >> robot.crash_value_coef >> robot.v_rad
       >> robot.v_x >> robot.v_y >> robot.pos_rad >> robot.pos_x >> robot.pos_y;
    return in;
}

bool Robot::material_exist(Stage &stage) {
    int s_id = this->task.init_stage_id;
    int material_tmp = stage.material_status;
    return ((material_tmp >> s_id) & 1) == 1;
}
