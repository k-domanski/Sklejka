workspace "Sklejka"
    architecture "x64"
    startproject "Editor"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

-- Include directories relative to solution directory
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Engine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Engine/vendor/Glad/include"
IncludeDir["glm"] = "%{wks.location}/Engine/vendor/glm"
IncludeDir["ImGui"] = "%{wks.location}/Engine/vendor/imgui"
IncludeDir["stb_image"] = "%{wks.location}/Engine/vendor/stb_image"
IncludeDir["assimp"] = "%{wks.location}/Engine/vendor/assimp"
IncludeDir["better_enums"] = "%{wks.location}/Engine/vendor/better-enums"
IncludeDir["irrKlang"] = "%{wks.location}/Engine/vendor/irrKlang"
IncludeDir["rttr"] = "%{wks.location}/Engine/vendor/rttr"

group "Dependencies"
    include "Engine/vendor/GLFW"
    include "Engine/vendor/Glad"
    include "Engine/vendor/imgui"
    include "Engine/vendor/rttr"
group ""

include "Engine"
include "Editor"
include "Game"