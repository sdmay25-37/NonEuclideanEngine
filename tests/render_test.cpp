#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "App.hpp"

#include "ne_system/SystemSet.hpp"
#include "ne_system/SystemSchedule.hpp"
#include "ne_system/SystemExecutor.hpp"
#include "ne_util/DirectedGraph.hpp"


void system1() {
    std::cout << "System 1: start" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "System 1: end" << std::endl;
}

void system2() {
    std::cout << "System 2: start" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "System 2: end" << std::endl;
}

void system3() {
    std::cout << "System 3: start" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "System 3: end" << std::endl;
}

int main() {
    SystemSchedule schedule(std::move(
        SystemSet(system1, system2)
        .Before(system3))
    );

    auto executor = SystemExecutor::Create(SystemExecutor::Type::SingleThreaded);
    executor->Execute(schedule);

    // App app;
    // app.run();

    return 0;
}
