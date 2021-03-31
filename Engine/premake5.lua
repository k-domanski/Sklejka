project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "src/pch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/glm/glm/**.hpp",
        "vendor/glm/glm/**.inl",
		"vendor/stb_image/**.h",
        "vendor/stb_image/**.cpp",
        "vendor/assimp/include/assimp/**",
        "vendor/better-enums/include/**",
        "vendor/irrKlang/include/**",
        "vendor/freetype/include/**"
    }

    defines
    {
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "src",
        "vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.assimp}/include",
        "%{IncludeDir.better_enums}/include",
        "%{IncludeDir.irrKlang}/include",
        "%{IncludeDir.rttr}/src",
        "%{IncludeDir.freetype}/include"
    }
    libdirs
    {
        "%{IncludeDir.assimp}/lib",
        "%{IncludeDir.irrKlang}/lib",
        "%{IncludeDir.freetype}/lib"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "rttr",
        "opengl32.lib",
        "assimp-vc142-mtd.lib",
        "irrKlang.lib",
        "freetype.lib"
    }
    
    filter "system.windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"