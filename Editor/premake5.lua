project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    debugdir "%{wks.location}/SquirrelNinja"
    
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "LOG_CORE"
    }

    includedirs
    {
        "%{wks.location}/Engine/src",
        "%{wks.location}/Editor/src",
        "%{IncludeDir.irrKlang}/include",
        "%{wks.location}/Engine/vendor/spdlog/include",
        "%{wks.location}/Engine/vendor",
        "%{wks.location}/Engine/vendor/assimp/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.better_enums}/include",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.rttr}/src",
        "%{IncludeDir.json}/single_include",
    }

    links
    {
        "ImGui",
        "Engine"
    }

    postbuildcommands
    {
        "copy \"%{wks.location}lib\" \"%{cfg.buildtarget.directory}\"",
        "{COPY} \"%{wks.location}SquirrelNinja/Assets\" \"%{cfg.buildtarget.directory}Assets\""
    }


    filter "system.windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"