//
// Created by angus.goucher on 4/17/2024.
//

#ifndef SHADERMAKER_NODESPEC_H
#define SHADERMAKER_NODESPEC_H

#include <string>
#include <vector>
#include "window.h"
#include "pinspec.h"

namespace sm::maker {
    struct NodeSpec {
        std::string name;
        std::string funcName;
        bool isDataHook;
        bool isInputOnly;
        bool isOutputOnly;
        bool isHidden;

        std::vector<PinSpec> inputs;
        std::vector<PinSpec> outputs;

        NodeSpec(const std::string &name, const std::string &funcName, std::vector<PinSpec> inputs,
                 std::vector<PinSpec> outputs, bool isDataHook = false, bool isInputOnly = false, bool isOutputOnly = false, bool isHidden = false)
                : name(name), funcName(funcName), inputs(inputs), outputs(outputs), isDataHook(isDataHook),
                  isInputOnly(isInputOnly), isOutputOnly(isOutputOnly), isHidden(isHidden) {}
    };
}

#endif //SHADERMAKER_NODESPEC_H
