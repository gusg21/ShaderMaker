#include "shadermaker.h"

#include <imgui.h>
#include <imgui_node_editor.h>
#include <misc/cpp/imgui_stdlib.h>

#define ICON_SIZE 16

using namespace sm::maker;

Window::Window()
{
	ax::NodeEditor::Config nodesConfig;
	context = ax::NodeEditor::CreateEditor(&nodesConfig);

	iconsTexture = new Texture("assets/textures/icons/icons.png");

	nodeSpecs.emplace_back("Output Assignment (Int)", "",
		std::vector<PinSpec>{
		PinSpec{ "Value", PinType::INT }
	}, std::vector<PinSpec>{}, true
			);
	nodeSpecs.emplace_back("Output Assignment (Vec3)", "",
		std::vector<PinSpec>{
		PinSpec{ "Value", PinType::VEC3 }
	}, std::vector<PinSpec>{}, true
	);
	nodeSpecs.emplace_back("Constant (Float)", "",
		std::vector<PinSpec>{}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::FLOAT }
	}, true
	);
	nodeSpecs.emplace_back("Constant (Int)", "",
		std::vector<PinSpec>{}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::INT }
	}, true
	);
	nodeSpecs.emplace_back("Constant (Vec2)", "",
		std::vector<PinSpec>{}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::VEC2 }
	}, true
	);
	nodeSpecs.emplace_back("Constant (Vec3)", "",
		std::vector<PinSpec>{}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::VEC3 }
	}, true
	);
	nodeSpecs.emplace_back("Constant (Vec4)", "",
		std::vector<PinSpec>{}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::VEC4 }
	}, true
	);
	nodeSpecs.emplace_back("Compose (Vec2)", "vec2",
		std::vector<PinSpec>{
		PinSpec{ "X", PinType::FLOAT },
			PinSpec{ "Y", PinType::FLOAT },
	}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::VEC2 }
	}
	);
	nodeSpecs.emplace_back("Compose (Vec3)", "vec3",
		std::vector<PinSpec>{
		PinSpec{ "X", PinType::FLOAT },
			PinSpec{ "Y", PinType::FLOAT },
			PinSpec{ "Z", PinType::FLOAT },
	}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::VEC3 }
	}
	);
	nodeSpecs.emplace_back("Compose (Vec4)", "vec4",
		std::vector<PinSpec>{
		PinSpec{ "X", PinType::FLOAT },
			PinSpec{ "Y", PinType::FLOAT },
			PinSpec{ "Z", PinType::FLOAT },
			PinSpec{ "W", PinType::FLOAT },
	}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::VEC4 }
	}
	);
	nodeSpecs.emplace_back("Lerp (Float)", "lerp",
		std::vector<PinSpec>{
		PinSpec{ "A", PinType::FLOAT },
			PinSpec{ "B", PinType::FLOAT },
			PinSpec{ "Delta", PinType::FLOAT },
	}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::FLOAT }
	}
	);
	nodeSpecs.emplace_back("Lerp (Vec2)", "lerp",
		std::vector<PinSpec>{
		PinSpec{ "A", PinType::VEC2 },
			PinSpec{ "B", PinType::VEC2 },
			PinSpec{ "Delta", PinType::FLOAT },
	}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::VEC2 }
	}
	);
	nodeSpecs.emplace_back("Lerp (Vec3)", "lerp",
		std::vector<PinSpec>{
		PinSpec{ "A", PinType::VEC3 },
			PinSpec{ "B", PinType::VEC3 },
			PinSpec{ "Delta", PinType::FLOAT },
	}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::VEC3 }
	}
	);
	nodeSpecs.emplace_back("Lerp (Vec4)", "lerp",
		std::vector<PinSpec>{
		PinSpec{ "A", PinType::VEC4 },
			PinSpec{ "B", PinType::VEC4 },
			PinSpec{ "Delta", PinType::FLOAT },
	}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::VEC4 }
	}
	);
	nodeSpecs.emplace_back("Floor", "floor",
		std::vector<PinSpec>{
		PinSpec{ "X", PinType::FLOAT },
	}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::INT }
	}
	);
	nodeSpecs.emplace_back("Integer Divide", "div",
		std::vector<PinSpec>{
		PinSpec{ "X", PinType::INT },
			PinSpec{ "Y", PinType::INT },
	}, std::vector<PinSpec>{
		PinSpec{ "Value", PinType::INT }
	}
	);
}

