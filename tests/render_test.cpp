#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "App.hpp"
#include "DAG.hpp"
#include "SystemThreadPool.hpp"
#include "SystemSet.hpp"

int main() {
    using SystemType = std::function<void()>;
    using SystemId = DAG<SystemType>::NodeId;

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

    // DAG<SystemType> dag;
    //
    // SystemId s1 = dag.AddNode(std::move(f1));
    // SystemId s2 = dag.AddNode(std::move(f2));
    // SystemId s3 = dag.AddNode(std::move(f3));
    //
    // dag.AddEdge(s1, s3);

    SystemThreadPool thread_pool(2);

    SystemSet set = std::move(
        SystemSet(std::move(f1), std::move(f2))
        .Before(std::move(f3))
    );

    thread_pool.Execute(set.GetSystemGraph());

    // App app;
    // app.run();

    return 0;
}
