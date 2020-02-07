//
// Created by mtesseract on 03-01-20.
//

#ifndef VALCANO_SWAPCHAIN_HPP
#define VALCANO_SWAPCHAIN_HPP

#include <gfx/vulkan/pipelines/RenderPass.hpp>
#include "vulkan/vulkan.hpp"
#include "Device.hpp"

#include <gfx/window/ContextWindow.hpp>
#include <aux/logging/Logger.hpp>


namespace mt::gfx::mtvk {
    class Swapchain {
        std::shared_ptr<Device> device;

        vk::SwapchainKHR swapchain;
        vk::Format image_format;
        vk::Extent2D extent;

        std::vector<vk::Image> images;
        std::vector<vk::ImageView> image_views;
        std::vector<vk::Framebuffer> framebuffers;

    public:
        Swapchain(const std::shared_ptr<Device>& device, const Surface& surface, const ContextWindow& window) : device(device) {
            auto physical_device = device->get_physical_device();

            auto surface_formats = physical_device.getSurfaceFormatsKHR(surface.get_surface());
            auto present_modes = physical_device.getSurfacePresentModesKHR(surface.get_surface());
            auto capabilities = physical_device.getSurfaceCapabilitiesKHR(surface.get_surface());

            auto surface_format = choose_swap_surface_format(surface_formats);
            auto present_mode = choose_swap_present_mode(present_modes);
            auto swapchain_extent = choose_swap_extent(capabilities, window);

            uint32_t image_count = capabilities.minImageCount + 1;
            if (capabilities.maxImageCount > 0 && image_count > capabilities.maxImageCount) {
                image_count = capabilities.maxImageCount;
            }

            vk::SwapchainCreateInfoKHR create_info;
            create_info.surface = surface.get_surface();
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

            Logger::log("Set up Swapchain", Info);

            images = device->get_device().getSwapchainImagesKHR(swapchain);

            Logger::log("Created Swapchain Images", Info);

            image_format = surface_format.format;
            extent = swapchain_extent;

            create_image_views();
        }

        ~Swapchain() = default;

        void destroy(){
            destroy_image_views();
            if(swapchain) device->get_device().destroySwapchainKHR(swapchain);
            Logger::log("Destroyed Swapchain", Info);
        }

        void create_framebuffers(const RenderPass& render_pass) {
            framebuffers.resize(image_views.size());

            for(std::size_t i = 0; i < image_views.size(); i++){
                vk::ImageView attachments[] = {
                        image_views[i]
                };

                vk::FramebufferCreateInfo framebuffer_info;
                framebuffer_info.renderPass = render_pass.get_render_pass();
                framebuffer_info.attachmentCount = 1;
                framebuffer_info.pAttachments = attachments;
                framebuffer_info.width = extent.width;
                framebuffer_info.height = extent.height;
                framebuffer_info.layers = 1;

                framebuffers[i] = device->get_device().createFramebuffer(framebuffer_info);
            }
        }
        void destroy_framebuffers() {
            for(auto framebuffer : framebuffers){
                device->get_device().destroyFramebuffer(framebuffer);
            }
            framebuffers.clear();
            Logger::log("Destroyed Swapchain Framebuffers", Info);
        }

        vk::Extent2D get_extent() const {
            return extent;
        }
        vk::Format get_format() const {
            return image_format;
        }

        vk::SwapchainKHR get_swapchain() const {
            return swapchain;
        }

        std::vector<vk::Framebuffer> get_framebuffers() const {
            return framebuffers;
        }


    private:
        vk::SurfaceFormatKHR choose_swap_surface_format(const std::vector<vk::SurfaceFormatKHR>& available_formats){
            for(const auto& available_format : available_formats){
                if(available_format.format == vk::Format::eB8G8R8A8Unorm && available_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear){
                    return available_format;
                }
            }
            return available_formats[0]; //If nothing else works, just pick the first one
        }

        vk::PresentModeKHR choose_swap_present_mode(const std::vector<vk::PresentModeKHR>& available_modes){
            for(const auto& available_mode : available_modes){
                if(available_mode == vk::PresentModeKHR::eMailbox){
                    return available_mode;
                }
            }
            return vk::PresentModeKHR::eFifo;
        }
        vk::Extent2D choose_swap_extent(const vk::SurfaceCapabilitiesKHR& capabilities, const ContextWindow& window){
            if(capabilities.currentExtent.width != UINT32_MAX){
                return capabilities.currentExtent;
            } else{
                return vk::Extent2D {
                        std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, window.get_width())),
                        std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, window.get_height()))
                };
            }
        }

        void create_image_views(){
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

        void destroy_image_views(){
            for (auto image_view : image_views) {
                if(image_view) device->get_device().destroyImageView(image_view);
            }
            Logger::log("Destroyed Swapchain Image Views", Info);
        }

    };
}



#endif //VALCANO_SWAPCHAIN_HPP
