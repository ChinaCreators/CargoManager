project(CargoManager)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

include_directories(./include)
aux_source_directory(source SRC_LIST)

add_executable(CargoManager ${SRC_LIST})
target_link_libraries(CargoManager wt)
target_link_libraries(CargoManager wthttp)
