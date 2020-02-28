//
// Created by MTesseracT on 17/02/2020.
//

#ifndef VALCANO_VERTEXLAYOUT_HPP
#define VALCANO_VERTEXLAYOUT_HPP

#include "math/linalg/LinAlgTypes.hpp"
#include <vulkan/vulkan.hpp>

namespace mt::gfx {
	struct Vertex {
		Vec2 pos;
		Vec3 colour;

		static vk::VertexInputBindingDescription getBindingDescription() {
			vk::VertexInputBindingDescription bindingDescription;
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = vk::VertexInputRate::eVertex;
			return bindingDescription;
		}

		static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions() {
			std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = {};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
			attributeDescriptions[1].offset = offsetof(Vertex, colour);
			return attributeDescriptions;
		}
	};
}


#endif //VALCANO_VERTEXLAYOUT_HPP
