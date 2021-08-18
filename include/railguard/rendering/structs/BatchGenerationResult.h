#pragma once

#include <vector>
#include <railguard/rendering/Ids.h>

namespace railguard::rendering::structs
{
    struct BatchGenerationResult {
        std::vector<struct RenderBatch> batches;
        std::vector<model_id_t> models;
    };
} // namespace railguard::rendering::structs
