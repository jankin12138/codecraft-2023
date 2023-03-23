#pragma once

#include <vector>
#include <cstdio>
#include <fstream>

#include "Robot.hpp"

class Stage;

class Map {
public:
    int frame;//当前帧数
    int money;//金钱数
    int stage_num;      //工作台数量
    int robot_num = 4;  //机器人数量
    std::vector<Stage> stage_arr[9];//工作台序列 1-9
    Robot robot_arr[4];//机器人序列
    void init_map(FILE *file);       // 初始化地图
    void flush_map(FILE *file,int frameID);//刷新地图
};
