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
      architecture "x64"
      targetdir "bin/%{cfg.buildcfg}"
      location "build/railguard"

      -- Add header dependencies
      includedirs {
         "$(VULKAN_SDK)/Include",
         "$(SDL2_PATH)/include",
         "external/glm"
      }

      -- Add lib dependencies
      libdirs {
         "$(VULKAN_SDK)/Lib",
         "$(SDL2_PATH)/lib"
      }

      links {"SDL2", "SDL2main"}

      -- Same as above, fix directory creation bug
      os.mkdir "build/railguard/obj"

      -- Source files
	   files { "src/**.cpp", "include/**.h", "include/**.hpp" }
