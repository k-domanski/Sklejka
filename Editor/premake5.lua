project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    defines
    {
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "%{wks.location}/Engine/src",
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

    }

    links
    {
        "Engine"
    }

    postbuildcommands
    {
        ("copy \"%{wks.location}lib\" \"%{cfg.buildtarget.directory}\"")
    }

    filter "system.windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"