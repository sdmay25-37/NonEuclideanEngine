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

                auto texture_result = Texture::create(filepath);

                if(texture_result.is_ok()) {
                    _fileDirectory.insert({filename, std::make_shared<Texture>(texture_result.ok())});
                } else {
                    std::cerr << "Failed to load image file: " << filepath << std::endl;
                }
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