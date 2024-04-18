//
// Created by angus.goucher on 4/17/2024.
//

#ifndef SHADERMAKER_LINK_H
#define SHADERMAKER_LINK_H

#include <cstdint>
#include "pin.h"

namespace sm::maker {
    struct Link {
        ax::NodeEditor::LinkId id;

        const Pin *inPin;
        const Pin *outPin;

        Link(int32_t id, const Pin *inPin, const Pin *outPin)
                : id(id), inPin(inPin), outPin(outPin) {}
    };
}

#endif //SHADERMAKER_LINK_H
