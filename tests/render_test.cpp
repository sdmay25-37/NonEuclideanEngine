#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "App.hpp"

#include "ne_system/SystemSet.hpp"
#include "ne_system/SystemSchedule.hpp"
#include "ne_util/DirectedGraph.hpp"


int main() {
    using SystemType = std::function<void()>;
    using SystemId = DirectedGraph<SystemType>::NodeId;

    SystemType f1 = [] {
        std::cout << "System 1: start" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "System 1: end" << std::endl;
    };

    SystemType f2 = [] {
        std::cout << "System 2: start" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(4));
        std::cout << "System 2: end" << std::endl;
    };

    SystemType f3 = [] {
        std::cout << "System 3: start" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "System 3: end" << std::endl;
    };

    // SystemSet set = std::move(
    //     SystemSet(std::move(f1), std::move(f2))
    //     .Before(std::move(f3))
    // );



    DirectedGraph<SystemType> dag;
    SystemId id1 = dag.AddNode(std::move(f1));
    SystemId id2 = dag.AddNode(std::move(f2));
    SystemId id3 = dag.AddNode(std::move(f3));
    dag.AddEdge(id1, id3);

    SystemSchedule schedule(std::move(dag));
    schedule.Execute();

    // App app;
    // app.run();

    return 0;
}
