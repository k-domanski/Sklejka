workspace "Sklejka"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

-- Include directories relative to solution directory
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Sklejka/vendor/GLFW/include"

include "Sklejka/vendor/GLFW"
include "Sklejka"
include "Sandbox"