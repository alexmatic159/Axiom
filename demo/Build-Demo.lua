project "AxiomDemo"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }
 
   includedirs { 
       "src",
       "../axiom/src", 
       "../include/GLFW",
       "../include/spdlog/include",
       "../include/imgui/",
       "../include/json/include",
       "../include/openssl/lib/x64/Release/include"
   }  

   targetdir ("../binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../binaries/intermediates/" .. OutputDir .. "/%{prj.name}")

   links { "Axiom" }
   
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