Window::~Window()
{
	ax::NodeEditor::DestroyEditor(context);
}

void Window::createNodeFromSpec(const NodeSpec& spec)
{
	nodes.emplace_back(nextId++, spec.name, spec.isConstant, &spec);
	Node* node = &nodes.back();
	for (const PinSpec& pinSpec : spec.inputs) {
		node->inputs.emplace_back(nextId++, pinSpec.name, pinSpec.type, node, ax::NodeEditor::PinKind::Input);
	}
	for (const PinSpec& pinSpec : spec.outputs) {
		node->outputs.emplace_back(nextId++, pinSpec.name, pinSpec.type, node, ax::NodeEditor::PinKind::Output);
	}
}

void Window::createNodeFromSpecAt(const NodeSpec& spec, ImVec2 position)
{
	nodes.emplace_back(nextId++, spec.name, spec.isConstant, &spec);
	Node* node = &nodes.back();
	for (const PinSpec& pinSpec : spec.inputs) {
		node->inputs.emplace_back(nextId++, pinSpec.name, pinSpec.type, node, ax::NodeEditor::PinKind::Input);
	}
	for (const PinSpec& pinSpec : spec.outputs) {
		node->outputs.emplace_back(nextId++, pinSpec.name, pinSpec.type, node, ax::NodeEditor::PinKind::Output);
	}

	ax::NodeEditor::SetNodePosition(node->id, position);
}

