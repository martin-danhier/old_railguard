
workspace "railguard"
   configurations { "Debug", "Release" }
   platforms {"Win64", "Linux"}

   -- Place cmake and others builds in different directories so clion and vscode dont mix
   filter "action:cmake"
      location "cmakebuild"
   filter "action:not cmake"
      location "build"
   filter {}

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

----------------------------------
   -- We use os.getenv below because the cmake export does not support the premake env variables
   -- Thus we get them as we would normally do in Lua

   -- Find SDL2
   sdl_include_dir = os.findheader(
     "SDL.h", {
        "$(SDL2_PATH)/include/SDL2",
        "/usr/include",
        "/usr/include/SDL2",
        "$(sdl2_image_DIR)/include"
     })
   sdl_lib_dir = os.findlib(
        "SDL2", {
           "$(SDL2_PATH)/lib",
           "/usr/lib/x86_64-linux-gnu/",
           "$(sdl2_image_DIR)/lib",
           "/usr/lib64",
           "/usr/lib64/cmake/SDL2"
   })
   if (sdl_include_dir == nil) then
      print("Using default value for SDL")
      if not os.getenv('SDL2_PATH') then
        error("SDL2 must be installed and pointed by SDL2_PATH")
      end

      sdl_include_dir = os.getenv('SDL2_PATH') .. "/include"

   end
   if (sdl_include_dir == nil or sdl_lib_dir == nil) then
      error("\n--> SDL2 must be installed.\n")
   end

   -- Find Vulkan
   if os.getenv("VULKAN_SDK") then
      vulkan_include_dir = os.getenv("VULKAN_SDK") .. "/include"
      vulkan_lib_dir = os.getenv("VULKAN_SDK") .. "/lib"
      vulkan_glslang_validator = os.getenv("VULKAN_SDK") .. "/bin/glslangValidator"
   else
      vulkan_include_dir = os.findheader(
        "vulkan/vulkan.h", {
          '/usr/include'
        }
      )
      vulkan_lib_dir = os.findlib(
         "vulkan", {
            '/usr/lib/x86_64-linux-gnu'
         }
      )
      vulkan_glslang_validator = 'glslangValidator'
   end

   if not vulkan_include_dir then
      error("Can't find Vulkan headers")
   end
   if not vulkan_lib_dir then
      error("Can't find Vulkan lib")
   end
   if not vulkan_glslang_validator then
      error("Can't find glslangValidator")
   end

---------------------------------
   project "vkbootstrap"
      kind "StaticLib"
      language "C++"
      architecture "x64"

      -- Place cmake and others builds in different directories so clion and vscode dont mix
      filter "action:cmake"
         location "cmakebuild/vkbootstrap"
      filter "action:not cmake"
         location "build/vkbootstrap"
      filter {}

      files {"./external/vk-bootstrap/src/**.cpp", "./external/vk-bootstrap/src/**.h"}
      includedirs {vulkan_include_dir}
      libdirs {vulkan_lib_dir}

   project "shaders"
      kind "Utility"

      os.mkdir "bin/shaders"

       -- Place cmake and others builds in different directories so clion and vscode dont mix
      filter "action:cmake"
         location "cmakebuild/shaders"
      filter "action:not cmake"
         location "build/shaders"
      filter {}

      -- Build shaders with glslangValidator
      filter {"files:**"}
      buildcommands { '"' .. vulkan_glslang_validator .. '"' ..  ' -V "%{file.relpath}" -o "../../bin/shaders/%{file.name}.spv"' }
      buildoutputs {"bin/shaders/%{file.name}.spv"}
      filter {}
      -- Take all shader files
      files { "shaders/**.vert", "shaders/**.frag", "shaders/**.glsl", "shaders/**.comp" }

   -- Main Project
   project "railguard"
      kind "ConsoleApp"
      language "C++"
      -- cppdialect "C++20"
      dependson "shaders"

      -- Place cmake and others builds in different directories so clion and vscode dont mix
      filter "action:cmake"
         location "cmakebuild/railguard"
         os.mkdir "cmakebuild/railguard/obj"
      filter "action:not cmake"
         -- Specify c++ version when not using cmake
         buildoptions(iif(os.istarget("windows"), "/std:c++latest", "--std=c++20"))
         location "build/railguard"
         
         -- Same as above, fix directory creation bug
         os.mkdir "build/railguard/obj"
      filter {}
      
      architecture "x64"
      targetdir "bin/%{cfg.buildcfg}"

         -- Add header dependencies
         includedirs {
            vulkan_include_dir,
            sdl_include_dir,
            "external/glm",
            "external/vk-bootstrap/src",
            "external/vma",
            "include"
         }
         
         
      -- Add lib dependencies
      libdirs {
         vulkan_lib_dir,
         sdl_lib_dir
      }

      links {"SDL2", "vkbootstrap"}

      -- Only link SDL2main on windows
      filter {"platforms:Win64"}
         links { "SDL2main"}
         buildoptions("/EHsc")

      -- Only link dl on Linux
      filter {"platforms:Linux"}
         links {"dl"}
      -- Reset filter
      filter{}


      -- Source files
      -- files { "include/**.h" }
	   files { "src/**.cpp" }

