cmake_minimum_required(VERSION 2.8)

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.7" CACHE STRING "Minimum OS X deployment version")
elseif(MSVC)
    set(CMAKE_SYSTEM_VERSION 8.1 CACHE TYPE INTERNAL FORCE)
endif()

if (UNIX AND NOT CMAKE_C_COMPILER)
	set(CMAKE_C_COMPILER gcc)
	set(CMAKE_CXX_COMPILER g++)
endif()
set(CMAKE_CXX_STANDARD 11)


if(MSVC)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Debug and Release Builds Configured" FORCE)
endif()

Project(sam_simulation_core)

add_subdirectory(splinter)
add_subdirectory(shared)
add_subdirectory(nlopt)
add_subdirectory(lpsolve)
add_subdirectory(solarpilot)
add_subdirectory(tcs)
add_subdirectory(ssc)

if (NOT system_advisor_model_EXPORT)
	add_subdirectory(sdktool)
	add_subdirectory(tcsconsole)
	add_subdirectory(test)
endif()