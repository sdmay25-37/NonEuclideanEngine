#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <App.hpp>


// void sprite_update(entt::registry &registry, std::vector<glm::vec4> &uv_ranges, unsigned int UV_VBO) {
//     auto view = registry.view<Sprite>();
//
//     uv_ranges.clear();
//     for(auto [entity, sprite]: view.each()) {
//         uv_ranges.emplace_back(
//             sprite.uv_min.x,
//             sprite.uv_min.y,
//             sprite.uv_max.x,
//             sprite.uv_max.y
//         );
//     }
//
//     glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
//     glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * uv_ranges.size(), uv_ranges.data());
// }

int main() {
    App app;
    app.run();

    return 0;
}