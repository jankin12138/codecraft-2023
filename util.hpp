#pragma once

#include <cmath>
#include <string.h>
#include <stdlib.h>
#include <cmath>

#define DEBUG 0

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

inline double calc_rad(double delta_x, double delta_y) {
    if (delta_x == 0)
        return delta_y > 0 ? pi / 2 : -pi / 2;
    return atan(delta_y / delta_x) + (delta_x > 0 ? 0 : pi);
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

inline int countBits(unsigned long long n) {
    int number = 0;
    int b = n;
    while (b > 0) {
        if (b % 2 == 1) {
            number++;
        }
        b = b / 2;
    }
    return number;
}

inline double ParamGive(int num7, int num8) {
    if (num7 == 0)  //图3
    {
        return 4.0;
    }
    else if(num7 >= 8)//图1
    {
        return 5.9;
    }
    else if(num7 == 2 && num8 == 2) //图2
    {
        return 4.25;
    }
    else // 图4和其他情况
    {
        return 6;
    }
}