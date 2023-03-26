#include <algorithm>
#include <bits/stdc++.h>
#include "Producer.hpp"
#include "Task.hpp"
#include "Robot.hpp"

class Task;
class Robot;
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
//去哈希表查找对应stage_id的Stage
Task *find_stage_id(std::unordered_map<Stage *, Task *> &task_map, int stage_id) {
    Task *res = nullptr;
    for (auto a: task_map) {
        if (a.first->stage_id == stage_id && a.second != nullptr) {
            res = a.second;
            task_map[a.first] = nullptr; // 因为不支持删除，所以只是置0？
            break;
        }
    }
    return res;
}

Task *find_stage_id3(std::unordered_map<Stage *, Task *> &task_map, int stage_id,double pos_x1,double pos_x2,double pos_y1,double pos_y2) {
    Task *res = nullptr;
    for (auto a: task_map) {
        if (a.first->stage_id == stage_id && a.second != nullptr && a.second->from_stage->pos_x <= pos_x1 &&a.second->from_stage->pos_x >= pos_x2 && a.second->from_stage->pos_y <= pos_y1 && a.second->from_stage->pos_y >= pos_y2) {
            res = a.second;
            task_map[a.first] = nullptr; // 因为不支持删除，所以只是置0？
            break;
        }
    }
    return res;
}

//均衡获取任务，7大于4、5、6大于1、2、3（主要调度逻辑函数）
Task *Producer::get_task() {
    fprintf(stderr, "stage_id_ans:%d %d %d %d %d %d %d %d %d %d\n",
            stage_id_ans[0], stage_id_ans[1], stage_id_ans[2], stage_id_ans[3], stage_id_ans[4],
            stage_id_ans[5], stage_id_ans[6], stage_id_ans[7], stage_id_ans[8], stage_id_ans[9]);
    fprintf(stderr, "stage_id_ans:%d %d %d %d %d %d %d %d %d %d\n\n",
            count_task_ans[0], count_task_ans[1], count_task_ans[2], count_task_ans[3], count_task_ans[4],
            count_task_ans[5], count_task_ans[6], count_task_ans[7], count_task_ans[8], count_task_ans[9]);
    Task *tmp = nullptr;
    if (stage_id_ans[7] != 0) {
        tmp = find_stage_id(task_map, 7);
        stage_id_ans[7]--;
        count_task_ans[7]++;
    } else if (stage_id_ans[4] != 0 || stage_id_ans[5] != 0 || stage_id_ans[6] != 0) {
        std::vector<std::vector<int>> tmp_ans = {{count_task_ans[4], 4},
                                                 {count_task_ans[5], 5},
                                                 {count_task_ans[6], 6}};
        sort(tmp_ans.begin(), tmp_ans.end());
        for (auto &a: tmp_ans) {
            if (stage_id_ans[a[1]] != 0) {
                tmp = find_stage_id(task_map, a[1]);
                stage_id_ans[a[1]]--;
                count_task_ans[a[1]]++;
                break;
            }
        }
    } else if (stage_id_ans[1] != 0 || stage_id_ans[2] != 0 || stage_id_ans[3] != 0) {
        std::vector<std::vector<int>> tmp_ans = {{count_task_ans[1], 1},
                                                 {count_task_ans[2], 2},
                                                 {count_task_ans[3], 3}};
        sort(tmp_ans.begin(), tmp_ans.end());
        for (auto &a: tmp_ans) {
            if (stage_id_ans[a[1]] != 0) {
                tmp = find_stage_id(task_map, a[1]);//查询其实就是找到一个创建好的任务，等同于在此处创建任务
                stage_id_ans[a[1]]--;
                count_task_ans[a[1]]++;
                break;
            }
        }
    }

    return tmp;
}

