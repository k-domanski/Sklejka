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
IncludeDir["Glad"] = "%{wks.location}/Sklejka/vendor/Glad/include"
IncludeDir["glm"] = "%{wks.location}/Sklejka/vendor/glm"
IncludeDir["ImGui"] = "%{wks.location}/Sklejka/vendor/imgui"

include "Sklejka/vendor/GLFW"
include "Sklejka/vendor/Glad"
include "Sklejka/vendor/imgui"

include "Sklejka"
include "Sandbox"