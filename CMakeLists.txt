cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME_STR git)
project(${PROJECT_NAME_STR} LANGUAGES CXX)

# --- Стандарт C++ ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
#  ЛОКАЛЬНЫЙ GOOGLETEST
# ============================================================
set(GOOGLETEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/googletest"
    CACHE PATH "Path to local googletest source")

if(NOT EXISTS "${GOOGLETEST_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "GoogleTest не найден по пути: ${GOOGLETEST_DIR}\n"
        "Укажите корректный путь через -DGOOGLETEST_DIR=...")
endif()

set(gtest_force_shared_crt ON  CACHE BOOL "" FORCE)
set(INSTALL_GTEST          OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK            OFF CACHE BOOL "" FORCE)

add_subdirectory("${GOOGLETEST_DIR}"
                 "${CMAKE_BINARY_DIR}/googletest-build"
                 EXCLUDE_FROM_ALL)

# ============================================================
#  ИСХОДНИКИ ОСНОВНОЙ ПРОГРАММЫ
#  Собираем ВСЕ .cpp рекурсивно, кроме тестов и служебных папок.
# ============================================================
file(GLOB_RECURSE ALL_CPP CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Исключаем служебные каталоги и сам googletest
list(FILTER ALL_CPP EXCLUDE REGEX "/(build|out|CMakeFiles|\\.vs|cmake-build-[^/]*)/")
list(FILTER ALL_CPP EXCLUDE REGEX "/(googletest|googlemock)/")

# Исключаем тестовые файлы из основной цели
list(FILTER ALL_CPP EXCLUDE REGEX "/(tests?)/")
list(FILTER ALL_CPP EXCLUDE REGEX "/test[^/]*\\.cpp$")
list(FILTER ALL_CPP EXCLUDE REGEX "/[^/]*_test\\.cpp$")

if(NOT ALL_CPP)
    message(FATAL_ERROR
        "Не найден ни один .cpp файл в ${CMAKE_CURRENT_SOURCE_DIR}.\n"
        "Проверьте, что stack.cpp (или main.cpp) лежит внутри проекта.\n"
        "Текущее содержимое каталога: ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

message(STATUS "Найдены исходники основной программы:")
foreach(f IN LISTS ALL_CPP)
    message(STATUS "  ${f}")
endforeach()

add_executable(${PROJECT_NAME_STR} ${ALL_CPP})

target_include_directories(${PROJECT_NAME_STR} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(MSVC)
    target_compile_options(${PROJECT_NAME_STR} PRIVATE /W4 /permissive-)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                 PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME_STR})
endif()

# ============================================================
#  ТЕСТЫ
# ============================================================
enable_testing()

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/test*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*_test.cpp"
)
list(FILTER TEST_SOURCES EXCLUDE REGEX "/(build|out|CMakeFiles|\\.vs|cmake-build-[^/]*|googletest)/")

if(TEST_SOURCES)
    message(STATUS "Найдены файлы тестов:")
    foreach(f IN LISTS TEST_SOURCES)
        message(STATUS "  ${f}")
    endforeach()

    add_executable(unit_tests ${TEST_SOURCES})
    target_include_directories(unit_tests PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )
    target_link_libraries(unit_tests PRIVATE GTest::gtest_main)

    if(MSVC)
        target_compile_options(unit_tests PRIVATE /W4 /permissive-)
    endif()

    include(GoogleTest)
    gtest_discover_tests(unit_tests)
else()
    message(STATUS "Файлы тестов не найдены — цель unit_tests не создаётся.")
endif()