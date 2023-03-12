#include <iostream>
#include <bits/stdc++.h>
#include "lib.h"

using namespace std;

int main() {
    // 初始化地图
    FILE *file;
    fopen_s(&file, "..\\1.txt", "r");
    Map my_map = init_map(file);
    fclose(file);

    fopen_s(&file, "..\\IO1.txt", "r");
    flush_map(file, &my_map);
    fclose(file);

}
