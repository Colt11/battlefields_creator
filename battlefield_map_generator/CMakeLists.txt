# CMakeList.txt : CMake project for battlefield_map_generator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_PREFIX_PATH "libtorch")

set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("battlefield_map_generator")

find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable (battlefield_map_generator "main.cpp"  "torch_generator.h" "torch_generator.cpp" "helpers.cpp" "tree_generator.cpp" "color_map_generator.h" "color_map_generator.cpp")

target_link_libraries(battlefield_map_generator ${TORCH_LIBRARIES} ${OpenCV_LIBS})

target_compile_features(battlefield_map_generator PUBLIC cxx_std_17)

if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET battlefield_map_generator
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:battlefield_map_generator>)
endif (MSVC)

