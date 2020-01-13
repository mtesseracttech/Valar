//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_SURFACE_HPP
#define VALCANO_SURFACE_HPP

#include <gfx/window/ContextWindow.hpp>
#include "vulkan/vulkan.hpp"
#include "Instance.hpp"

namespace mt::gfx::mtvk {
    class Surface {
        vk::SurfaceKHR surface;
        std::shared_ptr<Instance> instance;
    public:
        Surface(const std::shared_ptr<Instance>& instance, const ContextWindow& window);
        ~Surface() = default;

        void destroy();

        vk::SurfaceKHR get_surface() const;
    };
}



#endif //VALCANO_SURFACE_HPP
