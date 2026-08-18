cmake_minimum_required(VERSION 4.0)

find_package(cmake-bare REQUIRED)

project(bare_app_kit_testing C OBJC)

add_bare_module(bare_app_kit_testing NAME bare-app-kit-testing)
target_sources(${bare_app_kit_testing} PRIVATE binding.m)
target_compile_definitions(${bare_app_kit_testing} PRIVATE BARE_APP_KIT_TESTING)
target_link_libraries(${bare_app_kit_testing} PUBLIC "-framework AppKit")
