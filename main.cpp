#include <iostream>
#include <bits/stdc++.h>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;
using namespace std::chrono;

constexpr static double seconds_per_frame = 0.015;

//������
const int ans_task[6][2] = {{3, 6},
                            {2, 6},
                            {3, 5},
                            {1, 5},
                            {2, 4},
                            {1, 4}};//�̶��������˳��


//�������
inline float distance(double x1, double y1, double x2, double y2) {
    return pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
}

//����
class Task {
public:
    Task() {};

    Task(int a, int b);

    int init_stage_id;//��ȡ��Ʒ����̨����
    int target_stage_id;//������Ʒ����̨����
    float target_x;//Ŀ�깤��̨x����
    float target_y;//Ŀ�깤��̨y����
};

//�����๹�캯��
Task::Task(int a, int b) {
    init_stage_id = a;
    target_stage_id = b;
}

//��Ʒ
class Object {
    int buy_price;//����۸�
    int sell_price;//�۳��۸�
};

//������
class Producer {
public:
    Task get_task();//��ȡĿ�깤��̨����
    deque<Task> task_queue;//Ŀ�깤��̨�������
};

//�����ߴ����������
Task Producer::get_task() {
    if (task_queue.empty()) {
        //���̶�˳������������
        for (auto ans: ans_task) {
            task_queue.emplace_back(Task(ans[0], ans[1]));
        }
    }
    Task res = task_queue.front();
    task_queue.pop_front();
    return res;
};

//����̨
class Stage {
public:
    int stage_id;//����̨����
    float pos_x;//x����
    float pos_y;//y����
    int rest_time;//ʣ������ʱ��
    int material_status;//ԭ���ϸ�״̬,λ���ʾ
    int product_status;//��Ʒ��״̬
    void notify_producer(Producer &p);//����������
};

//����̨��ɴ�������ô˺���֪ͨ������
void Stage::notify_producer(Producer &p) {
    //��Ҫ�ϳɵ���Ʒ�и������ȼ�
    if (stage_id < 4) return;
    if (stage_id < 7)p.task_queue.push_front(Task(stage_id, 7));
    if (stage_id == 7) p.task_queue.push_front(Task(7, 8));
}


