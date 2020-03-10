//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_SURFACE_HPP
#define VALCANO_SURFACE_HPP

#include <gfx/window/ContextWindow.hpp>
#include <util/GlfwHelper.hpp>

#include "Instance.hpp"
#include "vulkan/vulkan.hpp"

namespace mt::gfx::mtvk {
class Surface {
    vk::SurfaceKHR surface;
    std::shared_ptr<Instance> instance;

public:
    explicit Surface(const std::shared_ptr<Instance>& instance)
        : instance(instance)
    {
    }

    ~Surface() = default;

    void create(const ContextWindow& window)
    {
        surface = GlfwHelper::get_glfw_surface(instance->get_instance(),
            window.get_window());
        Logger::log("Created the Window Surface", Info);
    }

    void destroy()
    {
        instance->get_instance().destroySurfaceKHR(surface);
        Logger::log("Destroyed the Window Surface", Info);
    }

    vk::SurfaceKHR get_surface() const { return surface; }
};
} // namespace mt::gfx::mtvk

#endif // VALCANO_SURFACE_HPP
