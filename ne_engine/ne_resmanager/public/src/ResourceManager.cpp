//
//  Author: tagrinnell
//  2/18/25
//

#include "ResourceManager.hpp"

void freeSheets(std::unordered_map <const char*, FileInfo*> &fileDirectory, const char* filepath);
ImageData* loadTexture(const char* filepath);

ResourceManager::ResourceManager () {
    _loader = new JSONLoader;
}

ResourceManager::ResourceManager(std::string bindingsFilePath) {
    _loader = new JSONLoader(bindingsFilePath);
}

std::vector <std::vector <std::pair <std::string, int>>> ResourceManager::getBindingsArray() {
    return _loader->processFileArray();
}

void ResourceManager::outputBindings(std::vector <std::vector <std::pair <std::string, int>>> bindings) {
    // Error check
    if (_loader->getFilepath() != "null") {
        _loader->outputBindingsArray(bindings);
    } else {
        std::cout << "Loader is hosting a null filepath." << std::endl;
    }
}

// Remove a reference to a File spritesheet
void ResourceManager::removeReference(const char* filepath) {
    if (_fileDirectory.find(filepath)->second->numberOfReferences == 0) {
        std::cerr << "Error: Number of References == 0, cannot decrease the number of references" << std::endl;
    }
    _fileDirectory.find(filepath)->second->numberOfReferences--;
}

// Retire a resource => Free the FileInfo struct that we previously malloc'd
void ResourceManager::retireResource(const char* filepath) {
    freeSheets(_fileDirectory, filepath);
}

// Returns spritesheet in an unsigned char array
unsigned char* ResourceManager::getResource(const char* filepath) {
    // Check existence in map
    auto check = _fileDirectory.find(filepath);
    if (check == _fileDirectory.end()) {
        // Try to open the file
        ImageData* texture = loadTexture(filepath);
        if (!texture) {
            std::cerr << "Failed to open file" << std::endl;
            return __null;
        }
        // Allocate space for the FileInfo struct to store the data
        FileInfo* newFile = new FileInfo;
        newFile->numberOfReferences = 1;
        newFile->spritesheetInfo = texture;
        
        // Put it into our directory
        _fileDirectory[filepath] = newFile;
        return texture->spritesheet;
    } else {
        // Exists => Increase the numberOfReferences and return
        check->second->numberOfReferences++;
        return check->second->spritesheetInfo->spritesheet;
    }
    // Fail
    return __null;
}

void ResourceManager::setLoaderPath(std::string filepath) {
    _loader->setFilepath(filepath);
}

void ResourceManager::retireManager() {
    for (auto keyFile : _fileDirectory) {
        freeSheets(_fileDirectory, keyFile.first);
    }
}

ImageData* loadTexture(const char* filepath) {
    // Load Spritesheet
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;  
    unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
        return NULL;
    }

    ImageData *returnData = new ImageData;
    returnData->width = width;
    returnData->height = height;
    returnData->channels = nrChannels;
    returnData->spritesheet = data;

    return returnData;
}

void freeSheets(std::unordered_map <const char*, FileInfo*> &fileDirectory, const char* filepath) {
    auto it = fileDirectory.find(filepath);
    delete it->second;
    fileDirectory.erase(filepath);
}
