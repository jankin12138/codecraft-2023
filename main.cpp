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
// #include "windows.h"
#include "RobotCrashPreventer.hpp"

using namespace std;

// 4.合成测试
int main() {
    // 初始化成员函数
//    Sleep(0 * 1000);
    Producer my_producer;
    Consumer my_consumer;
    Map my_map;
    my_map.init_map(stdin);

    //开始接收帧数据
    puts("OK\n");
    fflush(stdout);
    RobotCrashPreventer rcp(my_map.robot_arr, 4);
    //帧循环
    int frameID;
    while (scanf("%d", &frameID) != EOF) {
        cout << frameID << endl;
#if DEBUG
        cerr << frameID << endl;
#endif
        //1.刷新地图数据
        my_map.flush_map(stdin, frameID);

        //2.生产者产生任务
        //3.工作台产生任务
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
                    if (todo_task->to_stage != nullptr) {
                        select_robot.rcv_task(*todo_task);
#if DEBUG
                        fprintf(stderr, "from_stage_id: %d(%f,%f) ==> to_stage_id: %d(%f,%f)\n",
                                todo_task->from_stage->stage_id, todo_task->from_stage->pos_x, todo_task->from_stage->pos_y,
                                todo_task->to_stage->stage_id, todo_task->to_stage->pos_x, todo_task->to_stage->pos_y);
#endif
                    }
                }
            }
            select_robot.tick(my_producer);
            //rcp.prevent_robot_crash();
        }
        //确定结果输出
        cout << "OK" << endl;
        fflush(stdout);
    }
    return 0;

}

