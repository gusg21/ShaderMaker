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
        Texture(const std::string& path, int32_t wrapMode, int32_t magFilter, int32_t minFilter, bool mipmap);

        uint32_t getId() { return id; };
        void bindToTextureUnit(uint32_t unit);

    private:
        uint32_t loadTexture(const std::string &path, int32_t wrapMode, int32_t magFilter, int32_t minFilter, bool mipmap);

        uint32_t id;
    };
}

#endif //SHADERMAKER_TEXTURE_H


