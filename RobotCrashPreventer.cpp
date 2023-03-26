#include "RobotCrashPreventer.hpp"
#include "Robot.hpp"
#include "util.hpp"

using namespace std;

double mod_pi(double r) {
    while (r < 0)
        r += 2 * pi;
    return r > pi/2 ? r - 2 * pi : r;
}
void RobotCrashPreventer::prevent_robot_crash() {
    double extra_rotates[n];
    for (int i = 0; i < n; ++i)
        extra_rotates[i] = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dist = distance(robots[i].pos_x, robots[i].pos_y, robots[j].pos_x, robots[j].pos_y);
            // 当两机器人可能相撞时，需要根据机器人位置设置一个偏移转向角
            if (will_crash(robots[i], robots[j])) {
                double rad = calc_rad(robots[i].pos_x - robots[j].pos_x, robots[i].pos_y - robots[j].pos_y);
//                extra_rotates[i] += (rad - robots[i].pos_rad) * 0.05;
//                extra_rotates[j] += (rad + pi - robots[j].pos_rad) * 0.05;
                extra_rotates[i] += mod_pi(rad - robots[i].pos_rad) * 0.3;
                extra_rotates[j] += mod_pi(rad + pi - robots[j].pos_rad) * 0.3;
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        if (extra_rotates[i] != 0) {
//            cerr << i << " extra rotate " << robots[i].next_rotate + extra_rotates[i] << endl;
            double final_rotate = robots[i].next_rotate + extra_rotates[i];
            robots[i].print_rotate(robots[i].next_rotate + extra_rotates[i]);
        }
    }
}

bool RobotCrashPreventer::will_crash(Robot &robot1, Robot &robot2) {
    if (distance(robot1.pos_x, robot1.pos_y, robot2.pos_x, robot2.pos_y) > 1 + 0.53 * 2)
        return false;
    return true;
}
