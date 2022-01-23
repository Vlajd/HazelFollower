workspace "HazelFollower"
	architecture "x64"

	startproject "Sandbox"

	configurations {

		"Debug",
		"Release",
		"Dist"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	IncludeDir = {}
	IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
	IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
	IncludeDir["ImGui"] = "Hazel/vendor/ImGui"
	IncludeDir["glm"] = "Hazel/vendor/glm"
	IncludeDir["stb"] = "Hazel/vendor/stb"

	group "Dependencies"
		include "Hazel/vendor/GLFW"
		include "Hazel/vendor/Glad"
		include "Hazel/vendor/ImGui"
	group ""

	project "Hazel"
		location "Hazel"
		kind "StaticLib"
		language "C++"
		cppdialect "C++20"
		staticruntime "on"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		pchheader "hzpch.h"
		pchsource "Hazel/src/hzpch.cpp"

		files {

			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/vendor/stb/**.cpp",
			"%{prj.name}/vendor/stb/**.h",
			"%{prj.name}/vendor/glm/glm/**.hpp",
			"%{prj.name}/vendor/glm/glm/**.inl"
		}

		defines {
			"_CRT_SECURE_NO_WARNINGS"
		}

		includedirs {

			"%{prj.name}/src",
			"%{prj.name}/vendor/spdlog/include",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.stb}"
		}

		links {

			"GLFW",
			"Glad",
			"ImGui",
			"opengl32.lib"
		}

		filter "system:windows"
			systemversion "latest"

			defines {

				"HZ_PLATFORM_WINDOWS",
				"HZ_BUILD_DLL",
				"GLFW_INCLUDE_NONE"
			}

			postbuildcommands {

				("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
			}

		filter "configurations:Debug"
			defines "HZ_DEBUG"
			runtime "Debug"
			symbols "on"
			disablewarnings "4996"

		filter "configurations:Release"
			defines "HZ_RELEASE"
			runtime "Release"
			symbols "on"

		filter "configurations:Dist"
			defines "HZ_DIST"
			runtime "Release"
			symbols "on"
			optimize "on"


	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++20"
		staticruntime "on"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files {

			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}

		includedirs {

			"Hazel/vendor/spdlog/include",
			"Hazel/vendor/ImGui",
			"Hazel/src",
			"%{IncludeDir.glm}",
		}

		links {

			"Hazel"
		}

		linkoptions { "-IGNORE:4098" }

		filter "system:windows"
			systemversion "latest"

			defines {

				"HZ_PLATFORM_WINDOWS"
			}

		filter "configurations:Debug"
			defines "HZ_DEBUG"
			symbols "on"

		filter "configurations:Release"
			defines "HZ_RELEASE"
			symbols "on"

		filter "configurations:Dist"
			defines "HZ_DIST"
			symbols "on"
			optimize "on"

