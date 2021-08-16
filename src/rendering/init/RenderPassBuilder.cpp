#include "railguard/rendering/init/RenderPassBuilder.h"

namespace railguard::rendering::init
{
    AttachmentBuilder AttachmentBuilder::SetFormat(vk::Format format)
    {
        _format = format;

        // Return this to be able to chain the calls
        return *this;
    }

    AttachmentBuilder AttachmentBuilder::ClearOnLoad()
    {
        _loadOp = vk::AttachmentLoadOp::eClear;

        // Return this to be able to chain the calls
        return *this;
    }
    AttachmentBuilder AttachmentBuilder::StoreAtEnd()
    {
        _storeOp = vk::AttachmentStoreOp::eStore;

        // Return this to be able to chain the calls
        return *this;
    }
    AttachmentBuilder AttachmentBuilder::ClearStencilOnLoad()
    {
        _stencilLoadOp = vk::AttachmentLoadOp::eClear;

        // Return this to be able to chain the calls
        return *this;
    }
    AttachmentBuilder AttachmentBuilder::StoreStencilAtEnd()
    {
        _stencilStoreOp = vk::AttachmentStoreOp::eStore;

        // Return this to be able to chain the calls
        return *this;
    }
    AttachmentBuilder AttachmentBuilder::SetFinalLayout(vk::ImageLayout layout)
    {
        _finalLayout = layout;

        // Return this to be able to chain the calls
        return *this;
    }

    vk::AttachmentDescription AttachmentBuilder::Build()
    {
        return vk::AttachmentDescription {
            .format = _format,
            // No multisampling, so 1 sample
            // TODO maybe later
            .samples        = vk::SampleCountFlagBits::e1,
            .loadOp         = _loadOp,
            .storeOp        = _storeOp,
            .stencilLoadOp  = _stencilLoadOp,
            .stencilStoreOp = _stencilStoreOp,
            .initialLayout  = vk::ImageLayout::eUndefined,
            .finalLayout    = _finalLayout,
        };
    }

    RenderPassBuilder RenderPassBuilder::AddColorAttachment(const vk::AttachmentDescription &attachment)
    {
        _attachments.push_back(attachment);

        // Save reference to the attachment
        _attachmentReferences.push_back(vk::AttachmentReference {
            .attachment = static_cast<uint32_t>(_attachments.size()) - 1,
            .layout     = vk::ImageLayout::eColorAttachmentOptimal,
        });

        // Only allow 1 color attachment for now max.
        _subpass.colorAttachmentCount = 1;
        _subpass.pColorAttachments    = &_attachmentReferences[_attachmentReferences.size() - 1];

        return *this;
    }

    RenderPassBuilder RenderPassBuilder::AddDepthAttachment(const vk::AttachmentDescription &attachment)
    {
        _attachments.push_back(attachment);

        // Save reference to the attachment
        _attachmentReferences.push_back(vk::AttachmentReference {
            .attachment = static_cast<uint32_t>(_attachments.size()) - 1,
            .layout     = vk::ImageLayout::eDepthStencilAttachmentOptimal,
        });

        // Only allow 1 color attachment for now max.
        _subpass.pDepthStencilAttachment = &_attachmentReferences[_attachmentReferences.size() - 1];

        return *this;
    }

    RenderPassBuilder RenderPassBuilder::SetPipelineBindPoint(vk::PipelineBindPoint bindPoint)
    {
        _subpass.pipelineBindPoint = bindPoint;

        return *this;
    }

    vk::RenderPass RenderPassBuilder::Build(const vk::Device &device)
    {
        vk::RenderPassCreateInfo renderPassCreateInfo {
            .attachmentCount = static_cast<uint32_t>(_attachments.size()),
            .pAttachments    = _attachments.data(),
            .subpassCount    = 1,
            .pSubpasses      = &_subpass,
        };
        return device.createRenderPass(renderPassCreateInfo);
    }

} // namespace railguard::rendering::init