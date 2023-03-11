#include <iostream>
#include "bits/stdc++.h"

using namespace std;

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
    bool is_busy;//空闲状态
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
};

//任务
class Task {

};

//生产者
class Producer {
public:
    Task get_task();//获取目标工作台任务
    queue<Task> task_queue;//目标工作台任务队列
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
    Stage stage_arr[50];//工作台序列
    Robot robot_arr[4];//机器人序列
};

// 子函数：处理char型数组，按照空格切割并翻译为浮点数
void parse_char(char * line,float * temp_arr){
    char delims[] = " ";
    char *temp = NULL;
    temp = strtok( line, delims );
    int i = 0;
    while( temp != NULL ) {
        temp_arr[i]  = atof(temp);
        temp = strtok( NULL, delims );
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
        if (rows_count == 0){
            parse_char(line,temp_arr);
            map->frame = temp_arr[0];
            map->money = temp_arr[1];
        }
        // 处理第二行-1+map->stage_num：工作台
        else if(rows_count == 1){
            parse_char(line,temp_arr);
            map->stage_num= temp_arr[0];
        }
        else if(rows_count <= 1+map->stage_num){
            parse_char(line,temp_arr);
            map->stage_arr[rows_count-2].stage_id= temp_arr[0];
            map->stage_arr[rows_count-2].pos_x= temp_arr[1];
            map->stage_arr[rows_count-2].pos_y= temp_arr[2];
            map->stage_arr[rows_count-2].rest_time= temp_arr[3];
            map->stage_arr[rows_count-2].material_status = temp_arr[4];
            map->stage_arr[rows_count-2].product_status= temp_arr[5];
        }
        // 处理剩余行：机器人
        else{
            map->robot_arr[rows_count-(2+map->stage_num)].stage_id= temp_arr[0];
            map->robot_arr[rows_count-(2+map->stage_num)].object_id= temp_arr[1];
            map->robot_arr[rows_count-(2+map->stage_num)].time_value_coef= temp_arr[2];
            map->robot_arr[rows_count-(2+map->stage_num)].crash_value_coef= temp_arr[3];
            map->robot_arr[rows_count-(2+map->stage_num)].v_rad= temp_arr[4];
            map->robot_arr[rows_count-(2+map->stage_num)].v_x= temp_arr[5];
            map->robot_arr[rows_count-(2+map->stage_num)].v_y= temp_arr[6];
            map->robot_arr[rows_count-(2+map->stage_num)].pos_rad= temp_arr[7];
            map->robot_arr[rows_count-(2+map->stage_num)].pos_x= temp_arr[8];
            map->robot_arr[rows_count-(2+map->stage_num)].pos_y= temp_arr[9];
        }
        rows_count++;
    }
}


int main() {
    // 初始化地图
    FILE *file;
    fopen_s(&file, "..\\1.txt", "r");
    Map my_map = init_map(file);
    fclose(file);

    fopen_s(&file, "..\\IO1.txt", "r");
    flush_map(file,&my_map);
    fclose(file);

}
