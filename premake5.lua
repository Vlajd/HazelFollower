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

	group "Dependencies"
		include "Hazel/vendor/GLFW"
		include "Hazel/vendor/Glad"
		include "Hazel/vendor/ImGui"
	group ""

	project "Hazel"
		location "Hazel"
		kind "SharedLib"
		language "C++"
		staticruntime "off"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		pchheader "hzpch.h"
		pchsource "Hazel/src/hzpch.cpp"

		files {

			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			-- "%{prj.name}/vendor/ImGui/imgui_tables.cpp"
		}

		includedirs {

			"%{prj.name}/src",
			"%{prj.name}/vendor/spdlog/include",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.ImGui}"
		}

		links {

			"GLFW",
			"Glad",
			"ImGui",
			"opengl32.lib"
		}

		filter "system:windows"
			cppdialect "C++20"
			systemversion "latest"

			defines {

				"HZ_PLATFORM_WINDOWS",
				"HZ_BUILD_DLL",
				"_WINDLL",
				"IMGUI_IMPL_OPENGL_LOADER_CUSTOM",
				"GLFW_INCLUDE_NONE"
			}

			postbuildcommands {

				("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
			}

		filter "configurations:Debug"
			defines "HZ_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "HZ_RELEASE"
			runtime "Release"
			symbols "On"

		filter "configurations:Dist"
			defines "HZ_DIST"
			runtime "Release"
			symbols "On"
			optimize "On"

	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"
		staticruntime "off"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files {

			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}

		includedirs {

			"Hazel/vendor/spdlog/include",
			"Hazel/src"
		}

		links {

			"Hazel"
		}

		filter "system:windows"
			cppdialect "C++20"
			systemversion "latest"

			defines {

				"HZ_PLATFORM_WINDOWS"
			}

		filter "configurations:Debug"
			defines "HZ_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "HZ_RELEASE"
			buildoptions "/MD"
			symbols "On"

		filter "configurations:Dist"
			defines "HZ_DIST"
			buildoptions "/MD"
			symbols "On"
			optimize "On"
