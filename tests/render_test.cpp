#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "App.hpp"

 // -- Component -- //
struct Person {
    std::string name;
};

// -- Resource -- //
struct Greeting {
    std::string str;
    explicit Greeting(std::string str) : str(std::move(str)) {}
};

// -- Systems --//
void CreatePeople(entt::registry& registry) {
    const auto entity = registry.create();
    registry.emplace<Person>(entity, "John Doe");

    const auto entity2 = registry.create();
    registry.emplace<Person>(entity2, "Jane Doe");
}

void GreetPeople(entt::registry& registry, Resource<Greeting> greeting) {
    for(auto [entity, person] : registry.view<const Person>().each()) {
        std::cout << greeting->str << " " << person.name << std::endl;
    }
}

int main() {

    App()
        .AddSystems(ScheduleLabel::STARTUP, SystemSet(CreatePeople))
        .AddSystems(ScheduleLabel::UPDATE, SystemSet(GreetPeople))
        .InsertResource<Greeting>("Howdy")
        .run();

    return 0;
}
