-- premake5.lua
workspace "SchwachMatt"
   configurations { "Debug", "Release" }

project "SchwachMatt"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "build/server/%{cfg.buildcfg}"

   buildoptions {
      "-gdwarf-3"
   }

   files { 
      "src/**.hpp",
      "src/**.cpp",
   }

   includedirs {
      "external/spdlog",
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"