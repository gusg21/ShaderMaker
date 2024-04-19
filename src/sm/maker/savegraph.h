//
// Created by xavier.olmstead on 4/19/2024.
//
#include <string>
#include "window.h"

#ifndef SHADERMAKER_SAVEGRAPH_H
#define SHADERMAKER_SAVEGRAPH_H

    namespace sm::maker
    {
        class savegraph {
        public:
            bool saveGraphToFile(std::string fileName, Window maker);
        };

    }

#endif //SHADERMAKER_SAVEGRAPH_H
