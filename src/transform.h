//
// Created by angus.goucher on 4/5/2024.
//

#ifndef SHADERMAKER_TRANSFORM_H
#define SHADERMAKER_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace sm {
    struct Transform {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::mat4 modelMatrix() const {
            glm::mat4 m = glm::mat4(1.0f);
            m = glm::scale(m, scale);
            m *= glm::mat4_cast(rotation);
            m = glm::translate(m, position);
            return m;
        }
    };
}


#endif //SHADERMAKER_TRANSFORM_H
