

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

set(LN_SOURCES_Demo_DxLib
	Main.cpp
)

include_directories(
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/external/Lumino.Core/include"
	"${PROJECT_SOURCE_DIR}/external/Lumino.Math/include")
link_libraries(Lumino_Static)
add_executable(Lumino_Demo_DxLib ${LN_SOURCES_Demo_DxLib})
