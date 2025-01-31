set(SOURCES src/query.cpp
    src/util.cpp
    src/Instance.cpp
    src/Window.cpp
    src/Surface.cpp
    src/PhysicalDevice.cpp
    src/LogicalDevice.cpp
    src/Swapchain.cpp
    src/ImageView.cpp
    src/Image.cpp
    src/GraphicsPipeline.cpp
    src/pipeline_util.cpp
    src/ShaderModule.cpp
    src/spirv.cpp
    src/RenderPass.cpp
    src/PipelineLayout.cpp
    src/Buffer.cpp
    src/Framebuffer.cpp
    src/CommandPool.cpp
    src/CommandBuffer.cpp
    src/Renderer.cpp
    src/Semaphore.cpp
    src/Fence.cpp
    src/RenderPassBuilder.cpp
    src/GraphicsPipelineBuilder.cpp
    src/Vertex.cpp)

add_library(vulkan STATIC ${SOURCES})
target_include_directories(vulkan PUBLIC include)
target_link_libraries(vulkan PRIVATE
                      Vulkan::Vulkan
                      glm::glm
                      SDL3::SDL3
                      math
                      core)