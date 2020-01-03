//
// Created by MTesseracT on 31/12/2019.
//

#ifndef VALCANO_RENDERER_HPP
#define VALCANO_RENDERER_HPP

#include <gfx/vulkan/Instance.hpp>
#include <memory>
#include <gfx/window/ContextWindow.hpp>
#include <gfx/vulkan/VulkanDebug.hpp>
#include <gfx/vulkan/Device.hpp>

namespace mt::gfx{
	class Renderer {
		std::weak_ptr<ContextWindow> window;
		std::shared_ptr<mtvk::VulkanDebug> debugging;
		std::shared_ptr<mtvk::Instance> instance;
		std::shared_ptr<mtvk::Device> device;

	public:
		Renderer(const std::weak_ptr<ContextWindow>& render_window, const std::string& app_name);
		~Renderer();
	};
}



#endif //VALCANO_RENDERER_HPP
