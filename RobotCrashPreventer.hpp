#pragma once

class Robot;

class RobotCrashPreventer {
public:
    RobotCrashPreventer(Robot*robots, int n):robots(robots), n(n){}
    void prevent_robot_crash();
    static bool will_crash(Robot& robot1, Robot& robot2);
    Robot*robots;
    int n;
};
