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

			Pin(int32_t id, const std::string& name, PinType type, Node* node, ax::NodeEditor::PinKind kind)
				: id(id), name(name), type(type), kind(kind), node(node) {}
		};

		struct NodeSpec;

		struct Node {
			ax::NodeEditor::NodeId id;

			bool isDataHook;
			char data[256] = { 0 };

			std::string name;
			std::vector<Pin> inputs;
			std::vector<Pin> outputs;

			const NodeSpec* spec;

			Node(int32_t id, const std::string& name, bool isConstant, const NodeSpec* spec)
				: id(id), name(name), isDataHook(isConstant), spec(spec) {
			}
		};

		struct Link {
			ax::NodeEditor::LinkId id;

			ax::NodeEditor::PinId inPinId;
			ax::NodeEditor::PinId outPinId;
			
			const Pin* inPin;
			const Pin* outPin;

			Link(int32_t id, const Pin* inPin, const Pin* outPin) 
				: id(id), inPin(inPin), outPin(outPin), inPinId(inPin->id), outPinId(outPin->id) {}
		};

		struct PinSpec {
			std::string name;
			PinType type;

			PinSpec(const std::string& name, PinType type) : name(name), type(type) {}
		};

		struct NodeSpec {
			std::string name;
			std::string funcName;
			bool isConstant;

			std::vector<PinSpec> inputs;
			std::vector<PinSpec> outputs;

			NodeSpec(const std::string& name, const std::string& funcName, std::vector<PinSpec> inputs, std::vector<PinSpec> outputs, bool isConstant = false)
				: name(name), funcName(funcName), inputs(inputs), outputs(outputs), isConstant(isConstant) {}
		};

		class Window {
		public:
			Window();
			~Window();

			void doGui();

			void createNodeFromSpec(const NodeSpec& spec);
			void createNodeFromSpecAt(const NodeSpec& spec, ImVec2 position);
			void getPinTypeTexCoords(PinType type, ImVec2* uv0, ImVec2* uv1);
			ImVec4 getPinTypeColor(PinType type);
			bool canCastFrom(PinType from, PinType to);
			const Pin* findPinById(ax::NodeEditor::PinId id) const;
			const Node* findNodeById(ax::NodeEditor::NodeId id) const;
			const Link* findLinkEndingAtId(ax::NodeEditor::PinId id) const;

			std::string composeCodeForNode(const Node* node);

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