#pragma once

#include <cmath>
#include <string.h>
#include <stdlib.h>
#include <cmath>

#define DEBUG 1

constexpr static double pi = 3.1415926535;
constexpr static double seconds_per_frame = 0.02;
constexpr static int no_stage = -1;
constexpr static int no_object = 0;
constexpr static double v_max = 6;
constexpr static double v_min = -2;
constexpr static double operate_distance = 0.4;// 机器人操作工作台的最远距离


//计算距离
inline double distance(double x1, double y1, double x2, double y2) {
    return pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
}

//计算取模
inline double usermod(double x) {
    if (x<0){
        x = x+2*pi;
    }
    return x;
}

// 子函数：处理char型数组，按照空格切割并翻译为浮点数
inline void parse_char(char *line, double *temp_arr) {
    char delims[] = " ";
    char *temp = NULL;
    temp = strtok(line, delims);
    int i = 0;
    while (temp != NULL) {
        temp_arr[i] = atof(temp);
        temp = strtok(NULL, delims);
        i++;
    }
}
