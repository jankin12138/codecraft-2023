#include <iostream>
#include <bits/stdc++.h>
#include <string>

#include "Map.hpp"
#include "Object.hpp"
#include "Producer.hpp"
#include "Robot.hpp"
#include "Stage.hpp"
#include "Task.hpp"
#include "util.hpp"

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

//判断原材料格占用情况
bool material_exist(Stage &stage, Robot &robot) {
    int id = robot.task.init_stage_id;
    int material_tmp = stage.material_status;
    return (material_tmp >> id) & 1 == 1;
}

//找到最近的任务目标点
void find_nearest_pos(Map &map, Robot &robot, bool flag) {
    int target_stage_id;
    double dis;
    double min_distance = INT32_MAX;
    //为真是获取物品位置，为假是出售位置
    if (flag) {
        target_stage_id = robot.task.init_stage_id;
    } else {
        target_stage_id = robot.task.target_stage_id;
    }
    for (auto arr: map.stage_arr[target_stage_id]) {
        //判断是否有物品
        if (flag && arr.product_status != 1) {
            continue;
        }
        //判断目标工作台原料格是否被占用
        if (!flag && !material_exist(arr, robot)) {
            continue;
        }
        //计算符合条件的最近的距离
        dis = distance(robot.pos_x, robot.pos_y, arr.pos_x, arr.pos_y);
        if (dis < min_distance) {
            min_distance = dis;
            robot.task.target_x = arr.pos_x;
            robot.task.target_y = arr.pos_y;
        }
    }
}


// 子函数：处理char型数组，按照空格切割并翻译为浮点数
void parse_char(char *line, double *temp_arr) {
    char delims[] = " ";
    char *temp = NULL;
    char *context_ptr = NULL;
    temp = strtok_s(line, delims, &context_ptr); // vs提示strtok不安全,改用strtok_s
    int i = 0;
    while (temp != NULL) {
        temp_arr[i] = atof(temp);
        temp = strtok_s(NULL, delims, &context_ptr);
        i++;
    }
}

// 从地图读取数据：从本地文件刷新Map,提交代码需要从stdin初始化
Map init_map(FILE *file) {
    Map map;
    map.frame = 0;
    char line[1024];
    int robot_count = 0;
    int stage_count = 0;
    int rows_count = 0;
    //while (fgets(line, sizeof line, stdin)) {
    while (fgets(line, sizeof line, file)) {
        if (line[0] == 'O' && line[1] == 'K') {
            break;
        }
        // 解析每一行数据
        for (int i = 0; i < strlen(line); i++) {
            // 初始化机器人
            if (line[i] == 'A') {
                map.robot_arr[robot_count].pos_x = (i + 1) * 0.5;
                map.robot_arr[robot_count].pos_y = 50.0 - (rows_count + 1) * 0.5;
                robot_count++;
            }
                // 初始化工作台
            else if (line[i] >= '1' && line[i] <= '9') {
                Stage tempstage = {int(line[i] - '0'), (i + 1) * 0.5, 50.0 - (rows_count + 1) * 0.5, -1, 0, 0};
                map.stage_arr[int(line[i] - '1')].push_back(tempstage);
                stage_count++;
            }
        }
        rows_count++;
    }
    map.stage_num = stage_count;
    return map;
}


// 从每一帧刷新数据：从本地文件初始化Map,提交代码需要从stdin初始化
void flush_map(FILE *file, Map *map) {
    char line[1024];
    int rows_count = 0;        // 帧结构行数计数
    int stage_counts[9] = {0}; // 工作台各类型数量计数
    double temp_arr[10];        // 开辟解析数据用的临时空间
    //while (fgets(line, sizeof line, stdin)) {
    while (fgets(line, sizeof line, file)) {
        if (line[0] == 'O' && line[1] == 'K') {
            break;
        }
        // 处理第一行：帧数、金钱
        if (rows_count == 0) {
            parse_char(line, temp_arr);
            map->frame = temp_arr[0];
            map->money = temp_arr[1];
        }
            // 处理第二行-1+map->stage_num：工作台
        else if (rows_count == 1) {
            parse_char(line, temp_arr);
            map->stage_num = temp_arr[0];
        } else if (rows_count <= 1 + map->stage_num) {
            parse_char(line, temp_arr);
            Stage &stage = map->stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]];
            stage.stage_id = temp_arr[0];
            stage.pos_x = temp_arr[1];
            stage.pos_y = temp_arr[2];
            stage.rest_time = temp_arr[3];
            stage.material_status = temp_arr[4];
            stage.product_status = temp_arr[5];
            stage_counts[int(temp_arr[0]) - 1]++;
        }
            // 处理剩余行：机器人
        else {
            Robot &robot = map->robot_arr[rows_count - (2 + map->stage_num)];
            robot.stage_id = temp_arr[0];
            robot.object_id = temp_arr[1];
            robot.time_value_coef = temp_arr[2];
            robot.crash_value_coef = temp_arr[3];
            robot.v_rad = temp_arr[4];
            robot.v_x = temp_arr[5];
            robot.v_y = temp_arr[6];
            robot.pos_rad = temp_arr[7];
            robot.pos_x = temp_arr[8];
            robot.pos_y = temp_arr[9];
        }
        rows_count++;
    }
}

int main() {
    // 初始化地图测试
    FILE *file = fopen("./1.txt", "r");
    if (!file) {
        perror("file open error");
        exit(1);
    }
    Map my_map = init_map(file);
    fclose(file);
    // 刷新地图测试
    file = fopen("./IO1.txt", "r");
    if (!file) {
        perror("file open error2");
        exit(1);
    }
    flush_map(file, &my_map);
    fclose(file);
    return 0;

}
