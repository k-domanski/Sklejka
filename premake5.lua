workspace "Sklejka"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

include "Sklejka"
include "Sandbox"