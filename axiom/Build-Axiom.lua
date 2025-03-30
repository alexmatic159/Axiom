project "Axiom"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   targetdir "binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { 
       "src/**.h", 
       "src/**.cpp",
       "../include/imgui/*.cpp",
       "../include/imgui/backends/imgui_impl_glfw.cpp",
       "../include/imgui/backends/imgui_impl_opengl3.cpp"
   }

   includedirs {
      "src",
      "../include/GLFW",
      "../include/spdlog/include",
      "../include/imgui/",
   }

   libdirs {
      "../include/GLFW/lib-vc2022",
   }

   targetdir ("../binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../binaries/intermediates/" .. OutputDir .. "/%{prj.name}")

   links { "glfw3", "opengl32" } 

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }
       buildoptions { "/utf-8" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"