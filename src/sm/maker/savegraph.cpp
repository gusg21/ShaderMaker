//
// Created by xavier.olmstead on 4/19/2024.
//

#include "savegraph.h"
#include <fstream>

namespace sm {
    namespace maker {
        bool savegraph::saveGraphToFile(std::string fileName, Window maker) {
            std::ofstream fileOutput;
            fileOutput.open(fileName);

            if (fileOutput.fail()) {
                return false;
            }

            auto nodes = maker.getNodes();

            for(size_t i = 0; i < nodes.size(); i++)
            {
                auto nodePos = ax::NodeEditor::GetNodePosition(nodes[i].id);

                fileOutput << nodePos.x << " " << nodePos.y << "\n";

                fileOutput << nodes[i].id.Get() << " " << nodes[i].name << " " << nodes[i].isDataHook << " "
                           << nodes[i].isInputOnly << " " << nodes[i].isOutputOnly << " " << nodes[i].spec << "\n";
            }

            fileOutput << "Links\n";

            for(Link link : maker.getLinks())
            {
                fileOutput << link.id.Get() << " " << link.inPin << " " << link.outPin << "\n";
            }

            return true;
        }

    } // sm
} // maker