workspace "Minecraft-Clone"
    startproject "minecraft"
    configurations { "Debug", "Release" }
    architecture "ARM64"

tdir = "bin/%{cfg.buildcfg}"
odir = "bin-int/%{cfg.buildcfg}"

-- External Dependencies --
dependencies = {}
dependencies["spdlog"] = "deps/spdlog"
dependencies["glfw"] = "deps/glfw"
dependencies["glad"] = "deps/glad"
dependencies["glm"] = "deps/glm"
dependencies["yaml"] = "deps/yaml-cpp"
dependencies["simplexnoise"] = "deps/SimplexNoise"

-- Build glad before everything else --
include "deps/glad"
include "deps/yaml-cpp"

project "minecraft"
    location "minecraft"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.cpp",
        "deps/SimplexNoise/src/simplexnoise/**.h",
        "deps/SimplexNoise/src/simplexnoise/**.cpp",
    }

    sysincludedirs
    {
        "%{prj.name}/include",
        "%{dependencies.spdlog}/include",
        "%{dependencies.glfw}/include",
        "%{dependencies.glad}/include",
        "%{dependencies.glm}/include",
        "%{dependencies.yaml}/include",
        "%{dependencies.simplexnoise}/src"
    }

    libdirs 
    {
        "%{dependencies.glfw}/lib-arm64"
    }
    links { "glfw3", "glad", "yaml-cpp" }

    filter { "system:windows", "configurations:*"}
        systemversion "latest"

        defines
        {
            "MINECRAFT_PLATFORM_WINDOWS"
        }

    filter { "system:macosx", "configurations:*"}
        defines
        {
            "MINECRAFT_PLATFORM_MAC"
        }

        links { "Cocoa.framework", "OpenGL.framework", "IOKit.framework" }

    filter { "configurations:Debug" }
        defines
        {
            "MINECRAFT_CONFIG_DEBUG"
        }
        runtime "Debug"
        symbols "on"
        
    filter { "configurations:Release" }
        defines
        {
            "MINECRAFT_CONFIG_RELEASE"
        }
        runtime "Release"
        symbols "off"
        optimize "on"