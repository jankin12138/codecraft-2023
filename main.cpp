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




int main() {
    // 1.本地测试
    // 初始化地图测试
//     Map my_map;
//     FILE *file = fopen("../1.txt", "r");
//     if (!file) {
//         perror("file open error");
//         exit(1);
//     }
//     my_map.init_map(file);
//     fclose(file);
//     // 刷新地图测试
//     file = fopen("../IO1.txt", "r");
//     if (!file) {
//         perror("file open error2");
//         exit(1);
//     }
//     my_map.flush_map(file);
//     fclose(file);
//     return 0;

    // 2.线上测试

    //初始化成员函数
    Map my_map;
    my_map.init_map(stdin);

    //开始接收帧数据
    puts("OK");
    fflush(stdout);

    //帧循环
    int frameID;
    while (scanf("%d", &frameID) != EOF) {
        //读地图数据
        my_map.flush_map(stdin);


        //输出计算结果
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
