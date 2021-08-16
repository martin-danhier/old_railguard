#pragma once

// Various hardcoded constants used in the renderer. They are regrouped here for easy editing.

// Some files also define constants, most of the time they are constants that only relate to that file in particular and shouldn't be
// accessible elsewhere.

#define NB_OVERLAPPING_FRAMES   3
#define SWAPCHAIN_FORMAT        VK_FORMAT_B8G8R8A8_UNORM
#define SWAPCHAIN_COLOR_SPACE   VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
#define WAIT_FOR_FENCES_TIMEOUT 1000000000
#define SEMAPHORE_TIMEOUT       1000000000
