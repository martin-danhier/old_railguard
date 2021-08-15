#pragma once

#include <glm/glm.hpp>

namespace railguard::rendering::structs
{

    /**
     * @brief A single vertex in a mesh.
     */
    struct Vertex
    {
        /**
         * @brief Position of the vertex relatively to the mesh's origin.
         */
        glm::vec3 position;
        /**
         * @brief Normal vectors for this vertex. Used for lighting.
         */
        glm::vec3 normal;
        /**
         * @brief UV coordinates for this vertex. Used for texturing.
         */
        glm::vec2 uv;

        static struct VertexInputDescription GetVertexDescription();
    };

} // namespace railguard::rendering
