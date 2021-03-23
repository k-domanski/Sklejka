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
IncludeDir["stb_image"] = "%{wks.location}/Sklejka/vendor/stb_image"
IncludeDir["assimp"] = "%{wks.location}/Sklejka/vendor/assimp"
IncludeDir["better_enums"] = "%{wks.location}/Sklejka/vendor/better-enums"
IncludeDir["irrKlang"] = "%{wks.location}/Sklejka/vendor/irrKlang"

group "Dependencies"
    include "Sklejka/vendor/GLFW"
    include "Sklejka/vendor/Glad"
    include "Sklejka/vendor/imgui"
group ""

include "Sklejka"
include "Sandbox"