//
// Created by mtesseract on 13-01-20.
//

#include "RenderPass.hpp"
#include <gfx/vulkan/display/Swapchain.hpp>

namespace mt::gfx::mtvk {

    RenderPass::RenderPass(const std::shared_ptr<Device>& device, const Swapchain& swapchain) : device(device) {
        vk::AttachmentDescription color_attachment;
        color_attachment.format            = swapchain.get_format();
        color_attachment.samples           = vk::SampleCountFlagBits::e1;
        color_attachment.loadOp            = vk::AttachmentLoadOp::eClear;
        color_attachment.storeOp           = vk::AttachmentStoreOp::eStore;
        color_attachment.stencilLoadOp     = vk::AttachmentLoadOp::eDontCare;
        color_attachment.stencilStoreOp    = vk::AttachmentStoreOp::eDontCare;
        color_attachment.initialLayout     = vk::ImageLayout::eUndefined;
        color_attachment.finalLayout       = vk::ImageLayout::ePresentSrcKHR;

        vk::AttachmentReference color_attachment_ref;
        color_attachment_ref.attachment    = 0;
        color_attachment_ref.layout        = vk::ImageLayout::eColorAttachmentOptimal;

        vk::SubpassDescription subpass;
        subpass.pipelineBindPoint    = vk::PipelineBindPoint::eGraphics;
        subpass.pColorAttachments    = &color_attachment_ref;

        vk::SubpassDependency dependency;
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.srcAccessMask = vk::AccessFlagBits(0);
        dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

        vk::RenderPassCreateInfo create_info;
        create_info.attachmentCount    = 1;
        create_info.pAttachments       = &color_attachment;
        create_info.subpassCount       = 1;
        create_info.pSubpasses         = &subpass;
        create_info.dependencyCount    = 1;
        create_info.pDependencies      = &dependency;

        render_pass = device->get_device().createRenderPass(create_info);
    }

    void RenderPass::destroy() {
        device->get_device().destroyRenderPass(render_pass);
    }

    vk::RenderPass RenderPass::get_render_pass() const {
        return render_pass;
    }
}