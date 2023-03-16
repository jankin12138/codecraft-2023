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

//#define _CRT_SECURE_NO_WARNINGS // 改为在编辑器设置
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








int main() {
     //// 1.本地测试
     //// 初始化地图测试
     //Map my_map;
     //FILE *file = fopen("./1.txt", "r");
     //if (!file) {
     //    perror("file open error");
     //    exit(1);
     //}
     //my_map.init_map(file);
     //fclose(file);
     //// 刷新地图测试
     //file = fopen("./IO1.txt", "r");
     //if (!file) {
     //    perror("file open error2");
     //    exit(1);
     //}
     //my_map.flush_map(file);
     //fclose(file);
     //return 0;

    // 2.线上测试
    Map my_map;
    my_map.init_map(stdin);
    puts("OK");
    fflush(stdout);
    int frameID;
    while (scanf("%d", &frameID) != EOF) {
        my_map.flush_map(stdin);
        printf("%d\n", frameID);
        int lineSpeed = 3;
        double angleSpeed = 1.5;
        for (int robotId = 0; robotId < 4; robotId++) {
            printf("forward %d %d\n", robotId, lineSpeed);
            printf("rotate %d %f\n", robotId, angleSpeed);
        }
        printf("OK\n");
        fflush(stdout);
    }
    return 0;

}
