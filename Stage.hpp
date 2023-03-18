#pragma once

#include <cassert>
#include <vector>

#include "Producer.hpp"
#include "Stage.hpp"
#include "util.hpp"

class Stage {
public:
    int stage_id;   // 工作台类型
    double pos_x;   // x坐标
    double pos_y;   // y坐标
    constexpr static int not_producing = -1;
    constexpr static int blocking = 0;
    int rest_time;  // 剩余生产时间, -1表示不在生产, 0表示生产因输出格满而阻塞, >=0表示剩余生产帧数
    int material_status;    // 原材料格状态,位表表示
    int product_status;     // 产品格状态, 0为无, 1为有
    int count = 0; //已经生产的个数用于判断分配给哪个工作台
    void notify_producer(Producer &p);  // 工作台完成创建后调用此函数通知生产者
    int product_object_id() const;      // 生产物品id
    bool is_raw_material(int object_id) const;

    std::vector<int> get_raw_material_ids() const;

    /// 返回是否成功接受
    bool rcv_raw_material(int object_id);

    bool is_raw_materials_ready() const;

    int produce_time();

    /// 每帧调用
    void tick(Producer &p);
};