Task *Producer::get_task(int robot_id) {
    fprintf(stderr, "stage_id_ans:%d %d %d %d %d %d %d %d %d %d\n",
            stage_id_ans[0], stage_id_ans[1], stage_id_ans[2], stage_id_ans[3], stage_id_ans[4],
            stage_id_ans[5], stage_id_ans[6], stage_id_ans[7], stage_id_ans[8], stage_id_ans[9]);
    fprintf(stderr, "stage_id_ans:%d %d %d %d %d %d %d %d %d %d\n\n",
            count_task_ans[0], count_task_ans[1], count_task_ans[2], count_task_ans[3], count_task_ans[4],
            count_task_ans[5], count_task_ans[6], count_task_ans[7], count_task_ans[8], count_task_ans[9]);
    Task *tmp = nullptr;
    if (stage_id_ans[7] != 0) {
        tmp = find_stage_id(task_map, 7);
        stage_id_ans[7]--;
        count_task_ans[7]++;
    } else if (stage_id_ans[4] != 0 || stage_id_ans[5] != 0 || stage_id_ans[6] != 0) {
              if(stage_id_ans[5] != 0 && robot_id < 2) {
                  switch (robot_id) {
                      case 0:
                          tmp = find_stage_id3(task_map, 5, 30, 20, 50, 35);
                          if (tmp != nullptr) {
                              stage_id_ans[5]--;
                              count_task_ans[5]++;
                          }
                          break;
                      case 1:
                          tmp = find_stage_id3(task_map, 5, 30, 20, 50, 35);
                          if (tmp != nullptr) {
                              stage_id_ans[5]--;
                              count_task_ans[5]++;
                          }
                          break;
                  }
              }
                  if(stage_id_ans[6] != 0 && robot_id>=2) {
                      switch (robot_id) {
                          case 2:
                              tmp = find_stage_id3(task_map, 6, 50, 41, 33, 18);
                              if (tmp != nullptr) {
                                  stage_id_ans[6]--;
                                  count_task_ans[6]++;
                              }
                              break;
                          case 3:
                              tmp = find_stage_id3(task_map, 6, 50, 41, 33, 18);
                              if (tmp != nullptr) {
                                  stage_id_ans[6]--;
                                  count_task_ans[6]++;
                              }
                              break;
                      }
                  }

            }
    if ((stage_id_ans[1] != 0 || stage_id_ans[2] != 0 || stage_id_ans[3] != 0) && tmp == nullptr) {
                switch(robot_id){
                    case 0:
                        if(count[0]%2) tmp = find_stage_id3(task_map,1 ,30,20,50,35);
                        else tmp = find_stage_id3(task_map,3 ,30,20,50,35);
                        if(tmp == nullptr) tmp = find_stage_id3(task_map,1 ,30,20,50,35);
                        if(tmp == nullptr) tmp = find_stage_id3(task_map,3 ,30,20,50,35);
                                stage_id_ans[1]--;
                                count_task_ans[1]++;
                                count[0]++;
                                 break;
                    case 1:
                        if(count[1]%2) tmp = find_stage_id3(task_map,3 ,30,20,50,35);
                        else tmp = find_stage_id3(task_map,1 ,30,20,50,35);
                        if(tmp == nullptr) tmp = find_stage_id3(task_map,3 ,30,20,50,35);
                        if(tmp == nullptr) tmp = find_stage_id3(task_map,1 ,30,20,50,35);
                        stage_id_ans[3]--;
                            count_task_ans[3]++;
                            count[1]++;
                            break;
                    case 2:
                        if(count[2]%2) tmp = find_stage_id3(task_map,2 ,50,41,33,18);
                        else tmp = find_stage_id3(task_map,3 ,50,41,33,18);
                        if(tmp == nullptr) tmp = find_stage_id3(task_map,2 ,50,41,33,18);
                        if(tmp == nullptr) tmp = find_stage_id3(task_map,3 ,50,41,33,18);
                        stage_id_ans[2]--;
                        count_task_ans[2]++;
                        count[2]++;
                        break;
                    case 3:
                        if(count[3]%2) tmp = find_stage_id3(task_map,3 ,50,41,33,18);
                        else tmp = find_stage_id3(task_map,2 ,50,41,33,18);
                        if(tmp == nullptr) tmp = find_stage_id3(task_map,3 ,50,41,33,18);
                        if(tmp == nullptr) tmp = find_stage_id3(task_map,2 ,50,41,33,18);
                        stage_id_ans[3]--;
                        count_task_ans[3]++;
                        count[3]++;
                        break;
                }
            }
//        }
//    }
    std::cerr<<tmp<<std::endl;
    return tmp;
}

//判断任务是否为空
bool Producer::is_empty() {
    for (int i = 0; i < 10; i++) {
        if (stage_id_ans[i] != 0) return false;
    }
    return true;
}

int Producer::p_size() {
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += stage_id_ans[i];
    }
    return sum;
}
