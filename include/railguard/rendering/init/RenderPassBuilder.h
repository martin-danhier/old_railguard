#include "railguard/includes/Vulkan.h"

namespace railguard::rendering::init
{
    class AttachmentBuilder
    {
    private:
        vk::Format _format = vk::Format::eUndefined;
        vk::AttachmentLoadOp _loadOp = vk::AttachmentLoadOp::eDontCare;
        vk::AttachmentStoreOp _storeOp = vk::AttachmentStoreOp::eDontCare;
        vk::AttachmentLoadOp _stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        vk::AttachmentStoreOp _stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        vk::ImageLayout _finalLayout = vk::ImageLayout::eUndefined;

    public:
        AttachmentBuilder SetFormat(vk::Format format);
        AttachmentBuilder ClearOnLoad();
        AttachmentBuilder StoreAtEnd();
        AttachmentBuilder ClearStencilOnLoad();
        AttachmentBuilder StoreStencilAtEnd();
        AttachmentBuilder SetFinalLayout(vk::ImageLayout layout);

        [[nodiscard]] vk::AttachmentDescription Build();
    };

    class RenderPassBuilder
    {
    private:
        std::vector<vk::AttachmentDescription> _attachments;
        std::vector<vk::AttachmentReference> _attachmentReferences;
        vk::SubpassDescription _subpass;
    public:
        RenderPassBuilder AddColorAttachment(const vk::AttachmentDescription &attachment);
        RenderPassBuilder AddDepthAttachment(const vk::AttachmentDescription &attachment);
        RenderPassBuilder SetPipelineBindPoint(vk::PipelineBindPoint bindPoint);

        [[nodiscard]] vk::RenderPass Build(const vk::Device &device);
    };

}