
cmake_minimum_required(VERSION 3.23)

project(Chess 
    VERSION 0.0.1
    LANGUAGES CXX
)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8;/permissive->")
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/W4;/w14242;/w14254;/w14263;/w14265;/w14287;/we4289;/w14296;/w14311;/w14545;/w14546>")
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/w14547;/w14549;/w14555;/w14619;/w14640;/w14826;/w14905;/w14906;/w14928>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8;/permissive->")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/W4;/w14242;/w14254;/w14263;/w14265;/w14287;/we4289;/w14296;/w14311;/w14545;/w14546>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/w14547;/w14549;/w14555;/w14619;/w14640;/w14826;/w14905;/w14906;/w14928>")

add_compile_options("$<$<C_COMPILER_ID:GNU>:-Wall;-Wextra;-Wshadow;-pedantic;-Wcast-align;-Wunused>")
add_compile_options("$<$<CXX_COMPILER_ID:GNU>:-Wall;-Wextra;-Wshadow;-Wnon-virtual-dtor;-pedantic;-Wcast-align;-Wunused;-Woverloaded-virtual>")
add_compile_options("$<$<CXX_COMPILER_ID:GNU>:-Wconversion;-Wmisleading-indentation;-Wduplicated-cond;-Wduplicated-branches>")
add_compile_options("$<$<CXX_COMPILER_ID:GNU>:-Wlogical-op;-Wnull-dereference;-Wuseless-cast;-Wdouble-promotion;-Wformat=2>")


add_library(chessCommon STATIC
    src/board.hpp
    src/board.cpp
)

add_executable(chessTests
    tests/testMain.cpp
)
target_link_libraries(chessTests PUBLIC chessCommon)

target_include_directories(chessCommon PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_compile_features(chessCommon
    PUBLIC cxx_std_23
)

find_package(gsl-lite)
target_link_libraries(chessCommon PUBLIC gsl::gsl-lite-v1)

find_package(date)
target_link_libraries(chessCommon PUBLIC date::date date::date-tz)

find_package(cereal)
target_link_libraries(chessCommon PUBLIC cereal::cereal)

find_package(Catch2)
target_link_libraries(chessCommon PUBLIC Catch2::Catch2)

target_link_libraries(chessTests PRIVATE Catch2::Catch2WithMain)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(chessCommon PUBLIC unofficial::sqlite3::sqlite3)

find_package(fmt)
target_link_libraries(chessCommon PUBLIC fmt::fmt)

find_package(nlohmann_json)
target_link_libraries(chessCommon PUBLIC nlohmann_json::nlohmann_json)

find_package(tl-optional REQUIRED)
target_link_libraries(chessCommon PUBLIC tl::optional)

find_package(tl-expected REQUIRED)
target_link_libraries(chessCommon PUBLIC tl::expected)

find_package(clipp REQUIRED)
target_link_libraries(chessCommon PUBLIC clipp::clipp)
