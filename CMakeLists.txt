cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
project(libmptask-demo)

# Link MediaPipe Shared Lib
set(MEDIAPIPE_DIR "D:/code/git/github/mediapipe")
set(bazel_out_DIR "D:/code/git/github/mediapipe/bazel_out/uwisdqlp")

# D:/code/git/github/mediapipe/bazel_out/uwisdqlp/execroot/mediapipe/bazel-out/x64_windows-opt/bin
set(bazel_bin_DIR "${bazel_out_DIR}/execroot/mediapipe/bazel-out/x64_windows-opt/bin")

# D:/code/git/github/mediapipe/bazel_out/uwisdqlp/external
set(bazel_external_dir "${bazel_out_DIR}/external")

# D:/code/git/github/mediapipe/bazel_out/uwisdqlp/execroot/mediapipe/bazel-out/x64_windows-opt/bin/external
set(bazel_bin_external_DIR "${bazel_bin_DIR}/external")


# 3rd libs
# absl
set(absl_INCLUDE_DIR "${bazel_external_dir}/com_google_absl")

# glog
set(glog_INCLUDE_DIR "${bazel_bin_external_DIR}/com_github_glog_glog_windows/_virtual_includes/glog")

if(NOT EXISTS "${MEDIAPIPE_DIR}")
	message(FATAL_ERROR "Mediapipe directory '${MEDIAPIPE_DIR}' not found!")
endif()
get_filename_component(MEDIAPIPE_DIRNAME "${MEDIAPIPE_DIR}" NAME)


# libmptask
set(LIBMPTASK "libmptask")
set(LIBMPTASK_TARGET "face_landmarker")

# D:/code/git/github/mediapipe/mediapipe/tasks/libmptask/vision/face_landmarker
set(LIBMPTASK_SOURCE_DIR "${MEDIAPIPE_DIR}/mediapipe/tasks/${LIBMPTASK}/vision/${LIBMPTASK_TARGET}")

# D:/code/git/github/mediapipe/bazel_out/uwisdqlp/execroot/mediapipe/bazel-out/x64_windows-opt/bin/mediapipe/tasks/libmptask/vision/face_landmarker
set(LIBMPTASK_BIN_DIR "${bazel_bin_DIR}/mediapipe/tasks/${LIBMPTASK}/vision/${LIBMPTASK_TARGET}")

add_library(LIBMP SHARED IMPORTED)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

	#	D:/code/git/github/mediapipe/bazel_out/uwisdqlp/execroot/mediapipe/bazel-out/x64_windows-opt/bin/mediapipe/tasks/cc/vision/libmptask/libmptask.dll
	set(LIBMP_SHARED_LIBRARY "${LIBMPTASK_BIN_DIR}/${LIBMPTASK_TARGET}.dll")

	#	D:/code/git/github/mediapipe/bazel_out/uwisdqlp/execroot/mediapipe/bazel-out/x64_windows-opt/bin/mediapipe/tasks/cc/vision/libmptask/libmptask.dll.if.lib
	set(LIBMP_IMPORT_LIB "${LIBMPTASK_BIN_DIR}/${LIBMPTASK_TARGET}.dll.if.lib")

	set_target_properties(LIBMP PROPERTIES
			IMPORTED_LOCATION "${LIBMP_SHARED_LIBRARY}"
			IMPORTED_IMPLIB "${LIBMP_IMPORT_LIB}"
			INTERFACE_INCLUDE_DIRECTORIES "${LIBMPTASK_SOURCE_DIR}"
	)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(LIBMP_SHARED_LIBRARY "${LIBMPTASK_BIN_DIR}/${LIBMP_TARGET_NAME}.so")
	set_target_properties(LIBMP PROPERTIES
			IMPORTED_LOCATION "${LIBMP_SHARED_LIBRARY}"
			INTERFACE_INCLUDE_DIRECTORIES "${LIBMPTASK_SOURCE_DIR}"
	)
endif()


# Protobuf
# use Protobuf bundled with LibMP
# D:/code/git/github/mediapipe/bazel_out/uwisdqlp/external/com_google_protobuf
set(protobuf_source_dir "${bazel_out_DIR}/external/com_google_protobuf")
include("${CMAKE_CURRENT_SOURCE_DIR}/libprotobuf.cmake")

set(EIGEN_ROOT_DIR "${bazel_external_dir}/eigen_archive")

# MediaPipe Formats
file(GLOB_RECURSE MP_FORMATS_SRC "${bazel_bin_DIR}/mediapipe/framework/formats/*.cc")
if(MSVC)
  # MSVC warning suppressions
  add_definitions(${MSVC_WARNINGS})
endif(MSVC)

add_library(MP_FORMATS STATIC ${MP_FORMATS_SRC})
target_include_directories(MP_FORMATS PUBLIC
		"${bazel_bin_DIR}"
		"${MEDIAPIPE_DIR}"
		"${absl_INCLUDE_DIR}"
		"${glog_INCLUDE_DIR}"
		"${EIGEN_ROOT_DIR}"

)
target_link_libraries(MP_FORMATS libprotobuf)

# OpenCV
# use OpenCV bundled with LibMP
message("system: ${CMAKE_SYSTEM_NAME}")
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(LibMP_OpenCV_DIR "${bazel_out_DIR}/external/windows_opencv")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(LibMP_OpenCV_DIR "${MEDIAPIPE_DIR}/bazel-${MEDIAPIPE_DIRNAME}/external/linux_opencv")
endif()
set(OpenCV_DIR "${LibMP_OpenCV_DIR}/x64/vc15/lib") # use OpenCV bundled with LibMP
find_package(OpenCV REQUIRED)

# Main executable
add_executable(main code/src/main.cpp)
set_property(TARGET main PROPERTY CXX_STANDARD 17)
set_property(TARGET main PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(main
		LIBMP
		"${LIBMP_IMPORT_LIB}"
		MP_FORMATS
		${OpenCV_LIBS}
)

### Post-build copies ###
# LibMP - MediaPipe Shared Library
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${LIBMP_SHARED_LIBRARY}" "$<TARGET_FILE_DIR:main>"
    COMMENT "Copying ${LIBMP_SHARED_LIBRARY}"
)
# Mediapipe Modules
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${bazel_bin_DIR}/mediapipe/modules" "$<TARGET_FILE_DIR:main>/mediapipe/modules"
    COMMENT "Copying mediapipe/modules"
)
# Mediapipe Models
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${bazel_bin_DIR}/mediapipe/models" "$<TARGET_FILE_DIR:main>/mediapipe/models"
    COMMENT "Copying mediapipe/models"
)
# OpenCV (Windows only)
IF(WIN32)
	add_custom_command(TARGET main POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${LIBMPTASK_BIN_DIR}/opencv_world460.dll" "$<TARGET_FILE_DIR:main>"
		COMMENT "Copying OpenCV DLL (Release)"
	)
ENDIF()
