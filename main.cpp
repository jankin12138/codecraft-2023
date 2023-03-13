#include <iostream>
#include <bits/stdc++.h>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;
using namespace std::chrono;

constexpr static double seconds_per_frame = 0.015;

inline float distance(double x1, double y1, double x2, double y2) {
    return pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
}

//物品
class Object {
    int buy_price;//购买价格
    int sell_price;//售出价格
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

//机器人
class Robot {
public:
    int stage_id;//所处工作台, -1 表示当前没有处于任何工作台附近, [0,工作台总数-1] 表示某工作台的下标。当前机器人的所有购买、出售行为均针对该工作台进行。
    int object_id;//物品id, 0 表示未携带物品, 1-7 表示对应物品
    float time_value_coef;//时间价值系数
    float crash_value_coef;//碰撞价值系数
    float v_rad;//角速度, 逆时针为正, 单位弧度每秒, [-pi, pi]
    float v_x;//线速度
    float v_y;//线速度
    float pos_rad;//朝向
    float pos_x;//x坐标
    float pos_y;//y坐标
    bool is_busy;//空闲状态
    int id; // 机器人id[0, 3], 目前一共只有4个机器人
    Stage const *target_stage;
    constexpr static double v_max = 6;
    constexpr static double v_min = -2;
    constexpr static double v_rad_max = 3.14159;
    constexpr static double operateDistance = 0.4;// 机器人操作工作台的最远距离
    constexpr static int no_stage = -1;
    constexpr static int no_object = 0;

    Robot() : id(-1), pos_x(-1), pos_y(-1), stage_id(no_stage), object_id(no_object) {
    }

    void forward(float v) {
        cout << "forward" << ' ' << id << ' ' << v << endl;
    }

    void rotate(float v) {
        cout << "rotate" << ' ' << id << ' ' << v << endl;
    }

    void buy() {
        cout << "buy" << ' ' << id << endl;
    }

    void sell() {
        cout << "sell" << ' ' << id << endl;
    }

    void destroy() {
        cout << "destroy" << ' ' << id << endl;
    }

    void go_to_stage(Stage const &stage) {
        assert(!is_busy); // 在空闲时才能前往下一个工作台
        target_stage = &stage;
        is_busy = true;
    }

    /// 每帧调用，生成机器人行为对应输出
    void tick() {
        if (!is_busy)
            return;

        double dist = distance(pos_x, pos_y, target_stage->pos_x, target_stage->pos_y);
        double target_rad = atan((target_stage->pos_y - pos_y) / (target_stage->pos_x - pos_x));
        double dist_rad = target_rad - pos_rad;
        if (dist_rad) {
            // 转动
            if (dist_rad > 0)
                rotate(min(3.14159, dist_rad / seconds_per_frame));
            else
                rotate(max(-3.14159, dist_rad / seconds_per_frame));
            forward(0);
        } else if (dist >= operateDistance) {
            // 前进
            rotate(0);
            forward(min(6.0, dist / seconds_per_frame));
        } else {
            // 已到达目标工作台附近
            is_busy = false;
            forward(0);
            rotate(0);
            return;
        }
    }

};

istream &operator>>(istream &in, Robot &robot) {
    in >> robot.stage_id >> robot.object_id >> robot.time_value_coef >> robot.crash_value_coef >> robot.v_rad
       >> robot.v_x >> robot.v_y >> robot.pos_rad >> robot.pos_x >> robot.pos_y;
    return in;
}

//任务
class Task {

};

//生产者
class Producer {
public:
    //Task get_task();//获取目标工作台任务
    queue<Task> task_queue;//目标工作台任务队列
};

//消费者
class Consumer {
public:
    //void distribution_task();//任务分配
    queue<Robot> robot_queue;//空闲机器人
};

//地图
class Map {
public:
    int frame;//当前帧数
    int money;//金钱数
    int stage_num;      //工作台数量
    int robot_num = 4;  //机器人数量
    vector<Stage> stage_arr[9];//工作台序列 1-9
    Robot robot_arr[4];//机器人序列
};

// 子函数：处理char型数组，按照空格切割并翻译为浮点数
void parse_char(char* line, float* temp_arr) {
    char delims[] = " ";
    char* temp = NULL;
    char* context_ptr = NULL;
    temp = strtok_s(line, delims,&context_ptr); // vs提示strtok不安全,改用strtok_s
    int i = 0;
    while (temp != NULL) {
        temp_arr[i] = atof(temp);
        temp = strtok_s(NULL, delims,&context_ptr);
        i++;
    }
}

// 从地图读取数据：从本地文件刷新Map,提交代码需要从stdin初始化
Map init_map(FILE* file) {
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
                Stage tempstage = { int(line[i] - '0') ,(i + 1) * 0.5 ,50.0 - (rows_count + 1) * 0.5,-1,0,0 };
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
void flush_map(FILE* file, Map* map) {
    char line[1024];
    int rows_count = 0;        // 帧结构行数计数
    int stage_counts[9] = {0}; // 工作台各类型数量计数
    float temp_arr[10];        // 开辟解析数据用的临时空间
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
        }
        else if (rows_count <= 1 + map->stage_num) {
            parse_char(line, temp_arr);
            map->stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]].stage_id = temp_arr[0];
            map->stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]].pos_x = temp_arr[1];
            map->stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]].pos_y = temp_arr[2];
            map->stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]].rest_time = temp_arr[3];
            map->stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]].material_status = temp_arr[4];
            map->stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]].product_status = temp_arr[5];
            stage_counts[int(temp_arr[0]) - 1]++;
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

void test_robot() {
    Robot robot;
    Stage stage;
    robot.id = 0;
    robot.pos_x = 0;
    stage.pos_x = 10;
    stage.pos_y = 5;
    robot.go_to_stage(stage);
    while (robot.is_busy) {
        robot.tick();
    }
}

int main() {
    // 初始化地图测试
    FILE* file;
    errno_t err = fopen_s(&file, "1.txt", "r");
    if (err != 0) {
        printf("文件打开失败，错误代码：%d\n", err);
        return 1;
    }
    Map my_map = init_map(file);
    fclose(file);
    // 刷新地图测试
    err = fopen_s(&file, "IO1.txt", "r");
    if (err != 0) {
        printf("文件打开失败，错误代码：%d\n", err);
        return 1;
    }
    flush_map(file, &my_map);
    fclose(file);
    return 0;

}
