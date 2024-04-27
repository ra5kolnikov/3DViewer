cmake_minimum_required(VERSION 3.5)

project(3DViewer VERSION 0.1 LANGUAGES CXX)

# Execute qmake command to get QT_INSTALL_PREFIX
execute_process(COMMAND qmake -query QT_INSTALL_PREFIX
                OUTPUT_VARIABLE QT_INSTALL_PREFIX
                OUTPUT_STRIP_TRAILING_WHITESPACE)

# Set CMAKE_PREFIX_PATH to QT_INSTALL_PREFIX
set(CMAKE_PREFIX_PATH ${QT_INSTALL_PREFIX})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Set the directory you want to include
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/forms")
set(MODELS_DIR "${RESOURCES_DIR}/models")
set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/shaders")

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})




set(PROJECT_DIRS
    "${FORMS_DIR}"
    "${SHADERS_DIR}"
    "${CMAKE_SOURCE_DIR}/Models"
    "${CMAKE_SOURCE_DIR}/Controllers"
    "${CMAKE_SOURCE_DIR}/Views"
    "${CMAKE_SOURCE_DIR}/Commands"
    "${CMAKE_SOURCE_DIR}"
)

set(PROJECT_SOURCES "")

foreach(dir ${PROJECT_DIRS})
    file(GLOB_RECURSE SHADER_SOURCES "${dir}/*.vs" "${dir}/*.fs" "${dir}/*.frag" "${dir}/*.vert")
    file(GLOB_RECURSE CPP_SOURCES "${dir}/*.cpp" "${dir}/*.cc")
    file(GLOB_RECURSE HEADER_SOURCES "${dir}/*.h" "${dir}/*.hpp" "${dir}/*.tpp")
    file(GLOB_RECURSE UI_SOURCES "${dir}/*.ui")
    file(GLOB_RECURSE QRC_SOURCES "${dir}/*.qrc")
    list(APPEND PROJECT_SOURCES ${CPP_SOURCES} ${HEADER_SOURCES} ${UI_SOURCES} ${SHADER_SOURCES} ${QRC_SOURCES})
endforeach()


find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)

add_subdirectory(ThirdParty/QtGifImage)


set(OUTPUT_DIR "${CMAKE_BINARY_DIR}")
# set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}")

set(NEW_MODELS_DIR "${OUTPUT_DIR}/app_data")
add_custom_target(
    MoveResources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCES_DIR} ${NEW_MODELS_DIR}
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")


add_executable(3DViewer
        ${PROJECT_SOURCES}
)

add_dependencies(3DViewer MoveResources)

foreach(dir ${PROJECT_DIRS})
    target_include_directories(3DViewer PRIVATE ${dir})
endforeach()


target_link_libraries(3DViewer PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Widgets
)

target_link_libraries(3DViewer PRIVATE GIFLIB)

set_target_properties(3DViewer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


# install(TARGETS ${PROJECT_NAME}
#     BUNDLE DESTINATION .
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )
