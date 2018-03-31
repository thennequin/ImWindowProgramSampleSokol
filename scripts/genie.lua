-- Just change solution/project name and project GUID

local PROJECT_DIR          = (path.getabsolute("..") .. "/")
local PROJECT_BUILD_DIR    = path.join(PROJECT_DIR, ".build/")
local PROJECT_PROJECTS_DIR = path.join(PROJECT_DIR, ".projects")
local PROJECT_RUNTIME_DIR  = path.join(PROJECT_DIR, "Output/")

solution "Program"
	language				"C++"
	configurations			{ "Debug", "Release" }
	platforms				{ "x32", "x64" }

	location				(path.join(PROJECT_PROJECTS_DIR, _ACTION))
	objdir					(path.join(PROJECT_BUILD_DIR, _ACTION))

	startproject "Program"
	project "Program"
		uuid				"e0ba3c4d-338b-4517-8bbd-b29311fd6830"
		kind				"WindowedApp"
		targetdir			(PROJECT_RUNTIME_DIR)

		files {
							"../src/**.cpp",
							"../src/**.h",

							"../Externals/ImWindow/Externals/imgui/imconfig.h",
							"../Externals/ImWindow/Externals/imgui/imgui.h",
							"../Externals/ImWindow/Externals/imgui/imgui_internal.h",
							"../Externals/ImWindow/Externals/imgui/imgui.cpp",
							"../Externals/ImWindow/Externals/imgui/imgui_draw.cpp",
							"../Externals/ImWindow/Externals/imgui/SFF_rect_pack.h",
							"../Externals/ImWindow/Externals/imgui/SFF_textedit.h",
							"../Externals/ImWindow/Externals/imgui/SFF_truetype.h",
							
							"../Externals/ImWindow/Externals/EasyWindow/EasyWindow*.cpp",
							"../Externals/ImWindow/Externals/EasyWindow/EasyWindow*.h",

							"../Externals/ImWindow/Externals/flextGL/flextGL.c",
							"../Externals/ImWindow/Externals/flextGL/flextGL.h",

							"../Externals/ImWindow/ImWindow/**.cpp",
							"../Externals/ImWindow/ImWindow/**.h",

							"../Externals/ImWindow/ImWindowEasyWindow/**.cpp",
							"../Externals/ImWindow/ImWindowEasyWindow/**.h",

							"../Externals/ImWindow/ImWindowSokol/Imw**.h",
							"../Externals/ImWindow/ImWindowSokol/Imw**.cpp",
		}
		
		vpaths {
							["ImGui"] = "../Externals/ImWindow/Externals/imgui/**",
							["EasyWindow"] = "../Externals/ImWindow/Externals/EasyWindow/**",
							["ImWindow"] = "../Externals/ImWindow/ImWindow/**",
							["flextGL"] = "../Externals/ImWindow/Externals/flextGL/**",
							["sokol"] = "../Externals/ImWindow/Externals/sokol/**",
							["ImWindowEasyWindow"] = "../Externals/ImWindow/ImWindowEasyWindow/**",
							["ImWindowSokol"] = "../Externals/ImWindow/ImWindowSokol/**"
		}
		
		includedirs {
							"../Externals/ImWindow/Externals/imgui",
							"../Externals/ImWindow/Externals/EasyWindow",
							"../Externals/ImWindow/Externals/flextGL",
							"../Externals/ImWindow/Externals/sokol",
							"../Externals/ImWindow/ImWindow",
							"../Externals/ImWindow/ImWindowEasyWindow",
							"../Externals/ImWindow/ImWindowSokol",
							"../src/"
		}

		links {
							"OpenGL32",
							"glu32"
		}
	
		configuration		"Debug"
			targetsuffix	"_d"
			flags			{ "Symbols" }
			
		configuration		"Release"
			targetsuffix	"_r"
			flags			{ "Optimize" }

		configuration {}
