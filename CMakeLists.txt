cmake_minimum_required(VERSION 3.16)

project(CpuRenderEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB TXT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/bezierSurfaces/*.txt")
foreach(TXT_FILE ${TXT_FILES})
    get_filename_component(TXT_FILE_NAME ${TXT_FILE} NAME)
    configure_file(${TXT_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${TXT_FILE_NAME} COPYONLY)
endforeach()

file(GLOB MESHES "${CMAKE_CURRENT_SOURCE_DIR}/assets/meshes/*")
foreach(MESH ${MESHES})
    get_filename_component(MESH_NAME ${MESH} NAME)
    configure_file(${MESH} ${CMAKE_CURRENT_BINARY_DIR}/meshes/${MESH_NAME} COPYONLY)
endforeach()

file(GLOB TEXTURES "${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/*.png" "${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/*.jpg")
foreach(TEXTURE ${TEXTURES})
    get_filename_component(TEXTURE_NAME ${TEXTURE} NAME)
    configure_file(${TEXTURE} ${CMAKE_CURRENT_BINARY_DIR}/textures/${TEXTURE_NAME} COPYONLY)
endforeach()

file(GLOB NORMALMAPS "${CMAKE_CURRENT_SOURCE_DIR}/assets/normalMaps/*.png" "${CMAKE_CURRENT_SOURCE_DIR}/assets/normalMaps/*.jpg")
foreach(NORMALMAP ${NORMALMAPS})
    get_filename_component(NORMALMAP_NAME ${NORMALMAP} NAME)
    configure_file(${NORMALMAP} ${CMAKE_CURRENT_BINARY_DIR}/normalMaps/${NORMALMAP_NAME} COPYONLY)
endforeach()

file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp" include/*.h)
set(PROJECT_SOURCES
        ${PROJECT_SOURCES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CpuRenderEngine
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(CpuRenderEngine SHARED
                ${PROJECT_SOURCES}
        )
    else()
        add_executable(CpuRenderEngine
                ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CpuRenderEngine PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(CpuRenderEngine PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CpuRenderEngine)
endif()
set_target_properties(CpuRenderEngine PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CpuRenderEngine
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CpuRenderEngine)
endif()
