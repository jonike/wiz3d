#Minimum version
cmake_minimum_required(VERSION 2.6)

#Define project
project(wiz3d)

#Define file groups
file(GLOB WIZ3D_FILES "src/*.c" "lib/lua/*.c" "lib/base64/base64.c")
file(GLOB IDE_FILES "ide/ide.cc" "ide/tinyfiledialogs/tinyfiledialogs.c")

#Add header and lib directories
include_directories(${INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/lib/lua ${CMAKE_CURRENT_SOURCE_DIR}/lib ${CMAKE_CURRENT_SOURCE_DIR}/ide/fltk)
link_directories(${LINK_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/lib/sdl2 ${CMAKE_CURRENT_SOURCE_DIR}/lib/ode ${CMAKE_CURRENT_SOURCE_DIR}/ide/fltk)

#Add definitions
add_definitions(-DdNODEBUG -DdSINGLE -DODE_LIB)

#Add executable targets
add_executable(wiz3d ${WIZ3D_FILES})
#add_executable(ide ${IDE_FILES})

#Add platform specific options
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-DWIN32)
	if (MSVC)
		message("Platform is Windows (MSVC)")
		#add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	else (MSVC)
		message("Platform is Windows (MinGW)")
		add_definitions(-m32)
		#set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c90 -pedantic-errors -Wall")
		set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++98 -fno-exceptions -fno-rtti")
		set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-m32 -s -static -mwindows")
	endif (MSVC)
	target_link_libraries(wiz3d SDL2.win32 ode.win32 opengl32 comdlg32 winmm gdi32 ole32 oleaut32 uuid imm32 stdc++ version ws2_32 ${TARGET_LINK_LIBRARIES})
	#target_link_libraries(ide fltk.win32 comctl32 gdi32 ole32 uuid ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("Platform is macOS")
	#add_definitions(-std=c++11 -stdlib=libc++)
	#set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c90 -pedantic-errors -Wall")
	set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-m32")
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-m32 -std=c++98 -fno-exceptions -fno-rtti")
	target_link_libraries(wiz3d -m32 -lSDL2.macos -lode.macos -liconv -lstdc++ "-framework AppKit" "-framework AudioToolbox" "-framework Carbon" "-framework CoreAudio" "-framework CoreVideo" "-framework ForceFeedback" "-framework IOKit" "-framework OpenGL" ${TARGET_LINK_LIBRARIES})
	#target_link_libraries(ide -lfltk.macos64 "-framework AppKit" ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("Platform is Linux")
	#set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c90 -pedantic-errors -Wall")
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fno-exceptions -fno-rtti")
	#set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-s -static")
	target_link_libraries(wiz3d -lSDL2.linux64 -lode.linux64 -ldl -lGL -lm -lpthread -lstdc++ ${TARGET_LINK_LIBRARIES})
	#target_link_libraries(ide -lfltk.linux64 -lXext -lXcursor -lXrender -lXfixes -lX11 -lm -ldl -lXinerama ${TARGET_LINK_LIBRARIES})
else (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("Unrecognized platform " ${CMAKE_SYSTEM_NAME})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
