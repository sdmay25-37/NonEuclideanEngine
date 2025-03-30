#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "App.hpp"
#include "ne_system/SystemSet.hpp"
#include "ne_system/Resource.hpp"
#include "ne_system/System.hpp"

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

struct Sound {
    // Testing resource class
    float volume;
    explicit Sound(float volume) : volume(volume) {}
};

void sound_system(entt::registry& registry, Resource<Sound> sound) {

    std::cout << "Volume: " << sound->volume << std::endl;
}

void test(SystemBase&& system) {

}

int main() {

    ResourceManager resourceManager;
    resourceManager.insert<Sound>(10.0f);


    App()
        .AddSystems(ScheduleLabel::STARTUP, SystemSet(CreatePeople))
        .AddSystems(ScheduleLabel::UPDATE, SystemSet(GreetPeople))
        .run();

    return 0;
}
