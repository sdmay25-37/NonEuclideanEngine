//
//  Author: tagrinnell
//  2/18/25
//

#include "ResourceManager.hpp"

void freeSheets(std::unordered_map <const char*, FileInfo> fileDirectory, const char* filepath);
unsigned char* loadTexture(const char* filepath);

ResourceManager::ResourceManager () {
    _loader;
}

ResourceManager::ResourceManager(std::string bindingsFilePath) {
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

// Remove a reference to a File spritesheet
void ResourceManager::removeReference(const char* filepath) {
    _fileDirectory.find(filepath)->second.numberOfReferences--;
}

// Retire a resource => Free the FileInfo struct that we previously malloc'd
void ResourceManager::retireResource(const char* filepath) {
    freeSheets(_fileDirectory, filepath);
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
            return __null;
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
    return __null;
}

void ResourceManager::retireManager() {
    for (auto keyFile : _fileDirectory) {
        freeSheets(_fileDirectory, keyFile.first);
    }
}

unsigned char* loadTexture(const char* filepath) {
    // Load Spritesheet
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;  
    unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
        return __null;
    }
    return data;
}

void freeSheets(std::unordered_map <const char*, FileInfo> fileDirectory, const char* filepath) {
    free(&fileDirectory.find(filepath)->second);
}
