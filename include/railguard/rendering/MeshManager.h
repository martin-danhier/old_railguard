#pragma once

#include <railguard/core/StandaloneManager.h>
#include <railguard/rendering/Ids.h>
#include <railguard/rendering/structs/Vertex.h>

namespace railguard::rendering

{
    class MeshManager : public core::StandaloneManager<mesh_id_t>
    {
      private:
        // Typedef the parent type to make it easier to call from the methods
        typedef core::StandaloneManager<mesh_id_t> super;

        std::vector<std::vector<structs::Vertex>> _vertices;

      public:
    };
} // namespace railguard::rendering
