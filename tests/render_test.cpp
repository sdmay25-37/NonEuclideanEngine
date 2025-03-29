#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "App.hpp"

#include "ne_system/SystemSet.hpp"
#include "ne_system/SystemSchedule.hpp"
#include "ne_system/SystemExecutor.hpp"
#include "ne_util/DirectedGraph.hpp"

struct Person {
    std::string name;
};

void system1(entt::registry& registry) {
    const auto entity = registry.create();
    registry.emplace<Person>(entity, "John Doe");
}

void system2(entt::registry& registry) {
    const auto entity = registry.create();
    registry.emplace<Person>(entity, "Jane Doe");
}

void system3(entt::registry& registry) {
    for(auto [entity, person] : registry.view<const Person>().each()) {
        std::cout << "Name: " << person.name << std::endl;
    }
}

int main() {

    entt::registry registry;

    SystemSchedule schedule(std::move(
        SystemSet(system1, system2)
        .Before(system3)
    ));

    auto executor = SystemExecutor::Create(SystemExecutor::Type::SingleThreaded, registry);
    executor->Execute(schedule);

    // App app;
    // app.run();

    return 0;
}
