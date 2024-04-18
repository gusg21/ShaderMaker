//
// Created by angus.goucher on 4/17/2024.
//

#ifndef SHADERMAKER_PINSPEC_H
#define SHADERMAKER_PINSPEC_H

#include "pin.h"

namespace sm::maker {
    struct PinSpec {
        std::string name;
        PinType type;

        PinSpec(const std::string &name, PinType type) : name(name), type(type) {}
    };
}

#endif //SHADERMAKER_PINSPEC_H
