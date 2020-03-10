//
// Created by MTesseracT on 31/12/2019.
//

#include "VulkanRenderer.hpp"

#include <aux/logging/Logger.hpp>
#include <gfx/vulkan/vertexbuffer/VertexLayout.hpp>

namespace mt::gfx::mtvk {
VulkanRenderer::VulkanRenderer(
    const std::shared_ptr<ContextWindow>& render_window)
    : Renderer(render_window)
{
    Logger::log("Creating the renderer", LogType::Info);
    instance = std::make_shared<mtvk::Instance>("Valcano");
    if (instance->has_validation_layers()) {
        debugging = std::make_shared<mtvk::VulkanDebug>(instance);
    }

    surface = std::make_shared<mtvk::Surface>(instance);
    surface->create(*window);

    device = std::make_shared<mtvk::Device>(surface);
    device->create(*instance);

    swapchain = std::make_shared<mtvk::Swapchain>(device);
    swapchain->create(*surface, *window);

    render_pass = std::make_shared<mtvk::RenderPass>(device);
    render_pass->create(*swapchain);

    swapchain->create_framebuffers(*render_pass);

    // Depends on the shaders
    test_shader = std::make_shared<mtvk::Shader>(device, "base",
        mtvk::ShaderSourceType::GLSL);
    test_shader->create();

    for (const auto& module : test_shader->get_shader_meta_modules()) {
        DynamicVertexLayout layout(module, 0);
    }

    test_shader_pipeline = std::make_shared<mtvk::GraphicsPipeline>(device, test_shader);
    test_shader_pipeline->create(*render_pass, *swapchain);

    // Depends on the scene
    command_buffer = std::make_shared<mtvk::CommandBuffer>(device, max_frames_in_flight);
    command_buffer->create_command_pool();
    command_buffer->allocate_command_buffers(*swapchain);
    command_buffer->create_command_buffers(*test_shader_pipeline, *swapchain,
        *render_pass);
    command_buffer->create_sync_objects(*swapchain);
    // Stop the console spam
    Logger::set_log_level(LogLevel::LevelError);
}

void VulkanRenderer::draw()
{
    if (!command_buffer->submit_command_buffers(*swapchain)) {
        recreate_swapchain();
    }
}

void VulkanRenderer::terminate()
{
    // Restart Console Spam
    Logger::set_log_level(LogLevel::LevelAll);
    cleanup_swapchain();
    command_buffer->destroy();
    test_shader->destroy();
    device->destroy();
    if (debugging)
        debugging->destroy();
    surface->destroy();
    instance->destroy();
    Logger::log("Done terminating the renderer", Info);
}

void VulkanRenderer::on_resize(uint32_t new_width, uint32_t new_height)
{
    recreate_swapchain();
    Logger::log("Framebuffer Resized to: " + std::to_string(new_width) + "x" + std::to_string(new_height));
}

void VulkanRenderer::recreate_swapchain()
{
    device->wait_till_idle();
    cleanup_swapchain();
    swapchain->create(*surface, *window);
    render_pass->create(*swapchain);
    swapchain->create_framebuffers(*render_pass);
    test_shader_pipeline->create(*render_pass, *swapchain);
    command_buffer->allocate_command_buffers(*swapchain);
    command_buffer->create_command_buffers(*test_shader_pipeline, *swapchain,
        *render_pass);
}

void VulkanRenderer::cleanup_swapchain()
{
    swapchain->destroy_framebuffers();
    // Free command buffers
    command_buffer->free_buffers();
    // Destroy Pipelines and layouts
    test_shader_pipeline->destroy();
    render_pass->destroy();
    swapchain->destroy();
}
} // namespace mt::gfx::mtvk