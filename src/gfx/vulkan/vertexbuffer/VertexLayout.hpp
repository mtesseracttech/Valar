//
// Created by MTesseracT on 17/02/2020.
//

#ifndef VALCANO_VERTEXLAYOUT_HPP
#define VALCANO_VERTEXLAYOUT_HPP

#include <vulkan/vulkan.hpp>
#include "gfx/vulkan/shaders/Shader.hpp"

namespace mt::gfx::mtvk {
	class DynamicVertexLayout{
		vk::VertexInputBindingDescription binding_description;
		std::vector<vk::VertexInputAttributeDescription> attribute_descriptions;

	public:
		explicit DynamicVertexLayout(const Shader::ShaderMetaModule& shader_module, uint32_t binding){
			auto reflection_data = shader_module.get_reflection_data();
			auto res = reflection_data.get_shader_reflection_resources();
			auto comp = reflection_data.get_compiler();

			binding_description.binding = binding;

			binding_description.stride = 0;
			for(const auto& input : res->stage_inputs){
				auto type = comp->get_type(input.base_type_id);
				auto vecsize = type.vecsize;
				auto base_type_size = reflection_data.get_base_type_size(type.basetype);
				binding_description.stride += base_type_size * vecsize;
			}
			Logger::log("Total Stage Stride: " +  to_str(binding_description.stride));

			binding_description.inputRate = vk::VertexInputRate::eVertex;
		}

	};

	struct StandardVertexLayout {
		float pos[2];
		float colour[3];

		static vk::VertexInputBindingDescription getBindingDescription() {
			vk::VertexInputBindingDescription bindingDescription;
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(StandardVertexLayout);
			bindingDescription.inputRate = vk::VertexInputRate::eVertex;
			return bindingDescription;
		}

		static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions() {
			std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = {};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
			attributeDescriptions[0].offset = offsetof(StandardVertexLayout, pos);
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
			attributeDescriptions[1].offset = offsetof(StandardVertexLayout, colour);
			return attributeDescriptions;
		}
	};
}


#endif //VALCANO_VERTEXLAYOUT_HPP
