#include "RobotCrashPreventer.hpp"
#include "Robot.hpp"
#include "util.hpp"

using namespace std;

void RobotCrashPreventer::prevent_robot_crash() {
    bool isRobotStopped[n];
    for (int i = 0; i < n; ++i)
        isRobotStopped[i] = false;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            // 当两机器人可能相撞时，停下序号小的那个
            if (will_crash(robots[i], robots[j])) {
                isRobotStopped[i] = true;
                break;
            }
        }
    }
    for (int i = 0; i < n - 1; ++i) {
        if (isRobotStopped[i]) {
            robots[i].print_forward(0);
            robots[i].print_rotate(0);
        }
    }
}

bool RobotCrashPreventer::will_crash(Robot &robot1, Robot &robot2) {
    return distance(robot1.pos_x, robot1.pos_y, robot2.pos_x, robot2.pos_y) < v_max * seconds_per_frame * 3 + 0.53;
}