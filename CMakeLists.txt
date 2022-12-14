cmake_minimum_required (VERSION 3.8)
project ("RurDog")
set(SDL2_PATH "C:\\Program Files (x86)\\SDL2\\cmake")
set(SDL2_image_PATH "C:\\Program Files (x86)\\SDL2_image\\cmake")
set(SDL2_mixer_PATH "C:\\Program Files (x86)\\SDL2_mixer\\cmake")
set(SDL2_ttf_PATH "C:\\Program Files (x86)\\SDL2_ttf\\cmake")
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
add_executable (${CMAKE_PROJECT_NAME} "src/Game.cpp" "src/Game.h" "src/playField.h" "src/playField.cpp" "src/config.hpp" "src/utils.hpp"  "cpp.hint" "src/RurDog.cpp"   "src/color.cpp" "src/color.h" "src/coordinate.hpp" "src/tetrmino.h" "src/tetrmino.cpp"  "src/Piece.hpp" "src/animatedPlayField.h" "src/stage.h" "src/animatedPlayField.cpp")
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2_image::SDL2_image)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2_mixer::SDL2_mixer)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RurDog PROPERTY CXX_STANDARD 20)
endif()

add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/assets"
	"$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets"
)
