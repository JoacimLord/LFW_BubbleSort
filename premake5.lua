include "Dependencies.lua"

workspace "LFW_BubbleSort"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Luna/Vendor/GLFW"
	include "Luna/Vendor/Glad"
	include "Luna/Vendor/imgui"
group ""

include "Luna"
include "Sandbox"