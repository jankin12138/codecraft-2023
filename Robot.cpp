#include "Robot.hpp"
#include "util.hpp"
#include "Stage.hpp"
#include <cmath>

using namespace std;

void Robot::print_forward(double v) {
    cout << "forward" << ' ' << id << ' ' << v << endl;
#if DEBUG
    cerr << "forward" << ' ' << id << ' ' << v << endl;
#endif
}

void Robot::print_rotate(double v) {
    cout << "rotate" << ' ' << id << ' ' << v << endl;
    next_rotate = v;
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

void Robot::buy(Stage &stage, Producer &p) {
    assert(distance(pos_x, pos_y, stage.pos_x, stage.pos_y) < operate_distance); // 距离足够近
    assert(object_id == no_object); // 购买前机器人需没有其他物品
    assert(stage.product_status); // 工作台产品格需已有物品
    object_id = stage.product_object_id();
    assert(object_id != no_object); // 工作台需为生产型
    stage.product_status = 0; // 工作台产品格清空
    print_buy();

    // 处理1 2 3 阻塞 不通知的问题
    if (stage.rest_time == 0) {
        stage.notify_producer(p);
    }

}

void Robot::sell(Stage &stage) {
    assert(distance(pos_x, pos_y, stage.pos_x, stage.pos_y) < operate_distance); // 距离足够近
    assert(object_id != no_object); // 购买前机器人需持有一样物品
    assert(stage.is_raw_material(object_id)); // 机器人持有物品需为工作台原料
    assert(stage.rcv_raw_material(object_id)); // 可能工作台原料格已占用无法接受
    stage.is_material_task[object_id] = 0;
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
    constexpr static double delta_v_rad_max_with_obj = 50 / (0.5 * 20 * 0.53 * 0.53 * 0.53 * 50);//0.67rad/s(38度/s)
    constexpr static double delta_v_rad_max_without_obj = 50 / (0.5 * 20 * 0.45 * 0.45 * 0.45 * 50);//1.09rad/s(62度/s)
    return object_id == no_object ? delta_v_rad_max_without_obj : delta_v_rad_max_with_obj;
}


double Robot::calc_v_rad(double dist_rad) {
    if (dist_rad == 0)
        return 0;
//    double target_v_rad;
//    // 通过判断v_rad防止死锁d
//    if (dist_rad < 0) {
//        if (v_rad < 0) {
//            target_v_rad = -(2 * pi + dist_rad) / seconds_per_frame;    // 逆时针；
//        } else {
//            target_v_rad = -dist_rad / seconds_per_frame;   // 顺时针；
//        }
//    } else {
//        if (v_rad < 0) {
//            target_v_rad = -dist_rad / seconds_per_frame;   // 逆时针；
//        } else {
//            target_v_rad = (2 * pi - dist_rad) / seconds_per_frame;    //顺时针；
//        }
//    }
//    // 弧度限制
//    if (target_v_rad < -delta_v_rad_max()) {
//        target_v_rad = -delta_v_rad_max();
//    } else if (target_v_rad > delta_v_rad_max()) {
//        target_v_rad = delta_v_rad_max();
//    }
//    return target_v_rad;
    if (fabs(-dist_rad) < fabs(dist_rad))
        dist_rad *= -1;
    return dist_rad < 0 ? max(-pi, dist_rad / seconds_per_frame) : min(pi, dist_rad / seconds_per_frame);
}

void Robot::tick(Producer &my_producer) {
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
    double target_rad, dist_rad;
    next_rotate = 0;
    switch (doing->actionType) {
        case ActionType::Goto:
            if (dist < operate_distance) {
                // 1.如果到达目标工作台附近
                print_rotate(0);
                print_forward(0);
                doing = nullptr;
                break;
            }
            target_rad = calc_rad(stage.pos_x - pos_x, stage.pos_y - pos_y);
            dist_rad = target_rad - pos_rad;
            if (dist_rad < 0)
                dist_rad += 2 * pi;
            if (dist_rad > fabs(dist_rad - 2 * pi))
                dist_rad -= 2 * pi;
            // 2.如果距离工作台存在距离
            // 2.1先对准角度(大角度对准) 大约5度误差
            if (fabs(dist_rad) > 1e-2/*允许角度偏差*/) {
                print_rotate(calc_v_rad(dist_rad));
                print_forward(4.2);
            } else {
                print_forward(v_max);
                print_rotate(0);
//                // 2.2再走直线（小角度修正5度误差）
//                double todo_v_rad = calc_v_rad(dist_rad);
//                print_rotate(todo_v_rad / 15);  //大约是62/15一帧，面向场景可以再调低点
//                print_forward(min(6.0, dist / seconds_per_frame));
            }
            break;
        case ActionType::Buy:
            buy(stage, my_producer);
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
