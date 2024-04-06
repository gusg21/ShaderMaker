//
// Created by angus.goucher on 4/5/2024.
//

#include "texture.h"
#include "../external/stb_image.h"
#include "../external/glad.h"

#include <string>

static int getTextureFormat(int32_t numComponents) {
    switch (numComponents) {
        default:
            return GL_RGBA;
        case 3:
            return GL_RGB;
        case 2:
            return GL_RG;
        case 1:
            return GL_RED;
    }
}

namespace sm {
    uint32_t Texture::loadTexture(const std::string &path, int32_t wrapMode, int32_t magFilter, int32_t minFilter, bool mipmap) {
        int32_t width, height, numComponents;
        uint8_t* data = stbi_load(path.c_str(), &width, &height, &numComponents, 0);
        if (data == NULL) {
            printf("Failed to load image %s", path.c_str());
            stbi_image_free(data);
            return 0;
        }

        uint32_t texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        int format = getTextureFormat(numComponents);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

        //Black border by default
        float borderColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        if (mipmap) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
        return texture;
    }

    Texture::Texture(const std::string &path, int32_t wrapMode, int32_t magFilter, int32_t minFilter, bool mipmap) {
        id = loadTexture(path, wrapMode, magFilter, minFilter, mipmap);
    }

    void Texture::bindToTextureUnit(uint32_t unit)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, getId());
        glBindTextureUnit(unit, getId());
    }

    Texture::Texture(const std::string &path) {
        id = loadTexture(path, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);
    }
}