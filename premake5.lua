-- premake5.lua
workspace "SchwachMatt"
   configurations { "Debug", "Release" }

project "SchwachMatt"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "build/server/%{cfg.buildcfg}"

   files { 
      "src/**.hpp",
      "src/**.cpp",
   }

   includedirs {
      "external/boost_1_77_0"
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "Client"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "build/client/%{cfg.buildcfg}"

   files { 
      "client/**.hpp",
      "client/**.cpp",
   }

   includedirs {
      "external/boost_1_77_0"
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"