cmake_minimum_required (VERSION 3.10)

#####################################################
## Variables to be set depending on project
#####################################################
set (PROJECTNAME "QuasarRush") 

set (ENABLE_WEB OFF CACHE BOOL "enable web build using SDL and em++")
set (ENABLE_DISPLAY_TESTS ON CACHE BOOL "enable unittests that require a display")
# Apple users: set to /usr/local/Cellar/sfml/2.4.2_1/lib/ or respectively
if (WIN32)
	set (SFML_DIR_ROOT "${CMAKE_SOURCE_DIR}/ext/SFML-2.5.1/" CACHE PATH "Path to SFML root dir.")
endif()

#####################################################
## Helper Functions
#####################################################
function (jt_use_assets TGT)
	add_custom_command(TARGET ${TGT} PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets)
		
	if(MSVC)
		add_custom_command(TARGET ${TGT} PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/Debug/assets)
		add_custom_command(TARGET ${TGT} PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/Release/assets)
	endif()
endfunction()

function (jt_deploy_dlls DESTFOLDER)
	message (STATUS "Deploy to ${DESTFOLDER}")

	if (WIN32)
		file(MAKE_DIRECTORY ${DESTFOLDER})
		# copy dlls

		if ($ENV{CLION_IDE})
			configure_file(${SFML_DIR_ROOT}/bin/sfml-system-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-window-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-graphics-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-audio-2.dll ${DESTFOLDER} COPYONLY)

			configure_file(${SFML_DIR_ROOT}/bin/sfml-system-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-window-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-graphics-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-audio-d-2.dll ${DESTFOLDER} COPYONLY)
		elseif(MSVC)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-system-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-window-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-graphics-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-audio-2.dll ${DESTFOLDER} COPYONLY)

			configure_file(${SFML_DIR_ROOT}/bin/sfml-system-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-window-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-graphics-d-2.dll ${DESTFOLDER} COPYONLY)
			configure_file(${SFML_DIR_ROOT}/bin/sfml-audio-d-2.dll ${DESTFOLDER} COPYONLY)

		endif()
	endif()
endfunction()

if ($ENV{CLION_IDE})
    set (SFML_DIR "${SFML_DIR_ROOT}/lib/cmake/SFML")
    find_package(SFML COMPONENTS graphics window system audio REQUIRED)
endif ()

#####################################################
## Create Project file and link directories
#####################################################
project (${PROJECTNAME})
if(NOT ENABLE_WEB)
	if (WIN32 OR APPLE)
		link_directories(${SFML_DIR_ROOT}/lib)
	endif()
endif()

#####################################################
## Other variables and compiler setup
#####################################################
set (CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_STANDARD 17)
set (CXX_STANDARD 17)
add_compile_definitions(DISABLE_CPP17_FILESYSTEM)

if(ENABLE_WEB)
	add_compile_definitions(MUSIC_OGG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2 --std=c++17 -fpermissive ")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --use-preload-plugins --preload-file assets")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"bmp\",\"png\"]'")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_MIXER=2 -s USE_VORBIS=1 -s USE_OGG=1")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_TTF=2 -s USE_PTHREADS=1")
	add_definitions(-DENABLE_WEB)
else()
	if(MSVC)
		## nasty warning 5205 from tileson should not show up,
		## so the warning level of this one is set to 4 and the global warning level is set to three
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w45205 /W3 /EHsc")
	else()
		if (ENABLE_COVERAGE)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -O3 -fprofile-arcs -ftest-coverage  --std=c++17 -fpermissive -lstdc++fs")
		else ()
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 --std=c++17 -fpermissive -lstdc++fs")
		endif()
	endif()
endif()

