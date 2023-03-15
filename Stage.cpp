#include "Stage.hpp"

using namespace std;

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
    }
}

bool Stage::rcv_raw_material(int object_id) {
    assert(is_raw_material(object_id));
    if ((material_status >> object_id) & 1)
        return false;
    material_status |= 1 << object_id;
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

void Stage::tick() {
    if (rest_time == not_producing) {
        if (is_raw_materials_ready()) {
            material_status = 0; // 产品格清空
            rest_time = produce_time(); // 进入生产周期
        }
    }
    if (rest_time > 0)
        --rest_time;
    if (rest_time == blocking && product_status == 0) {
        rest_time = not_producing;
        product_status = 1;
    }
}

int Stage::product_object_id() const {
    assert(1 <= stage_id && stage_id <= 9);
    if (stage_id <= 7) // 当stage_id<=7, stage生产的物品id与 stage_id 相同
        return stage_id;
    else // stage_id = 8, 9 为消耗型stage, 不生产物品
        return no_object;
}

void Stage::notify_producer(Producer &p) {
    //需要合成的物品有更高优先级
    if (stage_id < 4) return;
    if (stage_id < 7)p.task_queue.push_front(Task(stage_id, 7));
    if (stage_id == 7) p.task_queue.push_front(Task(7, 8));
}