//
// Created by mtesseract on 03-01-20.
//

#include <gfx/window/ContextWindow.hpp>
#include <aux/logging/Logger.hpp>
#include "Swapchain.hpp"
namespace mt::gfx::mtvk {
    Swapchain::Swapchain(const std::shared_ptr<Device>& device, const std::shared_ptr<Surface>& surface, const std::shared_ptr<ContextWindow>& window) : device(device) {
        auto physical_device = device->get_physical_device();

        auto surface_formats = physical_device.getSurfaceFormatsKHR(surface->get_surface());
        auto present_modes = physical_device.getSurfacePresentModesKHR(surface->get_surface());
        auto capabilities = physical_device.getSurfaceCapabilitiesKHR(surface->get_surface());

        auto surface_format = choose_swap_surface_format(surface_formats);
        auto present_mode = choose_swap_present_mode(present_modes);
        auto swapchain_extent = choose_swap_extent(capabilities, window);

        uint32_t image_count = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && image_count > capabilities.maxImageCount) {
            image_count = capabilities.maxImageCount;
        }

        vk::SwapchainCreateInfoKHR create_info;
        create_info.surface = surface->get_surface();
        create_info.minImageCount = image_count;
        create_info.imageFormat = surface_format.format;
        create_info.imageColorSpace = surface_format.colorSpace;
        create_info.imageExtent = swapchain_extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = vk::ImageUsageFlagBits ::eColorAttachment;

        auto indices = device->get_queue_indices();
        std::array<uint32_t, 2> queue_family_indices = {indices.graphics_family.value(), indices.present_family.value()};

        if(indices.graphics_family != indices.present_family){
            create_info.imageSharingMode = vk::SharingMode ::eConcurrent;
            create_info.queueFamilyIndexCount = queue_family_indices.size();
            create_info.pQueueFamilyIndices = queue_family_indices.data();
        } else{
            create_info.imageSharingMode = vk::SharingMode ::eExclusive;
            create_info.queueFamilyIndexCount = 0;
            create_info.pQueueFamilyIndices = nullptr;
        }

        create_info.preTransform = capabilities.currentTransform;
        create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR ::eOpaque;
        create_info.presentMode = present_mode;
        create_info.clipped = true;
        create_info.oldSwapchain = nullptr;

        swapchain = device->get_device().createSwapchainKHR(create_info);

        aux::Logger::log("Set up Swapchain", aux::LogType::Info);

        images = device->get_device().getSwapchainImagesKHR(swapchain);

        aux::Logger::log("Created Swapchain Images", aux::LogType::Info);

        image_format = surface_format.format;
        extent = swapchain_extent;

        create_image_views();
    }


    Swapchain::~Swapchain() {
        destroy_image_views();

        if(swapchain) device->get_device().destroySwapchainKHR(swapchain);
        aux::Logger::log("Destroyed Swapchain", aux::LogType::Info);
    }

    vk::SurfaceFormatKHR
    Swapchain::choose_swap_surface_format(const std::vector<vk::SurfaceFormatKHR> &available_formats) {
        for(const auto& available_format : available_formats){
            if(available_format.format == vk::Format::eB8G8R8A8Unorm && available_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear){
                return available_format;
            }
        }
        return available_formats[0]; //If nothing else works, just pick the first one
    }

    vk::PresentModeKHR Swapchain::choose_swap_present_mode(const std::vector<vk::PresentModeKHR> &available_modes) {
        for(const auto& available_mode : available_modes){
            if(available_mode == vk::PresentModeKHR::eMailbox){
                return available_mode;
            }
        }
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D Swapchain::choose_swap_extent(const vk::SurfaceCapabilitiesKHR &capabilities, const std::shared_ptr<ContextWindow>& window) {
        if(capabilities.currentExtent.width != UINT32_MAX){
            return capabilities.currentExtent;
        } else{
            return vk::Extent2D {
                    std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, window->get_width())),
                    std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, window->get_height()))
            };
        }
    }

    void Swapchain::create_image_views() {
        image_views.resize(images.size());
        for (size_t i = 0; i < images.size(); i++) {
            vk::ImageViewCreateInfo create_info;
            create_info.image = images[i];
            create_info.viewType = vk::ImageViewType ::e2D;
            create_info.format = image_format;
            create_info.components.r = vk::ComponentSwizzle ::eIdentity;
            create_info.components.g = vk::ComponentSwizzle ::eIdentity;
            create_info.components.b = vk::ComponentSwizzle ::eIdentity;
            create_info.components.a = vk::ComponentSwizzle ::eIdentity;
            create_info.subresourceRange.aspectMask = vk::ImageAspectFlagBits ::eColor;
            create_info.subresourceRange.baseMipLevel = 0;
            create_info.subresourceRange.levelCount = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.layerCount = 1;
            image_views[i] = device->get_device().createImageView(create_info);
        }
    }

    void Swapchain::destroy_image_views() {
        for (auto image_view : image_views) {
            if(image_view) device->get_device().destroyImageView(image_view);
        }
    }

    vk::Extent2D Swapchain::get_extent() {
        return extent;
    }
}