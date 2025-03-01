//
//  Author: tagrinnell
//  2/18/25
//

#include "ResourceManager.hpp"


ResourceManager::ResourceManager(const char *resourceFilepath) {

    try {
        for(const auto &entry : std::filesystem::directory_iterator(resourceFilepath)) {
            if(is_regular_file(entry.status())) {
                const char *filepath = entry.path().c_str();
                std::string filename = entry.path().filename().string();

                int width, height, channels;

                unsigned char *image_data = stbi_load(filepath, &width, &height, &channels, 0);
                if (!image_data) {
                    std::cerr << "Failed to load texture: " << filepath << std::endl;
                    return;
                }

                _fileDirectory.emplace(filename, std::make_shared<Texture>(width, height, channels, image_data));
            }
        }
    } catch(const std::exception &e) {
        std::cerr << "Failed to load file: " << e.what() << std::endl;
    }


}


std::weak_ptr<Texture> ResourceManager::getResource(const char *filepath) {

    auto it = _fileDirectory.find(filepath);
    if(it != _fileDirectory.end()) {
        return it->second;
    }

    // Return empty pointer if the file doesn't exist
    return {};
}