#include <railguard/rendering/init/RenderPassBuilder.h>
#include <vulkan/vulkan.h>

namespace railguard::rendering::init
{
    AttachmentBuilder AttachmentBuilder::SetFormat(VkFormat format)
    {
        _format = format;

        // Return this to be able to chain the calls
        return *this;
    }

    AttachmentBuilder AttachmentBuilder::ClearOnLoad()
    {
        _loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;

        // Return this to be able to chain the calls
        return *this;
    }
    AttachmentBuilder AttachmentBuilder::StoreAtEnd()
    {
        _storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        // Return this to be able to chain the calls
        return *this;
    }
    AttachmentBuilder AttachmentBuilder::ClearStencilOnLoad()
    {
        _stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;

        // Return this to be able to chain the calls
        return *this;
    }
    AttachmentBuilder AttachmentBuilder::StoreStencilAtEnd()
    {
        _stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;

        // Return this to be able to chain the calls
        return *this;
    }
    AttachmentBuilder AttachmentBuilder::SetFinalLayout(VkImageLayout layout)
    {
        _finalLayout = layout;

        // Return this to be able to chain the calls
        return *this;
    }

    VkAttachmentDescription AttachmentBuilder::Build()
    {
        return VkAttachmentDescription {
            .format = _format,
            // No multisampling, so 1 sample
            // TODO maybe later
            .samples        = VkSampleCountFlagBits::e1,
            .loadOp         = _loadOp,
            .storeOp        = _storeOp,
            .stencilLoadOp  = _stencilLoadOp,
            .stencilStoreOp = _stencilStoreOp,
            .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout    = _finalLayout,
        };
    }

    RenderPassBuilder RenderPassBuilder::AddColorAttachment(const VkAttachmentDescription &attachment)
    {
        _attachments.push_back(attachment);

        // Save reference to the attachment
        _attachmentReferences.push_back(VkAttachmentReference {
            .attachment = static_cast<uint32_t>(_attachments.size()) - 1,
            .layout     = VkImageLayout::eColorAttachmentOptimal,
        });

        // Only allow 1 color attachment for now max.
        _subpass.colorAttachmentCount = 1;
        _subpass.pColorAttachments    = &_attachmentReferences[_attachmentReferences.size() - 1];

        return *this;
    }

    RenderPassBuilder RenderPassBuilder::AddDepthAttachment(const VkAttachmentDescription &attachment)
    {
        _attachments.push_back(attachment);

        // Save reference to the attachment
        _attachmentReferences.push_back(VkAttachmentReference {
            .attachment = static_cast<uint32_t>(_attachments.size()) - 1,
            .layout     = VkImageLayout::eDepthStencilAttachmentOptimal,
        });

        // Only allow 1 color attachment for now max.
        _subpass.pDepthStencilAttachment = &_attachmentReferences[_attachmentReferences.size() - 1];

        return *this;
    }

    RenderPassBuilder RenderPassBuilder::SetPipelineBindPoint(VkPipelineBindPoint bindPoint)
    {
        _subpass.pipelineBindPoint = bindPoint;

        return *this;
    }

    VkRenderPass RenderPassBuilder::Build(const VkDevice &device)
    {
        VkRenderPassCreateInfo renderPassCreateInfo {
            .attachmentCount = static_cast<uint32_t>(_attachments.size()),
            .pAttachments    = _attachments.data(),
            .subpassCount    = 1,
            .pSubpasses      = &_subpass,
        };
        return device.createRenderPass(renderPassCreateInfo);
    }

} // namespace railguard::rendering::init