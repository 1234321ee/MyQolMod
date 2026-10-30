cmake_minimum_required(VERSION 3.25)
project(MyQoLMod VERSION 1.0.0)

# Указываем стандарт C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Эта строчка исправляет ошибку библиотеки fmt на серверах GitHub
add_compile_definitions(GEODE_DISABLE_FMT_CONSTEVAL=1)

# Проверяем, что переменная окружения GEODE_SDK установлена
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK!")
endif()

# Подключаем Geode
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# 👇 СОЗДАЕМ ЦЕЛЬ (библиотеку) для нашего мода
# Находим все .cpp файлы в папке src
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Настраиваем мод
setup_geode_mod(${PROJECT_NAME})
