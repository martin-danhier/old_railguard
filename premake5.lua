
workspace "railguard"
   configurations { "Debug", "Release" }
   platforms {"Win64", "Linux"}
   location "build"
   -- Fix a bug on windows where this directory would not be created
   os.mkdir "./bin"

   -- Debug config
   filter "configurations:Debug"
      defines { "DEBUG", "USE_ADVANCED_CHECKS", "USE_VK_VALIDATION_LAYERS" }
      symbols "On"
   -- Release config
   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
   -- Windows platform
   filter "platforms:Win64"
      system "windows"
   -- Linux platform
   filter "platforms:Linux"
      system "linux"
   filter {}

---------------------------------
   project "vkbootstrap"
      kind "StaticLib"
      language "C++"
      architecture "x64"
      location "build/vkbootstrap"
      files {"./external/vk-bootstrap/src/**.cpp", "./external/vk-bootstrap/src/**.h"}
      includedirs {"$(VULKAN_SDK)/include"}
      libdirs {"$(VULKAN_SDK)/Lib"}

   -- Main Project
   project "railguard"
      kind "ConsoleApp"
      language "C++"
      buildoptions(iif(os.istarget("windows"), "/std:c++latest", "--std=c++20"))
      architecture "x64"
      targetdir "bin/%{cfg.buildcfg}"
      location "build/railguard"
      
      -- Find SDL2
      sdl_include_dir = os.findheader(
         "SDL.h", {
            "$(SDL2_PATH)/include/SDL2",
            "/usr/include",
            "$(sdl2_image_DIR)/include"
         })
         sdl_lib_dir = os.findlib(
            "SDL2", {
               "$(SDL2_PATH)/lib",
               "/usr/lib/x86_64-linux-gnu/",
               "$(sdl2_image_DIR)/lib",
               "/usr/lib64"
            }
         )
         if (sdl_include_dir == nil) then
            print("Using default value for SDL")
            sdl_include_dir = "$(SDL2_PATH)/include"
         end
         if (sdl_include_dir == nil or sdl_lib_dir == nil) then
            error("\n--> SDL2 must be installed.\n")
         end
         
         -- Add header dependencies
         includedirs {
            "$(VULKAN_SDK)/include",
            sdl_include_dir,
            "external/glm",
            "external/vk-bootstrap/src",
            "external/vma"
         }
         
         
      -- Add lib dependencies
      libdirs {
         "$(VULKAN_SDK)/Lib",
         sdl_lib_dir
      }

      links {"SDL2", "vkbootstrap"}

      -- Only link SDL2main on windows
      filter {"platforms:Win64"}
         links { "SDL2main"}

      -- Only link dl on Linux
      filter {"platforms:Linux"}
         links {"dl"}
      -- Reset filter
      filter{}

      -- Same as above, fix directory creation bug
      os.mkdir "build/railguard/obj"

      -- Source files
      files { "include/**.h" }
	   files { "src/**.cpp" }
