//
// Created by angus.goucher on 4/17/2024.
//

#ifndef SHADERMAKER_NODE_H
#define SHADERMAKER_NODE_H

#include <string>
#include <vector>
#include "pin.h"
#include "nodespec.h"

namespace sm::maker {
    struct Node {
        ax::NodeEditor::NodeId id;

        bool isDataHook;
        bool isInputOnly;
        bool isOutputOnly;
        char data[256] = {0};

        std::string name;
        std::vector<Pin> inputs;
        std::vector<Pin> outputs;

        const NodeSpec *spec;

        Node(int32_t id, const std::string &name, bool isDataHook, bool isInputOnly, bool isOutputOnly,
             const NodeSpec *spec)
                : id(id), name(name), isDataHook(isDataHook), isInputOnly(isInputOnly), isOutputOnly(isOutputOnly),
                  spec(spec) {}
    };
}

#endif //SHADERMAKER_NODE_H
