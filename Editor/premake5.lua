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

    includedirs
    {
        "%{wks.location}/Engine/src",
        "%{IncludeDir.irrKlang}/include",
        "%{wks.location}/Engine/vendor/spdlog/include",
        "%{wks.location}/Engine/vendor",
        "%{IncludeDir.GLFW}"
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