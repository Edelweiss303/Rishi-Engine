workspace "REngine";
    architecture "x86_64"
    startproject "Sandbox"
     
    configurations
    {
            "Debug",
            "Release",
            "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "REngine/vendor/GLFW/include"
IncludeDir["Glad"] = "REngine/vendor/Glad/include"
IncludeDir["ImGui"] = "REngine/vendor/imgui"
IncludeDir["glm"] = "REngine/vendor/glm"
IncludeDir["stb_image"] = "REngine/vendor/stb_image"


group "Dependencies"
    include "REngine/vendor/GLFW"
    include "REngine/vendor/Glad"
    include "REngine/vendor/imgui"
group ""

project "REngine"

    location "REngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "REpch.h"
    pchsource "REngine/src/REpch.cpp"

    -- Files to include

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
	"GLFW_INCLUDE_NONE"
    }

    includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

    filter "system:windows"
        systemversion "latest"


        defines
        {
        }

    filter "configurations:Debug"
        defines "RE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "RE_DIST"
        runtime "Release"
        optimize "on"

    
    
project "Sandbox"

    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    -- Files to include

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "REngine/vendor/spdlog/include",
        "REngine/src",
        "REngine/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "REngine"
    }

    filter "system:windows"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        defines "RE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "RE_DIST"
        runtime "Release"
        optimize "on"
