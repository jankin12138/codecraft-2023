#include "Producer.hpp"
#include "Task.hpp"
#include "algorithm"

class Task;
namespace {
//常量区
    const int ans_task[6][2] = {{3, 6},
                                {2, 6},
                                {3, 5},
                                {1, 5},
                                {2, 4},
                                {1, 4}};//固定任务分配顺序
}

//

Task *find_stage_id(std::unordered_map<Stage *, Task *> &task_map, int stage_id) {
    Task *res = nullptr;
    for (auto a: task_map) {
        if (a.first->stage_id == stage_id && a.second != nullptr) {
            res = a.second;
            task_map[a.first] = nullptr;
            break;
        }
    }
    return res;
}

Task *Producer::get_task_front() {
    Task *tmp = nullptr;

    if (stage_id_ans[7] != 0) {
        tmp = find_stage_id(task_map, 7);
        stage_id_ans[7]--;
        count_task_ans[7]++;
    } else if (stage_id_ans[4] != 0 || stage_id_ans[5] != 0 || stage_id_ans[6] != 0) {
        std::vector<std::vector<int>> tmp_ans = {{4, count_task_ans[4]},
                                                 {5, count_task_ans[5]},
                                                 {6, count_task_ans[6]}};
        sort(tmp_ans.begin(), tmp_ans.end());
        for (auto a: tmp_ans) {
            if (stage_id_ans[a[0]] != 0) {
                tmp = find_stage_id(task_map, a[0]);
                stage_id_ans[a[0]]--;
                count_task_ans[a[0]]++;
            }
        }
    } else if (stage_id_ans[1] != 0 || stage_id_ans[2] != 0 || stage_id_ans[3] != 0) {
        std::vector<std::vector<int>> tmp_ans = {{1, count_task_ans[1]},
                                                 {2, count_task_ans[2]},
                                                 {3, count_task_ans[3]}};
        sort(tmp_ans.begin(), tmp_ans.end());
        for (auto a: tmp_ans) {
            if (stage_id_ans[a[0]] != 0) {
                tmp = find_stage_id(task_map, a[0]);
                stage_id_ans[a[0]]--;
                count_task_ans[a[0]]++;
            }
        }
    }
    return tmp;
}

bool Producer::is_empty() {
    for (int i = 0; i < 10; i++) {
        if (count_task_ans[i] != 0) return false;
    }
    return true;
}
