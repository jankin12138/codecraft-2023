# codecraft-2023
华为软挑2023

## baseline任务分工

### io

-   输入输出格式正确

### 机器人行为

- 物品购买、出售
- 机器人移动

### 任务分配

- 任务类的定义（购买地、出售地）
- 生产者消费者函数完善





## 进展

1.增加了初始化解析；

2.增加了每一帧的解析；

3.修改了接口-Map类，增加了工作台数量、机器人数量、金钱，把 vector<Stage> stage_arr[9]改为Stage stage_arr[50]，修改原因一个是后续逐帧刷新atege_arr方便，另一个是本身stage类型可以从对象获取。

```
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
```

