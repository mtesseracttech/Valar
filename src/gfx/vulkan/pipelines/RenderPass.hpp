//
// Created by mtesseract on 13-01-20.
//

#ifndef VALCANO_RENDERPASS_HPP
#define VALCANO_RENDERPASS_HPP

#include <gfx/vulkan/display/Device.hpp>

namespace mt::gfx::mtvk {
class Swapchain;

class RenderPass {
    std::shared_ptr<Device> device;

    vk::RenderPass render_pass;

public:
    explicit RenderPass(const std::shared_ptr<Device>& device);
    ~RenderPass() = default;

    void create(const Swapchain& swapchain);

    void destroy();

    vk::RenderPass get_render_pass() const;
};
} // namespace mt::gfx::mtvk

#endif // VALCANO_RENDERPASS_HPP
