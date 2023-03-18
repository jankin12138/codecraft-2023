#pragma once

#include "Stage.hpp"

class Task {
public:
    Task(Stage &from_stage, Stage &to_stage) : from_stage(from_stage), to_stage(to_stage) {}

    Stage &from_stage;
    Stage &to_stage;
};
