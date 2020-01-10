//
// Created by mtesseract on 03-01-20.
//

#include "GraphicsPipeline.hpp"
#include <array>

namespace mt::gfx::mtvk {
    GraphicsPipeline::GraphicsPipeline(const Shader& shader,
            const std::shared_ptr<Device>& device,
            const std::weak_ptr<Swapchain>& swapchain) : device(device), shader(shader), swapchain(swapchain){
        create_render_pass();
        create_pipeline(shader);
    }

    GraphicsPipeline::~GraphicsPipeline() {
        auto actual_device = this->device->get_device();
        actual_device.destroyPipeline(graphics_pipeline);
        actual_device.destroyPipelineLayout(pipeline_layout);
        actual_device.destroyRenderPass(render_pass);
    }

    void GraphicsPipeline::create_render_pass() {
        vk::AttachmentDescription color_attachment;
        color_attachment.format            = swapchain.lock()->get_format();
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

        vk::RenderPassCreateInfo create_info;
        create_info.attachmentCount    = 1;
        create_info.pAttachments       = &color_attachment;
        create_info.subpassCount       = 1;
        create_info.pSubpasses         = &subpass;

        render_pass = device->get_device().createRenderPass(create_info);
    }

    void GraphicsPipeline::create_pipeline(const Shader &shader) {
        vk::Extent2D swapchain_extent = {0,0};
        if(!swapchain.expired()){
            swapchain_extent = swapchain.lock()->get_extent();
        }

        vk::PipelineVertexInputStateCreateInfo vertex_input_info;
        vertex_input_info.vertexBindingDescriptionCount      = 0;
        vertex_input_info.pVertexBindingDescriptions         = nullptr;
        vertex_input_info.vertexAttributeDescriptionCount    = 0;
        vertex_input_info.pVertexAttributeDescriptions       = nullptr;

        vk::PipelineInputAssemblyStateCreateInfo input_assembly_info;
        input_assembly_info.topology                  = vk::PrimitiveTopology ::eTriangleList;
        input_assembly_info.primitiveRestartEnable    = false;

        vk::Viewport viewport;
        viewport.x           = 0;
        viewport.y           = 0;
        viewport.width       = static_cast<float>(swapchain_extent.width);
        viewport.height      = static_cast<float>(swapchain_extent.height);
        viewport.minDepth    = 0.0f;
        viewport.maxDepth    = 1.0f;

        vk::Rect2D scissor;
        scissor.offset.x    = 0;
        scissor.offset.y    = 0;
        scissor.extent      = swapchain_extent;

        vk::PipelineViewportStateCreateInfo viewport_create_info;
        viewport_create_info.viewportCount    = 1;
        viewport_create_info.pViewports       = &viewport;
        viewport_create_info.scissorCount     = 1;
        viewport_create_info.pScissors        = &scissor;

        vk::PipelineRasterizationStateCreateInfo rasterization_create_info;
        rasterization_create_info.depthClampEnable           = false;
        rasterization_create_info.rasterizerDiscardEnable    = false;
        rasterization_create_info.polygonMode                = vk::PolygonMode ::eFill;
        rasterization_create_info.lineWidth                  = 1.0f;
        rasterization_create_info.cullMode                   = vk::CullModeFlagBits ::eBack;
        rasterization_create_info.frontFace                  = vk::FrontFace ::eClockwise;
        rasterization_create_info.depthBiasEnable            = false;
        rasterization_create_info.depthBiasConstantFactor    = 0.0f;
        rasterization_create_info.depthBiasClamp             = 0.0f;
        rasterization_create_info.depthBiasSlopeFactor       = 0.0f;

        vk::PipelineMultisampleStateCreateInfo multisample_create_info;
        multisample_create_info.sampleShadingEnable      = false;
        multisample_create_info.rasterizationSamples     = vk::SampleCountFlagBits ::e1;
        multisample_create_info.minSampleShading         = 1.0f;
        multisample_create_info.pSampleMask              = nullptr;
        multisample_create_info.alphaToCoverageEnable    = false;
        multisample_create_info.alphaToOneEnable         = false;

        vk::PipelineColorBlendAttachmentState color_blend_attachment;
        color_blend_attachment.colorWriteMask         = vk::ColorComponentFlagBits ::eR |
                                                        vk::ColorComponentFlagBits ::eG |
                                                        vk::ColorComponentFlagBits ::eB |
                                                        vk::ColorComponentFlagBits ::eA;
        color_blend_attachment.blendEnable            = true;
        color_blend_attachment.srcColorBlendFactor    = vk::BlendFactor ::eSrcAlpha;
        color_blend_attachment.dstColorBlendFactor    = vk::BlendFactor::eOneMinusSrcAlpha;
        color_blend_attachment.colorBlendOp           = vk::BlendOp ::eAdd;
        color_blend_attachment.srcAlphaBlendFactor    = vk::BlendFactor ::eOne;
        color_blend_attachment.dstAlphaBlendFactor    = vk::BlendFactor ::eZero;
        color_blend_attachment.alphaBlendOp           = vk::BlendOp ::eAdd;

        vk::PipelineColorBlendStateCreateInfo color_blend_create_info;
        color_blend_create_info.logicOpEnable        = true;
        color_blend_create_info.logicOp              = vk::LogicOp ::eCopy;
        color_blend_create_info.attachmentCount      = 1;
        color_blend_create_info.pAttachments         = &color_blend_attachment;
        color_blend_create_info.blendConstants[0]    = 0.0f;
        color_blend_create_info.blendConstants[1]    = 0.0f;
        color_blend_create_info.blendConstants[2]    = 0.0f;
        color_blend_create_info.blendConstants[3]    = 0.0f;

        std::array<vk::DynamicState, 2> dynamic_states = {
                vk::DynamicState ::eViewport,
                vk::DynamicState ::eLineWidth
        };

        vk::PipelineDynamicStateCreateInfo dynamic_state_create_info;
        dynamic_state_create_info.dynamicStateCount    = 2;
        dynamic_state_create_info.pDynamicStates       = dynamic_states.data();

        vk::PipelineLayoutCreateInfo pipeline_layout_create_info;
        pipeline_layout_create_info.setLayoutCount            = 0;
        pipeline_layout_create_info.pSetLayouts               = nullptr;
        pipeline_layout_create_info.pushConstantRangeCount    = 0;
        pipeline_layout_create_info.pPushConstantRanges       = nullptr;

        pipeline_layout = device->get_device().createPipelineLayout(pipeline_layout_create_info);

        auto shader_stages = shader.create_shader_stage_create_infos();

        vk::GraphicsPipelineCreateInfo pipeline_create_info;
        pipeline_create_info.stageCount = shader_stages.size();
        pipeline_create_info.pStages = shader_stages.data();
        pipeline_create_info.pVertexInputState = &vertex_input_info;
        pipeline_create_info.pInputAssemblyState = &input_assembly_info;
        pipeline_create_info.pViewportState = &viewport_create_info;
        pipeline_create_info.pRasterizationState = &rasterization_create_info;
        pipeline_create_info.pMultisampleState = &multisample_create_info;
        pipeline_create_info.pDepthStencilState = nullptr;
        pipeline_create_info.pColorBlendState = &color_blend_create_info;
        pipeline_create_info.pDynamicState = nullptr;
        pipeline_create_info.layout = pipeline_layout;
        pipeline_create_info.renderPass = render_pass;
        pipeline_create_info.subpass = 0;
        pipeline_create_info.basePipelineHandle = nullptr;
        pipeline_create_info.basePipelineIndex = -1;

        graphics_pipeline = device->get_device().createGraphicsPipeline(nullptr, pipeline_create_info);
    }
}