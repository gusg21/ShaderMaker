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

			Pin(int32_t id, const std::string& name, PinType type)
				: id(id), name(name), type(type), kind(ax::NodeEditor::PinKind::Input), node(nullptr) {}
		};

		struct Node {
			ax::NodeEditor::NodeId id;
			bool isConstant;

			std::string name;
			std::vector<Pin> inputs;
			std::vector<Pin> outputs;

			Node(int32_t id, const std::string& name, bool isConstant) : id(id), name(name), isConstant(isConstant) {}
		};

		struct Link {
			ax::NodeEditor::LinkId id;

			ax::NodeEditor::PinId startPinId;
			ax::NodeEditor::PinId endPinId;

			Link(int32_t id, ax::NodeEditor::PinId start, ax::NodeEditor::PinId end) : id(id), startPinId(start), endPinId(end) {}
		};

		struct PinSpec {
			std::string name;
			PinType type;

			PinSpec(const std::string& name, PinType type) : name(name), type(type) {}
		};

		struct NodeSpec {
			std::string name;
			bool isConstant;

			std::vector<PinSpec> inputs;
			std::vector<PinSpec> outputs;

			NodeSpec(const std::string& name, std::vector<PinSpec> inputs, std::vector<PinSpec> outputs, bool isConstant = false)
				: name(name), inputs(inputs), outputs(outputs), isConstant(isConstant) {}
		};

		class Window {
		public:
			Window();
			~Window();

			void doGui();
			void doNodeMenuGui();

			void createNodeFromSpec(const NodeSpec& spec);
			void createNodeFromSpecAt(const NodeSpec& spec, ImVec2 position);
			void buildNode(Node* node);
			void getPinTypeTexCoords(PinType type, ImVec2* uv0, ImVec2* uv1);
			bool canCastFrom(PinType from, PinType to);
			const Pin* findPinById(ax::NodeEditor::PinId id) const;

		private:
			uint32_t nextId = 0;

			ax::NodeEditor::EditorContext* context;
			sm::Texture* iconsTexture;

			std::vector<NodeSpec> nodeSpecs;
			std::vector<Node> nodes;
			std::vector<Link> links;
		};
	}
}