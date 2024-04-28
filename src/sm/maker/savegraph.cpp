//
// Created by xavier.olmstead on 4/19/2024.
//

#include "savegraph.h"
#include <fstream>

namespace sm {
    namespace maker {
        bool savegraph::saveGraphToFile(const std::string& fileName, Window& maker) {
            std::ofstream fileOutput;
            fileOutput.open(fileName);

            if (fileOutput.fail()) {
                return false;
            }

            auto nodes = maker.getNodes();

            fileOutput << nodes.size() << "\n";

            for(size_t i = 0; i < nodes.size(); i++)
            {
                Node* node = &nodes[i];
                auto nodePos = ax::NodeEditor::GetNodePosition(node->id);

                fileOutput << nodePos.x << " " << nodePos.y << "\n";

                auto nodeSpecs = maker.getNodeSpecs();

                auto val = std::find(nodeSpecs.begin(), nodeSpecs.end(), *node->spec);

                fileOutput << (val != nodeSpecs.end() ? val - nodeSpecs.begin() : -1) << "\n"; // -1 means not found
                fileOutput << node->id.Get() << "\n";

                for(char c : node->data)
                {
                    fileOutput << c;
                }

                fileOutput << "\n";
                //fileOutput << node->data << "\n";
            }

            fileOutput << maker.getLinks().size() << "\n";

            for(Link link : maker.getLinks())
            {
                //TODO: ensure links aren't pointing to potentially wrong id on initial creation
                fileOutput << link.inPin->id.Get() << " " << link.outPin->id.Get() << "\n";
            }

            fileOutput.close();

            return true;
        }

        bool savegraph::loadFileToGraph(const std::string &fileName, Window &maker) {
            std::ifstream fileInput;
            fileInput.open(fileName);

            if (fileInput.fail()) {
                return false;
            }

            int numNodes;
            fileInput >> numNodes;

            for(int i = 0; i < numNodes; i++)
            {
                float x, y;
                int32_t specIndex;
                int32_t nodeId;
                char nodeData[256];
                fileInput >> x >> y >> specIndex >> nodeId;

                Node* node;

                if(specIndex != -1)
                    node = maker.createNodeFromSpecAtWithId(maker.getNodeSpecs()[specIndex], ImVec2(x, y), nodeId);

                fileInput.getline(nodeData, 256, '\n');
                fileInput.getline(nodeData, 256, '\n');

                if(node != nullptr)
                    std::strcpy(node->data, nodeData); //TODO: remove this()
                //fileInput.getline(nodeData, 256, '\n');
                //maker.findNodeById(nodeId)->data = nodeData;
            }

            int numLinks;
            fileInput >> numLinks;

            for(int i = 0; i < numLinks; i++)
            {
                int32_t inPinId, outPinId;
                fileInput >> inPinId >> outPinId;

                maker.createLink(inPinId, outPinId, false);
            }

            return true;
        }

    } // sm
} // maker