#####################################################
## Setup Box2d
#####################################################
file(GLOB_RECURSE BOX2DFILES ext/box2d/Box2D/*)
add_library(Box2D ${BOX2DFILES})
target_include_directories(Box2D SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext/box2d)

#####################################################
## setup JamTemplate
#####################################################

file(GLOB_RECURSE JAMTEMPLATEBASEFILES ${CMAKE_CURRENT_SOURCE_DIR}/src/jamtemplate/common/*)
if(ENABLE_WEB)
	file(GLOB_RECURSE JAMTEMPLATEFILES ${CMAKE_CURRENT_SOURCE_DIR}/src/jamtemplate/sdl/*)
else()
	file(GLOB_RECURSE JAMTEMPLATEFILES ${CMAKE_CURRENT_SOURCE_DIR}/src/jamtemplate/sfml/*)
endif()

add_library(JamTemplate ${JAMTEMPLATEFILES} ${JAMTEMPLATEBASEFILES})

# add public includes, so they can be used by dependent targets
target_include_directories(JamTemplate PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/jamtemplate/common)
target_include_directories(JamTemplate SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext/tileson-master/include)
if(ENABLE_WEB)
	target_include_directories(JamTemplate PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/jamtemplate/sdl)
else()
	target_include_directories(JamTemplate PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/jamtemplate/sfml)
	target_include_directories(JamTemplate SYSTEM PUBLIC ${SFML_DIR_ROOT}/include)
endif()

if(NOT ENABLE_WEB)
	# group together similar files for a cleaner JamTemplate Project in IDEs that support this feature
	source_group(
		GameLoop FILES
		src/jamtemplate/common/game_interface.hpp

		src/jamtemplate/common/game_loop_interface.hpp

		src/jamtemplate/common/game_base.cpp
		src/jamtemplate/common/game_base.hpp

		src/jamtemplate/common/game_object.cpp
		src/jamtemplate/common/game_object.hpp

		src/jamtemplate/common/game_state.cpp
		src/jamtemplate/common/game_state.hpp

		src/jamtemplate/sfml/game.cpp
		src/jamtemplate/sfml/game.hpp
	)
	source_group(
		Drawables FILES
		src/jamtemplate/common/animation.cpp
		src/jamtemplate/common/animation.hpp

		src/jamtemplate/common/bar.cpp
		src/jamtemplate/common/bar.hpp

		src/jamtemplate/common/drawable_impl.cpp
		src/jamtemplate/common/drawable_impl.hpp
		
		src/jamtemplate/common/drawable_interface.hpp

		src/jamtemplate/common/drawable_helpers.cpp
		src/jamtemplate/common/drawable_helpers.hpp

		src/jamtemplate/sfml/shape.cpp
		src/jamtemplate/sfml/shape.hpp

		src/jamtemplate/sfml/sprite.cpp
		src/jamtemplate/sfml/sprite.hpp

		src/jamtemplate/sfml/text.cpp
		src/jamtemplate/sfml/text.hpp

		src/jamtemplate/common/tilemap.hpp
		src/jamtemplate/common/tilemap.cpp
	)
	source_group(Tweens REGULAR_EXPRESSION src/jamtemplate/common/tween*)

	target_link_libraries(JamTemplate optimized sfml-system )
	target_link_libraries(JamTemplate optimized sfml-window )
	target_link_libraries(JamTemplate optimized sfml-graphics )
	target_link_libraries(JamTemplate optimized sfml-audio )

	if (WIN32)
		target_link_libraries(JamTemplate debug sfml-system-d )
		target_link_libraries(JamTemplate debug sfml-window-d )
		target_link_libraries(JamTemplate debug sfml-graphics-d )
		target_link_libraries(JamTemplate debug sfml-audio-d )
	endif()
endif()

target_link_libraries(JamTemplate Box2D)

#####################################################
## setup Game executable
#####################################################
file(GLOB GAMEFILES src/game/*)
file(GLOB HUDFILES src/game/hud/*)
message("${HUDFILES}")
add_executable(${PROJECTNAME} ${GAMEFILES} ${HUDFILES})
if(ENABLE_WEB)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

target_link_libraries(${PROJECTNAME} JamTemplate)
target_include_directories(${PROJECTNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/game/)
target_include_directories(${PROJECTNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext/tileson-master/include/external)

jt_use_assets(${PROJECTNAME})

#####################################################
## copy sfml dlls and .clang-format around
#####################################################
if (NOT ENABLE_WEB)
	jt_deploy_dlls(${CMAKE_BINARY_DIR})
	if(MSVC)
		jt_deploy_dlls(${CMAKE_BINARY_DIR})
    endif()
endif()
configure_file(${CMAKE_SOURCE_DIR}/.clang-format ${CMAKE_BINARY_DIR}/ COPYONLY)
