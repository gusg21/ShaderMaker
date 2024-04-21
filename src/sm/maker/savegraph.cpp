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
                Node* node = &nodes[i];
                auto nodePos = ax::NodeEditor::GetNodePosition(node->id);

                fileOutput << nodePos.x << " " << nodePos.y << "\n";

                fileOutput << node->spec << "\n";
            }

            fileOutput << "Links\n";

            for(Link link : maker.getLinks())
            {
                fileOutput << link.id.Get() << " " << link.inPin << " " << link.outPin << "\n";
            }

            fileOutput.close();

            return true;
        }

    } // sm
} // maker