//
// Created by Administrator on 2023/3/12.
//

#ifndef CRAFTCODE2023_LIB_H
#define CRAFTCODE2023_LIB_H

#include "bits/stdc++.h"

using namespace std;


//任务
class Task {
public:
    Task() {};

    Task(int a, int b);

    int init_stage_id;//获取物品工作台类型
    int target_stage_id;//出售物品工作台类型
    float target_x;//目标工作台x坐标
    float target_y;//目标工作台y坐标
};

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
    Task task;//当前执行的任务
    bool is_busy;//空闲状态
};

//生产者
class Producer {
public:
    Task get_task();//获取目标工作台任务
    deque<Task> task_queue;//目标工作台任务队列
};

//工作台
class Stage {
public:
    int stage_id;//工作台类型
    float pos_x;//x坐标
    float pos_y;//y坐标
    int rest_time;//剩余生产时间
    int material_status;//原材料格状态,位表表示
    int product_status;//产品格状态
    void notify_producer(Producer &p);
};


//消费者
class Consumer {
public:
    void distribution_task();//任务分配
    queue<Robot> robot_queue;//空闲机器人
};

//地图
class Map {
public:
    int frame;//当前帧数
    int money;//金钱数
    int stage_num;      //工作台数量
    int robot_num = 4;  //机器人数量
    vector<Stage> stage_arr[10];//工作台序列
    Robot robot_arr[4];//机器人序列
};

void find_nearest_pos(Map &map, Robot &robot, bool flag);//找到最近的任务目标点

void parse_char(char *line, float *temp_arr);// 子函数：处理char型数组，按照空格切割并翻译为浮点数

Map init_map(FILE *file);// 从地图读取数据：从本地文件初始化Map,提交代码需要从stdin初始化

void flush_map(FILE *file, Map *map);// 从每一帧刷新数据：从本地文件初始化Map,提交代码需要从stdin初始化
#endif //CRAFTCODE2023_LIB_H
