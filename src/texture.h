//
// Created by angus.goucher on 4/5/2024.
//

#ifndef SHADERMAKER_TEXTURE_H
#define SHADERMAKER_TEXTURE_H

#include <string>

namespace sm {
    class Texture {
    public:
        Texture(const std::string& path);
        Texture(const std::string& path, int wrapMode, int magFilter, int minFilter, bool mipmap);

        unsigned int getId() { return id; };

    private:
        unsigned int loadTexture(const std::string &path, int wrapMode, int magFilter, int minFilter, bool mipmap);

        unsigned int id;
    };
}

#endif //SHADERMAKER_TEXTURE_H
