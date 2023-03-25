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
#include "Consumer.hpp"
#include "windows.h"

//#define _CRT_SECURE_NO_WARNINGS // 改为在编辑器设置
using namespace std;



// 1.本地测试
//int main() {
//    // 初始化地图测试
//    perror("tag1");
//    Map my_map;
//    FILE *file = fopen("../1.txt", "r");
//    if (!file) {
//     perror("file open error");
//     exit(1);
//    }
//    my_map.init_map(file);
//    fclose(file);
//    // 刷新地图测试
//    file = fopen("../IO1.txt", "r");
//    if (!file) {
//     perror("file open error2");
//     exit(1);
//    }
//    my_map.flush_map(file);
//    fclose(file);
//    return 0;
//}


// 2.线上测试
//int main() {
//    // 初始化成员函数
//    // Sleep(10*1000);
//    perror("Tag2");
//    Map my_map;
//    my_map.init_map(stdin);
//
//    //开始接收帧数据
//    puts("OK");
//    fflush(stdout);
//
//    //帧循环
//    int frameID;
//    while (scanf("%d", &frameID) != EOF) {
//        //读地图数据
//        my_map.flush_map(stdin);
//
//
//        //输出计算结果
//        printf("%d\n", frameID);
//        int lineSpeed = 3;
//        double angleSpeed = 1.5;
//        for (int robotId = 0; robotId < 4; robotId++) {
//            printf("forward %d %d\n", robotId, lineSpeed);
//            printf("rotate %d %f\n", robotId, angleSpeed);
//        }
//        printf("OK\n");
//        fflush(stdout);
//    }
//    return 0;
//
//}

// 3.机器人执行测试
//int main() {
//    // 初始化成员函数
//    Sleep(10*1000);
//    perror("Tag3");
//    Producer my_producer;
//    Consumer my_consumer;
//    Map my_map;
//    my_map.init_map(stdin);
//
//    //开始接收帧数据
//    puts("OK");
//    fflush(stdout);
//
//    //帧循环
//    int frameID;
//    while (scanf("%d", &frameID) != EOF) {
//        printf("%d\n", frameID);
//        //1.刷新地图数据
//        my_map.flush_map(stdin);
//
//        //2.生产者产生任务
//        //3.工作台产生任务
//        //4.消费者分配任务
//        //5.机器人执行任务
//        Robot &select_robot = my_map.robot_arr[0];
//        // 5.1 测试，自定义任务
//        Task temptask = {&my_map.stage_arr[0][0],&my_map.stage_arr[3][0]};
//
//        //确定结果输出
//        printf("OK\n");
//        fflush(stdout);
//    }
//    return 0;
//
//}




// 4.合成测试
int main() {
    // 初始化成员函数
    Sleep(8 * 1000);
    Producer my_producer;
    Consumer my_consumer;
    Map my_map;
    my_map.init_map(stdin);

    //开始接收帧数据
    puts("OK\n");
    fflush(stdout);
    //帧循环
    int frameID;
    while (scanf("%d", &frameID) != EOF) {
        printf("%d\n", frameID);
        //cerr<<"frameID:"<<frameID<<"\n";
        //1.刷新地图数据
        my_map.flush_map(stdin, frameID);

        //2.生产者产生任务
        //3.工作台产生任务
        //fprintf(stderr,"-------------------------------------------------\n");
        for (int stageId = 0; stageId < 9; stageId++) {
            for (int j = 0; j < my_map.stage_arr[stageId].size(); j++) {
                Stage &select_stage = my_map.stage_arr[stageId][j];
                select_stage.tick(my_producer);
            }
        }
        //4.消费者分配任务
        //5.机器人执行任务
        for (int robotId = 0; robotId < 4; robotId++) {
            Robot &select_robot = my_map.robot_arr[robotId];
            if (!my_producer.is_empty()) {
                //5.1 空闲机器人向Consumer申请任务
                if (!select_robot.is_busy()) {
                    Task *todo_task = my_consumer.get_task(my_producer, my_map, select_robot);
                    if(todo_task->to_stage != nullptr) {
                        select_robot.rcv_task(*todo_task);
                        fprintf(stderr, "from_stage_id: %d(%f,%f) ==> to_stage_id: %d(%f,%f)\n",
                                todo_task->from_stage->stage_id, todo_task->from_stage->pos_x, todo_task->from_stage->pos_y,
                                todo_task->to_stage->stage_id, todo_task->to_stage->pos_x, todo_task->to_stage->pos_y);
                    }
                }
#if DEBUG
                if (select_robot.doing!=NULL){
                    fprintf(stderr, "robotID: %d,(%f,%f),stageId: %d,(%f,%f)\n",
                            select_robot.id,select_robot.pos_x,select_robot.pos_y,
                            select_robot.doing->stage->stage_id,select_robot.doing->stage->pos_x,select_robot.doing->stage->pos_y);}// 用于定位问题
#endif
            }
            select_robot.tick(my_producer);
        }
        //确定结果输出
        cout << "OK" << endl;
        fflush(stdout);
    }
    return 0;

}

