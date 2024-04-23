#pragma once

#include <imgui_node_editor.h>
#include <vector>
#include <string>
#include "pin.h"
#include "../texture.h"
#include "nodespec.h"
#include "node.h"
#include "link.h"

namespace sm {
    namespace maker {
        class Window {
        public:
            Window(const std::vector<PinSpec> &shaderInputs, const std::vector<PinSpec> &shaderOutputs);

            ~Window();

            void doGui();

            void createNodeFromSpec(const NodeSpec &spec);

            void createNodeFromSpecAt(const NodeSpec &spec, ImVec2 position);

            void createNodeFromSpecAtWithId(const NodeSpec &spec, ImVec2 position, uint32_t nodeId);

            bool createLink(const ax::NodeEditor::PinId startPinId, const ax::NodeEditor::PinId endPinId, const bool needsAcceptance = true);

            void getPinTypeTexCoords(PinType type, ImVec2 *uv0, ImVec2 *uv1);

            ImVec4 getPinTypeColor(PinType type);

            bool canCastFrom(PinType from, PinType to);

            ax::NodeEditor::NodeId Window::getOutputNodeId() const;

            const Pin *findPinById(ax::NodeEditor::PinId id) const;

            const Node *findNodeById(ax::NodeEditor::NodeId id) const;

            const Link *findLinkEndingAtId(ax::NodeEditor::PinId id) const;

            std::string composeCodeForNodeId(const ax::NodeEditor::NodeId node) const;

            const std::vector<Node> getNodes();
            const std::vector<NodeSpec> getNodeSpecs();
            const std::vector<Link> getLinks();

        private:
            uint32_t nextId = 1;

            ax::NodeEditor::EditorContext *context;
            sm::Texture *iconsTexture;

            std::vector<NodeSpec> nodeSpecs;
            std::vector<Node> nodes;
            std::vector<Link> links;


        };
    }
}