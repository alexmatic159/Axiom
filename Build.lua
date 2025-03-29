-- premake5.lua
workspace "AxiomSolution"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "AxiomDemo"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

include "axiom/Build-Axiom.lua"
include "demo/Build-Demo.lua"