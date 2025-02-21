//
//  Author: tagrinnell
//  2/18/25
//

#include "ResourceManager.hpp"

ResourceManager::ResourceManager () {
    _loader;
}

ResourceManager::ResourceManager (std::string bindingsFilePath) {
    JSONLoader _loader(bindingsFilePath);
}

void ResourceManager::outputBindings(std::vector <std::vector <std::pair <std::string, int>>> bindings) {
    // Error check
    if (_loader.getFilepath() != "null") {
        _loader.outputBindingsArray(bindings);
    } else {
        std::cout << "Loader is hosting a null filepath." << std::endl;
    }
}

void ResourceManager::initResourceManager() {
}

// Returns a FileDescriptor from a string.  If the resource hasn't been opened, then it needs to open the file
unsigned char* ResourceManager::getResource(const char* filepath) {
    // Check existence in map
    auto check = _fileDirectory.find(filepath);
    if (check == _fileDirectory.end()) {
        // Try to open the file
        unsigned char* texture = loadTexture(filepath);
        if (!texture) {
            std::cerr << "Failed to open file" << std::endl;
            return;
        }
        // Allocate space for the FileInfo struct to store the data
        FileInfo* newFile = (FileInfo*) malloc(sizeof(int) + sizeof(texture));
        newFile->numberOfReferences = 1;
        newFile->spritesheet = texture;
        
        // Put it into our directory
        _fileDirectory[filepath] = *newFile;
        return texture;
    } else {
        // Exists => Increase the numberOfReferences and return
        check->second.numberOfReferences++;
        return check->second.spritesheet;
    }
    // Fail
    return;
}

unsigned char* loadTexture(const char* filepath) {
    // Load Spritesheet
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
        return;
    }
    // if (data) {
    //     GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
    //     glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // } else {
    //     std::cerr << "Failed to load texture" << std::endl;
    // }
    // stbi_image_free(data);
    return data;
}