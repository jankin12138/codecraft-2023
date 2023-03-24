#include <cstdio>
#include "Stage.hpp"
#include "Producer.hpp"
#include "Task.hpp"
#include "util.hpp"

using namespace std;

void Stage::notify_producer(Producer &p) {
    //需要合成的物品有更高优先级
    Task *a = new(Task);
    a->from_stage = this;
    if (stage_id <= 7) {
        p.stage_id_ans[stage_id]++;
        p.task_map[this] = a;
    }
}

bool Stage::is_raw_material(int object_id) const {
    assert(1 <= object_id && object_id <= 7);
    auto object_ids = get_raw_material_ids();
    for (auto id: object_ids) {
        if (id == object_id)
            return true;
    }
    return false;
}

vector<int> Stage::get_raw_material_ids() const {
    switch (stage_id) {
        case 1:
        case 2:
        case 3:
            return {};
        case 4:
            return {1, 2};
        case 5:
            return {1, 3};
        case 6:
            return {2, 3};
        case 7:
            return {4, 5, 6};
        case 8:
            return {7};
        case 9:
            return {1, 2, 3, 4, 5, 6, 7};
        default:
            assert(false);
            return {};
    }
}

bool Stage::rcv_raw_material(int object_id) {
    //fprintf(stderr, "stage.material_status: %d object_id: %d\n",this->material_status,object_id);
    //fprintf(stderr, "stage.x: %f Stage.y: %f\n",pos_x,pos_y);
    assert(is_raw_material(object_id));
    if ((material_status >> object_id) & 1)
        return false;
    material_status |= (1 << object_id);
    //fprintf(stderr, "stage.material_status: %d\n",this->material_status);
    return true;
}

bool Stage::is_raw_materials_ready() const {
    for (auto id: get_raw_material_ids()) {
        if (!((material_status >> id) & 1))
            return false;
    }
    return true;
}

int Stage::produce_time() {
    assert(1 <= stage_id && stage_id <= 9);
    if (stage_id <= 3)
        return 50;
    if (stage_id <= 6)
        return 500;
    if (stage_id == 7)
        return 1000;
    return 1;
}

void Stage::tick(Producer &p) {
    if (rest_time == not_producing) {
        if (is_raw_materials_ready()) {
            material_status = 0; // 产品格清空
            for(int i=0;i<10;i++){
                is_material_task[i] = 0;
            }
            rest_time = produce_time(); // 进入生产周期
        }
    }
    if (rest_time > 0)
        --rest_time;
    if (rest_time == blocking && product_status == 0) {
        rest_time = not_producing;
        product_status = 1;
        this->count++;
        this->notify_producer(p);//通知生产者
    }
}

int Stage::product_object_id() const {
    assert(1 <= stage_id && stage_id <= 9);
    if (stage_id <= 7) // 当stage_id<=7, stage生产的物品id与 stage_id 相同
        return stage_id;
    else // stage_id = 8, 9 为消耗型stage, 不生产物品
        return no_object;
}


