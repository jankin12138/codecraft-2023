#include <iostream>
#include "bits/stdc++.h"

using namespace std;

//物品
class Object {
    int buy_price;//购买价格
    int sell_price;//售出价格
};

//机器人
class Robot {
public:
    int stage_id;//工作台
    int object_id;//物品id
    float time_value_coef;//时间价值系数
    float crash_value_coef;//碰撞价值系数
    float v_rad;//角速度
    float v_x;//线速度
    float v_y;//线速度
    float pos_rad;//朝向
    float pos_x;//x坐标
    float pos_y;//y坐标
    bool is_busy;//空闲状态
};

//工作台
class Stage {
public:
    int stage_id;//工作台类型
    float pos_x;//x坐标
    float pos_y;//y坐标
    int rest_time;//剩余生产时间
    int material_status[8];//原材料格状态
    int product_status;//产品格状态
};

//任务
class Task {

};

//生产者
class Producer {
public:
    Task get_task();//获取目标工作台任务
    queue<Task> task_queue;//目标工作台任务队列
};

//消费者
class Consumer {
public:
    void distribution_task();//任务分配
    queue<Robot> robot_queue;//空闲机器人
};

//地图
class Map {
    int frame;//当前帧数
    vector<Stage> stage_arr[9];//工作台序列
    Robot robot_arr[4];//机器人序列
};


int main() {
    return 0;
}
