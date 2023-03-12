//
// Created by Administrator on 2023/3/12.
//

#include "lib.h"
#include <iostream>
#include <bits/stdc++.h>

//常量区
const int ans_task[6][2] = {{3, 6},
                            {2, 6},
                            {3, 5},
                            {1, 5},
                            {2, 4},
                            {1, 4}};//固定任务分配顺序
//任务类构造函数
Task::Task(int a, int b) {
    init_stage_id = a;
    target_stage_id = b;
}

//生产者创建任务队列
Task Producer::get_task() {
    if (task_queue.size() == 0) {
        //按固定顺序放入任务队列
        for (int i = 0; i < 6; i++) {
            task_queue.push_back(Task(ans_task[i][0], ans_task[i][1]));
        }
    }
    Task res = task_queue.front();
    task_queue.pop_front();
    return res;
};

//工作台完成创建后调用此函数通知生产者
void Stage::notify_producer(Producer &p) {
    //需要合成的物品有更高优先级
    if (stage_id < 3) return;
    p.task_queue.push_front(Task(stage_id, 7));
}

// 子函数：处理char型数组，按照空格切割并翻译为浮点数
void parse_char(char *line, float *temp_arr) {
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

// 从地图读取数据：从本地文件初始化Map,提交代码需要从stdin初始化
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
                map.stage_arr[stage_count].stage_id = int(line[i] - '0');
                map.stage_arr[stage_count].pos_x = (i + 1) * 0.5;
                map.robot_arr[stage_count].pos_y = 50.0 - (rows_count + 1) * 0.5;
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
    map->frame = 0;
    char line[1024];
    int rows_count = 0;
    //while (fgets(line, sizeof line, stdin)) {
    while (fgets(line, sizeof line, file)) {
        if (line[0] == 'O' && line[1] == 'K') {
            break;
        }
        float temp_arr[10];
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
            map->stage_arr[rows_count - 2].stage_id = temp_arr[0];
            map->stage_arr[rows_count - 2].pos_x = temp_arr[1];
            map->stage_arr[rows_count - 2].pos_y = temp_arr[2];
            map->stage_arr[rows_count - 2].rest_time = temp_arr[3];
            map->stage_arr[rows_count - 2].material_status = temp_arr[4];
            map->stage_arr[rows_count - 2].product_status = temp_arr[5];
        }
            // 处理剩余行：机器人
        else {
            map->robot_arr[rows_count - (2 + map->stage_num)].stage_id = temp_arr[0];
            map->robot_arr[rows_count - (2 + map->stage_num)].object_id = temp_arr[1];
            map->robot_arr[rows_count - (2 + map->stage_num)].time_value_coef = temp_arr[2];
            map->robot_arr[rows_count - (2 + map->stage_num)].crash_value_coef = temp_arr[3];
            map->robot_arr[rows_count - (2 + map->stage_num)].v_rad = temp_arr[4];
            map->robot_arr[rows_count - (2 + map->stage_num)].v_x = temp_arr[5];
            map->robot_arr[rows_count - (2 + map->stage_num)].v_y = temp_arr[6];
            map->robot_arr[rows_count - (2 + map->stage_num)].pos_rad = temp_arr[7];
            map->robot_arr[rows_count - (2 + map->stage_num)].pos_x = temp_arr[8];
            map->robot_arr[rows_count - (2 + map->stage_num)].pos_y = temp_arr[9];
        }
        rows_count++;
    }
}