//������
class Robot {
public:
    int stage_id;//��������̨, -1 ��ʾ��ǰû�д����κι���̨����, [0,����̨����-1] ��ʾĳ����̨���±ꡣ��ǰ�����˵����й��򡢳�����Ϊ����Ըù���̨���С�
    int object_id;//��Ʒid, 0 ��ʾδЯ����Ʒ, 1-7 ��ʾ��Ӧ��Ʒ
    float time_value_coef;//ʱ���ֵϵ��
    float crash_value_coef;//��ײ��ֵϵ��
    float v_rad;//���ٶ�, ��ʱ��Ϊ��, ��λ����ÿ��, [-pi, pi]
    float v_x;//���ٶ�
    float v_y;//���ٶ�
    float pos_rad;//����
    float pos_x;//x����
    float pos_y;//y����
    bool is_busy;//����״̬
    int id; // ������id[0, 3], Ŀǰһ��ֻ��4��������
    Stage const *target_stage;
    Task task;//��ǰִ�е�����
    constexpr static double v_max = 6;
    constexpr static double v_min = -2;
    constexpr static double v_rad_max = 3.14159;
    constexpr static double operateDistance = 0.4;// �����˲�������̨����Զ����
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
        assert(!is_busy); // �ڿ���ʱ����ǰ����һ������̨
        target_stage = &stage;
        is_busy = true;
    }

    /// ÿ֡���ã����ɻ�������Ϊ��Ӧ���
    void tick() {
        if (!is_busy)
            return;

        double dist = distance(pos_x, pos_y, target_stage->pos_x, target_stage->pos_y);
        double target_rad = atan((target_stage->pos_y - pos_y) / (target_stage->pos_x - pos_x));
        double dist_rad = target_rad - pos_rad;
        if (dist_rad) {
            // ת��
            if (dist_rad > 0)
                rotate(min(3.14159, dist_rad / seconds_per_frame));
            else
                rotate(max(-3.14159, dist_rad / seconds_per_frame));
            forward(0);
        } else if (dist >= operateDistance) {
            // ǰ��
            rotate(0);
            forward(min(6.0, dist / seconds_per_frame));
        } else {
            // �ѵ���Ŀ�깤��̨����
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


//������
class Consumer {
public:
    //void distribution_task();//�������
    queue<Robot> robot_queue;//���л�����
};

//��ͼ
class Map {
public:
    int frame;//��ǰ֡��
    int money;//��Ǯ��
    int stage_num;      //����̨����
    int robot_num = 4;  //����������
    vector<Stage> stage_arr[9];//����̨���� 1-9
    Robot robot_arr[4];//����������
};

//�ж�ԭ���ϸ�ռ�����
bool material_exist(Stage &stage, Robot &robot) {
    int id = robot.task.init_stage_id;
    int material_tmp = stage.material_status;
    return (material_tmp >> id) & 1 == 1;
}

//�ҵ����������Ŀ���
void find_nearest_pos(Map &map, Robot &robot, bool flag) {
    int target_stage_id;
    float dis;
    float min_distance = INT32_MAX;
    //Ϊ���ǻ�ȡ��Ʒλ�ã�Ϊ���ǳ���λ��
    if (flag) {
        target_stage_id = robot.task.init_stage_id;
    } else {
        target_stage_id = robot.task.target_stage_id;
    }
    for (auto arr: map.stage_arr[target_stage_id]) {
        //�ж��Ƿ�����Ʒ
        if (flag && arr.product_status != 1) {
            continue;
        }
        //�ж�Ŀ�깤��̨ԭ�ϸ��Ƿ�ռ��
        if (!flag && !material_exist(arr, robot)) {
            continue;
        }
        //�����������������ľ���
        dis = distance(robot.pos_x, robot.pos_y, arr.pos_x, arr.pos_y);
        if (dis < min_distance) {
            min_distance = dis;
            robot.task.target_x = arr.pos_x;
            robot.task.target_y = arr.pos_y;
        }
    }
}


// �Ӻ���������char�����飬���տո��и����Ϊ������
void parse_char(char *line, float *temp_arr) {
    char delims[] = " ";
    char *temp = NULL;
    char *context_ptr = NULL;
    temp = strtok_s(line, delims, &context_ptr); // vs��ʾstrtok����ȫ,����strtok_s
    int i = 0;
    while (temp != NULL) {
        temp_arr[i] = atof(temp);
        temp = strtok_s(NULL, delims, &context_ptr);
        i++;
    }
}

// �ӵ�ͼ��ȡ���ݣ��ӱ����ļ�ˢ��Map,�ύ������Ҫ��stdin��ʼ��
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
        // ����ÿһ������
        for (int i = 0; i < strlen(line); i++) {
            // ��ʼ��������
            if (line[i] == 'A') {
                map.robot_arr[robot_count].pos_x = (i + 1) * 0.5;
                map.robot_arr[robot_count].pos_y = 50.0 - (rows_count + 1) * 0.5;
                robot_count++;
            }
                // ��ʼ������̨
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


// ��ÿһ֡ˢ�����ݣ��ӱ����ļ���ʼ��Map,�ύ������Ҫ��stdin��ʼ��
void flush_map(FILE *file, Map *map) {
    char line[1024];
    int rows_count = 0;        // ֡�ṹ��������
    int stage_counts[9] = {0}; // ����̨��������������
    float temp_arr[10];        // ���ٽ��������õ���ʱ�ռ�
    //while (fgets(line, sizeof line, stdin)) {
    while (fgets(line, sizeof line, file)) {
        if (line[0] == 'O' && line[1] == 'K') {
            break;
        }
        // �����һ�У�֡������Ǯ
        if (rows_count == 0) {
            parse_char(line, temp_arr);
            map->frame = temp_arr[0];
            map->money = temp_arr[1];
        }
            // ����ڶ���-1+map->stage_num������̨
        else if (rows_count == 1) {
            parse_char(line, temp_arr);
            map->stage_num = temp_arr[0];
        } else if (rows_count <= 1 + map->stage_num) {
            parse_char(line, temp_arr);
            map->stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]].stage_id = temp_arr[0];
            map->stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]].pos_x = temp_arr[1];
            map->stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]].pos_y = temp_arr[2];
            map->stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]].rest_time = temp_arr[3];
            map->stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]].material_status = temp_arr[4];
            map->stage_arr[int(temp_arr[0]) - 1][stage_counts[int(temp_arr[0]) - 1]].product_status = temp_arr[5];
            stage_counts[int(temp_arr[0]) - 1]++;
        }
            // ����ʣ���У�������
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
    // ��ʼ����ͼ����
    FILE *file;
    errno_t err = fopen_s(&file, "1.txt", "r");
    if (err != 0) {
        printf("�ļ���ʧ�ܣ�������룺%d\n", err);
        return 1;
    }
    Map my_map = init_map(file);
    fclose(file);
    // ˢ�µ�ͼ����
    err = fopen_s(&file, "IO1.txt", "r");
    if (err != 0) {
        printf("�ļ���ʧ�ܣ�������룺%d\n", err);
        return 1;
    }
    flush_map(file, &my_map);
    fclose(file);
    return 0;

}
