#pragma once

#include <imgui_node_editor.h>
#include <vector>
#include <string>

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
			Node* node;
			std::string name;
			PinType type;

			Pin(int32_t id, const std::string& name, PinType type) : id(id), name(name), type(type) {}
		};

		struct Node {
			ax::NodeEditor::NodeId id;
			std::string name;
			std::vector<Pin> inputs;
			std::vector<Pin> outputs;

			Node(int32_t id, const std::string& name) : id(id), name(name) {}
		};

		class Window {
		public:
			Window();
			~Window();

			void doGui();

		private:
			ax::NodeEditor::EditorContext* context;
			std::vector<Node> nodes;
		};
	}
}