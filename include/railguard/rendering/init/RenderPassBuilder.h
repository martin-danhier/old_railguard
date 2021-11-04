#include <vector>


typedef uint32_t VkFormat;
typedef int32_t VkAttachmentLoadOp;
typedef uint32_t VkAttachmentStoreOp;
typedef uint32_t VkImageLayout;
typedef uint32_t VkPipelineBindPoint;
struct VkAttachmentDescription;
struct VkAttachmentReference;
struct VkSubpassDescription;
#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
VK_DEFINE_HANDLE(VkRenderPass)
VK_DEFINE_HANDLE(VkDevice)

namespace railguard::rendering::init
{
    class AttachmentBuilder
    {
      private:
        VkFormat _format;
        VkAttachmentLoadOp _loadOp;
        VkAttachmentStoreOp _storeOp;
        VkAttachmentLoadOp _stencilLoadOp;
        VkAttachmentStoreOp _stencilStoreOp;
        VkImageLayout _finalLayout;

      public:
        AttachmentBuilder SetFormat(VkFormat format);
        AttachmentBuilder ClearOnLoad();
        AttachmentBuilder StoreAtEnd();
        AttachmentBuilder ClearStencilOnLoad();
        AttachmentBuilder StoreStencilAtEnd();
        AttachmentBuilder SetFinalLayout(VkImageLayout layout);

        [[nodiscard]] VkAttachmentDescription Build();
    };

    class RenderPassBuilder
    {
      private:
        std::vector<VkAttachmentDescription> _attachments;
        std::vector<VkAttachmentReference> _attachmentReferences;
        VkSubpassDescription *_subpass;

      public:
        RenderPassBuilder AddColorAttachment(const VkAttachmentDescription &attachment);
        RenderPassBuilder AddDepthAttachment(const VkAttachmentDescription &attachment);
        RenderPassBuilder SetPipelineBindPoint(VkPipelineBindPoint bindPoint);

        [[nodiscard]] VkRenderPass Build(const VkDevice &device);
        ~RenderPassBuilder();
    };

} // namespace railguard::rendering::init