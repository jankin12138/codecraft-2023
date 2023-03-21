#pragma once

class Stage;

class Task {
public:
    Task() : from_stage(nullptr), to_stage(nullptr) {}

    Task(Stage *from_stage, Stage *to_stage) : from_stage(from_stage), to_stage(to_stage) {}

    explicit Task(Stage *from_stage) : from_stage(from_stage), to_stage(nullptr) {}

    Stage *from_stage;
    Stage *to_stage;
};
