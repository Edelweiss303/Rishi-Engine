workspace "REngine";
    architecture "x64"
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


group "Dependencies"
    include "REngine/vendor/GLFW"
    include "REngine/vendor/Glad"
    include "REngine/vendor/imgui"
group ""

project "REngine"

    location "REngine"
    kind "SharedLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "repch.h"
    pchsource "REngine/src/repch.cpp"

    -- Files to include

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"


        defines
        {
            "RE_PLATFORM_WINDOWS",
            "RE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
    
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "RE_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "RE_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "RE_DIST"
        runtime "Release"
        optimize "On"

    
    
project "Sandbox"

    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

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
        "REngine/src"
    }

    links
    {
        "REngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"


        defines
        {
            "RE_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "RE_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "RE_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "RE_DIST"
        buildoptions "/MD"
        optimize "On"
