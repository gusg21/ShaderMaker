#pragma once

#include <imgui_node_editor.h>
#include <vector>
#include <string>
#include "texture.h"

namespace sm {
	namespace maker {
		struct Node;

		enum class PinType {
			FLOAT,
			INT,
			VEC2,
			VEC3,
			VEC4
		};

		struct Pin {
			ax::NodeEditor::PinId id;
			ax::NodeEditor::PinKind kind;

			Node* node;
			std::string name;
			PinType type;

			Pin(int32_t id, const std::string& name, PinType type) : id(id), name(name), type(type), kind(ax::NodeEditor::PinKind::Input), node(nullptr) {}
		};

		struct Node {
			ax::NodeEditor::NodeId id;

			std::string name;
			std::vector<Pin> inputs;
			std::vector<Pin> outputs;

			Node(int32_t id, const std::string& name) : id(id), name(name) {}
		};

		struct Link {
			ax::NodeEditor::LinkId id;

			ax::NodeEditor::PinId startPinId;
			ax::NodeEditor::PinId endPinId;

			Link(int32_t id, ax::NodeEditor::PinId start, ax::NodeEditor::PinId end) : id(id), startPinId(start), endPinId(end) {}
		};

		class Window {
		public:
			Window();
			~Window();

			void buildNode(Node* node);
			void doGui();
			void getPinTypeTexCoords(PinType type, ImVec2* uv0, ImVec2* uv1);

		private:
			uint32_t nextId;

			ax::NodeEditor::EditorContext* context;
			sm::Texture* iconsTexture;

			std::vector<Node> nodes;
			std::vector<Link> links;
		};
	}
}