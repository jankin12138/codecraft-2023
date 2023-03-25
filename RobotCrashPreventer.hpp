#pragma once

class Robot;

class RobotCrashPreventer {
public:
    void prevent_robot_crash(Robot* robots, int n);
    bool will_crash(Robot& robot1, Robot& robot2);
};
