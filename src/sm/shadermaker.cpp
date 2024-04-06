#include "shadermaker.h"

#include <imgui_node_editor.h>

using namespace sm::maker;

Window::Window()
{
	ax::NodeEditor::Config nodesConfig;
	context = ax::NodeEditor::CreateEditor(&nodesConfig);
}

sm::maker::Window::~Window()
{
	ax::NodeEditor::DestroyEditor(context);
}

void Window::doGui()
{
	ax::NodeEditor::SetCurrentEditor(this->context);
	ax::NodeEditor::Begin("Nodes");
	{
		int uniqueId = 1;
		ax::NodeEditor::BeginNode(uniqueId++);
		{
			ImGui::Text("Node A");

			ax::NodeEditor::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Input);
			{
				ImGui::Text("In");
			}
			ax::NodeEditor::EndPin();

			ImGui::SameLine();

			ax::NodeEditor::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Output);
			{
				ImGui::Text("Out");
			}
			ax::NodeEditor::EndPin();
		}
		ax::NodeEditor::EndNode();
	}
	ax::NodeEditor::End();
}
