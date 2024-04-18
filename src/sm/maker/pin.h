//
// Created by angus.goucher on 4/17/2024.
//

#ifndef SHADERMAKER_PIN_H
#define SHADERMAKER_PIN_H

#include <imgui_node_editor.h>
#include <string>

namespace sm::maker {
    enum class PinType {
        FLOAT,
        INT,
        VEC2,
        VEC3,
        VEC4,
        MAT3,
        MAT4,
        SAMPLER2D
    };

    struct Pin {
        ax::NodeEditor::PinId id;
        ax::NodeEditor::PinKind kind;
        ax::NodeEditor::NodeId nodeId;

        std::string name;
        PinType type;

        Pin(int32_t id, const std::string &name, PinType type, ax::NodeEditor::NodeId nodeId,
            ax::NodeEditor::PinKind kind)
                : id(id), name(name), type(type), kind(kind), nodeId(nodeId) {}
    };
}

#endif //SHADERMAKER_PIN_H
