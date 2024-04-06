#include "shadermaker.h"

#include <imgui.h>
#include <imgui_node_editor.h>

#define ICON_SIZE 16

using namespace sm::maker;

Window::Window()
{
	ax::NodeEditor::Config nodesConfig;
	context = ax::NodeEditor::CreateEditor(&nodesConfig);

	nodes.emplace_back(nextId++, "Smoothie");
	nodes.back().inputs.emplace_back(nextId++, "Strawberry", PinType::FLOAT);
	nodes.back().inputs.emplace_back(nextId++, "Kiwi", PinType::FLOAT);
	int out = nextId++;
	nodes.back().outputs.emplace_back(out, "Banana", PinType::INT);
	buildNode(&nodes.back());

	nodes.emplace_back(nextId++, "Smoothie");
	nodes.back().inputs.emplace_back(nextId++, "Strawberry", PinType::FLOAT);
	int in = nextId++;
	nodes.back().inputs.emplace_back(in, "Kiwi", PinType::FLOAT);
	nodes.back().outputs.emplace_back(nextId++, "Banana", PinType::INT);
	buildNode(&nodes.back());

	links.emplace_back(nextId++, out, in);

	iconsTexture = new Texture("assets/textures/icons/icons.png");
}

Window::~Window()
{
	ax::NodeEditor::DestroyEditor(context);
}

void Window::buildNode(Node* node)
{
	for (auto& input : node->inputs)
	{
		input.node = node;
		input.kind = ax::NodeEditor::PinKind::Input;
	}

	for (auto& output : node->outputs)
	{
		output.node = node;
		output.kind = ax::NodeEditor::PinKind::Output;
	}
}

void Window::doGui()
{
	ax::NodeEditor::SetCurrentEditor(this->context);
	ax::NodeEditor::Begin("Nodes");
	{
		int uniqueId = 1;

		for (Node node : nodes) {
			ax::NodeEditor::BeginNode(uniqueId++);
			{
				ImGui::Text(node.name.c_str());

				for (Link link : links) {
					ax::NodeEditor::Link(link.id, link.startPinId, link.endPinId);
				}

				ImGui::BeginGroup();
				for (Pin inputPin : node.inputs) {
					ax::NodeEditor::BeginPin(uniqueId++, inputPin.kind);
					{
						// Icon
						ImVec2 uv0, uv1;
						this->getPinTypeTexCoords(inputPin.type, &uv0, &uv1);
						ImGui::Image((ImTextureID)iconsTexture->getId(), ImVec2{ ICON_SIZE, ICON_SIZE }, uv0, uv1);

						ImGui::SameLine();
						
						ImGui::Text(inputPin.name.c_str());
					}
					ax::NodeEditor::EndPin();
				}
				ImGui::EndGroup();
				
				ImGui::SameLine();

				ImGui::BeginGroup();
				for (Pin outputPin : node.outputs) {
					ax::NodeEditor::BeginPin(uniqueId++, outputPin.kind);
					{
						ImGui::Text(outputPin.name.c_str());
						
						ImGui::SameLine();

						// Icon
						ImVec2 uv0, uv1;
						this->getPinTypeTexCoords(outputPin.type, &uv0, &uv1);
						ImGui::Image((ImTextureID)iconsTexture->getId(), ImVec2{ ICON_SIZE, ICON_SIZE }, uv0, uv1);
					}
					ax::NodeEditor::EndPin();
				}
				ImGui::EndGroup();
			}
			ax::NodeEditor::EndNode();
		}
	}
	ax::NodeEditor::End();
}

void Window::getPinTypeTexCoords(PinType type, ImVec2* uv0, ImVec2* uv1)
{
	switch (type)
	{
	case sm::maker::PinType::INT:
	{
		*uv0 = ImVec2{ 0.5f, 0.f };
		*uv1 = ImVec2{ 1.f, 0.5f };
		break;
	}
	case sm::maker::PinType::FLOAT:
	case sm::maker::PinType::VEC2:
	case sm::maker::PinType::VEC3:
	case sm::maker::PinType::VEC4:
	default:
	{
		*uv0 = ImVec2{ 0.f, 0.f };
		*uv1 = ImVec2{ 0.5f, 0.5f };
		break;
	}
	}
}
