//
// Created by MTesseracT on 31/12/2019.
//

#ifndef VALCANO_RENDERER_HPP
#define VALCANO_RENDERER_HPP

#include <memory>

#include <gfx/vulkan/Instance.hpp>
#include <gfx/window/ContextWindow.hpp>
#include <gfx/vulkan/VulkanDebug.hpp>
#include <gfx/vulkan/Device.hpp>
#include <gfx/vulkan/Surface.hpp>

namespace mt::gfx{
	class Renderer {
		std::shared_ptr<ContextWindow> window;
		std::shared_ptr<mtvk::VulkanDebug> debugging;
		std::shared_ptr<mtvk::Instance> instance;
		std::shared_ptr<mtvk::Device> device;
		std::shared_ptr<mtvk::Surface> surface;

	public:
		Renderer(const std::shared_ptr<ContextWindow>& render_window, const std::string& app_name);
		~Renderer();
	};
}



#endif //VALCANO_RENDERER_HPP
