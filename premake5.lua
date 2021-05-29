workspace "railguard"
   configurations { "Debug", "Release" }
   location "build"
   -- Fix a bug on windows where this directory would not be created
   os.mkdir "./bin"

   -- Debug config
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
   -- Release config
   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

---------------------------------

   -- Main Project
   project "railguard"
      kind "ConsoleApp"
      language "C++"
      buildoptions "--std=c++20"
      architecture "x64"
      targetdir "bin/%{cfg.buildcfg}"
      location "build/railguard"

      -- Find SDL2
      sdl_include_dir = os.findheader(
         "SDL.h", {
            "$(SDL2_PATH)/include/SDL2",
            "/usr/include/SDL2",
            "$(sdl2_image_DIR)/include/SDL2"
         })
      sdl_lib_dir = os.findlib(
         "SDL2", {
            "$(SDL2_PATH)/lib",
            "/usr/lib/x86_64-linux-gnu/",
            "$(sdl2_image_DIR)/lib",
            "/usr/lib64"
         }
      )
      if (sdl_include_dir == nil or sdl_lib_dir == nil) then
         error("\n--> SDL2 must be installed.\n")
      end

      -- Add header dependencies
      includedirs {
         "$(VULKAN_SDK)/Include",
         sdl_include_dir,
         "external/glm"
      }

      -- Add lib dependencies
      libdirs {
         "$(VULKAN_SDK)/Lib",
         sdl_lib_dir
      }

      links {"SDL2"}
      -- Only link SDL2main on windows
      if (os.is("windows")) then
         links { "SDL2main"}
      end

      -- Same as above, fix directory creation bug
      os.mkdir "build/railguard/obj"

      -- Source files
	   files { "src/**.cpp", "include/**.h", "include/**.hpp" }