void Window::doGui()
{
	ImGui::Begin("Shader Maker");
	{
		auto showLabel = [](const char* label, ImColor color)
			{
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
				auto size = ImGui::CalcTextSize(label);

				auto padding = ImGui::GetStyle().FramePadding;
				auto spacing = ImGui::GetStyle().ItemSpacing;

				ImGui::SetCursorPos(ImVec2(spacing.x + ImGui::GetCursorPos().x, -spacing.y + ImGui::GetCursorPos().y));

				auto rectMin = ImVec2{
					ImGui::GetCursorScreenPos().x - padding.x,
					ImGui::GetCursorScreenPos().y - padding.y
				};
				auto rectMax = ImVec2{
					ImGui::GetCursorScreenPos().x + size.x + padding.x,
					ImGui::GetCursorScreenPos().y + size.y + padding.y
				};

				auto drawList = ImGui::GetWindowDrawList();
				drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
				ImGui::TextUnformatted(label);
			};

		ax::NodeEditor::SetCurrentEditor(this->context);
		ax::NodeEditor::Begin("Nodes");
		{
			// Handle creation action, returns true if editor want to create new object (node or link)
			if (ax::NodeEditor::BeginCreate())
			{
				ax::NodeEditor::PinId startPinId, endPinId;
				if (ax::NodeEditor::QueryNewLink(&startPinId, &endPinId))
				{
					if (startPinId && endPinId) // both are valid, let's accept link
					{
						const Pin* inPin, * outPin;
						const Pin* startPin = findPinById(startPinId);
						const Pin* endPin = findPinById(endPinId);
						if (startPin->kind == ax::NodeEditor::PinKind::Input) {
							inPin = startPin;
							outPin = endPin;
						}
						else {
							inPin = endPin;
							outPin = startPin;
						}

						bool isInToOut = inPin->kind != outPin->kind;
						bool areTypesCompat = canCastFrom(outPin->type, inPin->type);
						if (isInToOut && areTypesCompat && ax::NodeEditor::AcceptNewItem())
						{
							// Since we accepted new link, lets add one to our list of links.
							links.emplace_back(nextId++, inPin, outPin);
						}
					}
					else {
						ax::NodeEditor::RejectNewItem();
					}
				}

				ax::NodeEditor::PinId sourcePinId;
				if (ax::NodeEditor::QueryNewNode(&sourcePinId))
				{
					const Pin* newLinkPin = findPinById(sourcePinId);
					if (newLinkPin != nullptr)
						showLabel("+ New Node", ImColor{ 0.f, 0.4f, 0.9f });

					if (ax::NodeEditor::AcceptNewItem()) {
						ax::NodeEditor::Suspend();
						ImGui::OpenPopup("Create New Node");
						ax::NodeEditor::Resume();
					}
				}
			}
			ax::NodeEditor::EndCreate(); // Wraps up object creation action handling.

			//if (ax::NodeEditor::BeginDelete())
			//{
			//	// There may be many links marked for deletion, let's loop over them.
			//	ax::NodeEditor::LinkId deletedLinkId;
			//	while (ax::NodeEditor::QueryDeletedLink(&deletedLinkId))
			//	{
			//		// If you agree that link can be deleted, accept deletion.
			//		if (ax::NodeEditor::AcceptDeletedItem())
			//		{
			//			// Then remove link from your data.
			//			for (const Link& link : links)
			//			{
			//				if (link.id == deletedLinkId)
			//				{
			//					links.erase(link);
			//					break;
			//				}
			//			}
			//		}

			//		// You may reject link deletion by calling:
			//		// ed::RejectDeletedItem();
			//	}
			//}
			//ax::NodeEditor::EndDelete(); // Wrap up deletion action

			// Nodes
			for (Node& node : nodes) {
				ax::NodeEditor::BeginNode(node.id);
				{
					if (node.isDataHook && node.inputs.size() == 0) { // no inputs
						ImGui::SetNextItemWidth(100);
						ImGui::PushID(node.id.Get());
						ImGui::InputText("", node.data, 255);
						ImGui::SameLine();
						ImGui::Text("%d", node.id.Get());
						ImGui::PopID();
					} else {
						// Node name
						ImGui::Text(node.name.c_str());
						ImGui::SameLine();
						ImGui::Text("%d", node.id.Get());

						// Inputs
						ImGui::BeginGroup();
						for (Pin inputPin : node.inputs) {
							ax::NodeEditor::BeginPin(inputPin.id, inputPin.kind);
							{
								ImGui::Text("%d", inputPin.id.Get());
								ImGui::SameLine();

								// Icon
								ImVec2 uv0, uv1;
								this->getPinTypeTexCoords(inputPin.type, &uv0, &uv1);
								ImGui::Image((ImTextureID)iconsTexture->getId(), ImVec2{ ICON_SIZE, ICON_SIZE }, uv0, uv1);
							}
							ax::NodeEditor::EndPin();

							ImGui::SameLine();

							// Label
							ImGui::Text(inputPin.name.c_str());
						}
						ImGui::EndGroup();
					}

					ImGui::SameLine();

					if (node.isDataHook && node.outputs.size() == 0) { // no outputs
						ImGui::SetNextItemWidth(100);
						ImGui::PushID(node.id.Get());
						ImGui::InputText("", node.data, 255);
						ImGui::PopID();
					}
					else {
						// Outputs
						ImGui::BeginGroup();
						for (Pin outputPin : node.outputs) {
							// Label
							ImGui::Text(outputPin.name.c_str());

							ImGui::SameLine();

							ax::NodeEditor::BeginPin(outputPin.id, outputPin.kind);
							{
								ImGui::Text("%d", outputPin.id.Get());
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
				}
				ax::NodeEditor::EndNode();
			}

			// Links
			for (const Link& link : links) {
				ax::NodeEditor::Link(link.id, link.inPinId, link.outPinId, getPinTypeColor(link.inPin->type));
				ImGui::Text("%d", link.id.Get());
			}

			// Popups
			ax::NodeEditor::Suspend();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
			if (ImGui::BeginPopup("Create New Node")) {
				ImVec2 position = ax::NodeEditor::ScreenToCanvas(ImGui::GetIO().MousePos);

				ImGui::SeparatorText("New Node");

				for (const NodeSpec& spec : nodeSpecs) {
					if (ImGui::MenuItem(spec.name.c_str())) {
						createNodeFromSpecAt(spec, position);
					}
				}

				ImGui::EndPopup();
			}
			ImGui::PopStyleVar();
			ax::NodeEditor::Resume();
		}
		ax::NodeEditor::End();
	}
	ImGui::End();

	ImGui::Begin("Node Menu");
	{
		for (const NodeSpec& spec : nodeSpecs) {
			if (ImGui::MenuItem(spec.name.c_str())) {
				createNodeFromSpec(spec);
			}
		}

		ImGui::Separator();
		if (ImGui::Button("Compose Code for Selected")) {
			ax::NodeEditor::NodeId selectedId;
			ax::NodeEditor::GetSelectedNodes(&selectedId, 1);
			if (!selectedId.Invalid) {
				const Node* node = findNodeById(selectedId);
				printf("%s", composeCodeForNode(node).c_str());
			}
		}

		ImGui::Text("Link Count: %d", links.size());
		ImGui::Text("Node Count: %d", nodes.size());
	}
	ImGui::End();
}

void Window::getPinTypeTexCoords(PinType type, ImVec2* uv0, ImVec2* uv1)
{
	switch (type)
	{
	case sm::maker::PinType::INT:
	{
		*uv0 = ImVec2{ 0.25f, 0.f };
		*uv1 = ImVec2{ 0.5f, 0.25f };
		break;
	}
	case sm::maker::PinType::VEC2:
	{
		*uv0 = ImVec2{ 0.f, 0.25f };
		*uv1 = ImVec2{ 0.25f, 0.5f };
		break;
	}
	case sm::maker::PinType::VEC3:
	{
		*uv0 = ImVec2{ 0.25f, 0.25f };
		*uv1 = ImVec2{ 0.5f, 0.5f };
		break;
	}
	case sm::maker::PinType::VEC4:
	{
		*uv0 = ImVec2{ 0.5f, 0.25f };
		*uv1 = ImVec2{ 0.75f, 0.5f };
		break;
	}
	case sm::maker::PinType::FLOAT:
	default:
	{
		*uv0 = ImVec2{ 0.f, 0.f };
		*uv1 = ImVec2{ 0.25f, 0.25f };
		break;
	}
	}
}

ImVec4 Window::getPinTypeColor(PinType type) {
	switch (type)
	{
	case sm::maker::PinType::FLOAT:
		return ImVec4{ 0.f / 255.f, 255.f / 255.f, 183.f / 255.f, 1.f };
	case sm::maker::PinType::INT:
		return ImVec4{ 252.f / 255.f, 72.f / 255.f, 72.f / 255.f, 1.f };
	case sm::maker::PinType::VEC2:
		return ImVec4{ 84.f / 255.f, 26.f / 255.f, 136.f / 255.f, 1.f };
	case sm::maker::PinType::VEC3:
		return ImVec4{ 210.f / 255.f, 29.f / 255.f, 183.f / 255.f, 1.f };
	case sm::maker::PinType::VEC4:
		return ImVec4{ 226.f / 255.f, 215.f / 255.f, 38.f / 255.f, 1.f };
	default:
		return ImVec4{ 1.f, 0.f, 1.f, 1.f };
	}
}

bool Window::canCastFrom(PinType from, PinType to)
{
	if (from == to) return true;

	switch (from)
	{
	case sm::maker::PinType::FLOAT:
		return false;
	case sm::maker::PinType::INT:
		if (to == PinType::FLOAT)
			return true;
		return false;
	case sm::maker::PinType::VEC2:
		return false;
	case sm::maker::PinType::VEC3:
		return false;
	case sm::maker::PinType::VEC4:
		return false;
	default:
		return false;
	}

	return false;
}

const Pin* Window::findPinById(ax::NodeEditor::PinId id) const
{
	for (const Node& node : nodes) {
		for (size_t i = 0; i < node.inputs.size(); i++)
		{
			const Pin* pin = &node.inputs[i];
			if (pin->id.Get() == id.Get()) return pin;
		}
		for (size_t j = 0; j < node.outputs.size(); j++)
		{
			const Pin* pin = &node.outputs[j];
			if (pin->id.Get() == id.Get()) return pin;
		}
	}
	return nullptr;
}

const Node* sm::maker::Window::findNodeById(ax::NodeEditor::NodeId id) const
{
	for (size_t i = 0; i < nodes.size(); i++) {
		if (nodes[i].id == id) return &nodes[i];
	}
}

const Link* Window::findLinkEndingAtId(ax::NodeEditor::PinId id) const
{
	for (size_t i = 0; i < links.size(); i++) {
		if (links[i].outPinId == id) return &links[i];
	}
	return nullptr;
}

std::string Window::composeCodeForNode(const Node* node) {
	if (node->isDataHook && node->inputs.size() == 0) { // Constant
		return std::string(node->data);
	}

	std::string contents;
	bool isAssignment = node->isDataHook && node->outputs.size() == 0;
	if (isAssignment) { // Assignment (x = ...)
		contents += node->data;
		contents += " = ";
	}
	else { // Function
		contents += node->spec->funcName;
		contents += "(";
	}

	for (const Pin& inputPin : node->inputs) {
		const Link* link = findLinkEndingAtId(inputPin.id);
		if (link == nullptr) return "#ERR(no link #" + std::to_string(inputPin.id.Get()) + ")";
		contents += composeCodeForNode(link->outPin->node);
	}

	if (isAssignment) {
		contents += ";";
	}
	else {
		contents += ")";
	}

	return contents;
}
