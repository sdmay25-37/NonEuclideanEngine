#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "App.hpp"
#include "ne_system/SystemSet.hpp"

struct Person {
    std::string name;
};

void CreatePeople(entt::registry& registry) {
    const auto entity = registry.create();
    registry.emplace<Person>(entity, "John Doe");

    const auto entity2 = registry.create();
    registry.emplace<Person>(entity2, "Jane Doe");
}

void GreetPeople(entt::registry& registry) {
    for(auto [entity, person] : registry.view<const Person>().each()) {
        std::cout << "Name: " << person.name << std::endl;
    }
}

int main() {

    entt::registry registry;

    App()
        .AddSystems(ScheduleLabel::STARTUP, SystemSet(CreatePeople))
        .AddSystems(ScheduleLabel::UPDATE, SystemSet(GreetPeople))
        .run();

    return 0;
}
