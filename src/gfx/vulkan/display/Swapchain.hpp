//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_SWAPCHAIN_HPP
#define VALCANO_SWAPCHAIN_HPP

#include "vulkan/vulkan.hpp"
#include "Device.hpp"

namespace mt::gfx::mtvk {
    class Swapchain {
        vk::SwapchainKHR swapchain;

        std::vector<vk::Image> images;
        std::vector<vk::ImageView> image_views;

        vk::Format image_format;
        vk::Extent2D extent;

        std::shared_ptr<Device> device;

        vk::SurfaceFormatKHR choose_swap_surface_format(const std::vector<vk::SurfaceFormatKHR>& available_formats);
        vk::PresentModeKHR choose_swap_present_mode(const std::vector<vk::PresentModeKHR>& available_modes);
        vk::Extent2D choose_swap_extent(const vk::SurfaceCapabilitiesKHR& capabilities, const std::shared_ptr<ContextWindow>& window);

        void create_image_views();

        void destroy_image_views();
    public:
        Swapchain(const std::shared_ptr<Device>& device, const std::shared_ptr<Surface>& surface,  const std::shared_ptr<ContextWindow>& window);
        ~Swapchain();

        vk::Extent2D get_extent();
        vk::Format get_format();
    };
}



#endif //VALCANO_SWAPCHAIN_HPP
