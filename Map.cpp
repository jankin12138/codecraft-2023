#include "Map.hpp"
#include "util.hpp"
#include "Stage.hpp"

// 从地图读取数据：从本地文件刷新Map,提交代码需要从stdin初始化
void Map::init_map(FILE *file) {
    char line[1024];
    int robot_count = 0;
    int stage_count = 0;
    int rows_count = 0;
    frame = 0; // 初始化帧数
    //while (fgets(line, sizeof line, stdin)) {
    while (fgets(line, sizeof line, file)) {
        if (line[0] == 'O' && line[1] == 'K') {
            break;
        }
        // 解析每一行数据
        for (int i = 0; i < strlen(line); i++) {
            // 初始化机器人
            if (line[i] == 'A') {
                robot_arr[robot_count].pos_x = (i + 1) * 0.5;
                robot_arr[robot_count].pos_y = 50.0 - (rows_count + 1) * 0.5;
                robot_arr[robot_count].id = robot_count;
                robot_count++;
            }
                // 初始化工作台
            else if (line[i] >= '1' && line[i] <= '9') {
                Stage temp_stage = {stage_count,int(line[i] - '0'), (i + 1) * 0.5, 50.0 - (rows_count + 1) * 0.5, -1, 0, 0};
                stage_arr[int(line[i] - '1')].push_back(temp_stage);
                stage_count++;
            }
        }
        rows_count++;
    }
    stage_num = stage_count; // 初始化工作台数量
}


// 从每一帧刷新数据：从本地文件初始化Map,提交代码需要从stdin初始化
void Map::flush_map(FILE *file,int frameID) {
    char line[1024];
    int rows_count = 0;          // 帧结构行数计数
    int stage_counts[9] = {0}; // 工作台各类型数量计数
    double temp_arr[10];         // 开辟解析数据用的临时空间
    //while (fgets(line, sizeof line, stdin)) {
    while (fgets(line, sizeof line, file)) {

        if (line[0] == 'O' && line[1] == 'K') {
            break;
        }
        parse_char(line, temp_arr);
        // 处理第一行：帧数、金钱
        if (rows_count == 0) {
            frame = frameID;
            money = temp_arr[0];
        }
        // 处理第二行-1+map->stage_num：工作台
        else if (rows_count == 1) {
            stage_num = temp_arr[0];
        }
        else if (rows_count <= 1 + stage_num) {
            Stage &stage = stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]];  // 获取需要刷新的工作台
            stage.stage_sn = rows_count-2;
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
            Robot &robot = robot_arr[rows_count - (2 + stage_num)]; // 获取需要刷新的机器人
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
            robot.id = rows_count - (2 + stage_num);
        }
        rows_count++;
    }
}

