if(NOT TARGET depends::ceres-solver)
  if(NOT TARGET depends::eigen)
    message(FATAL_ERROR "depends::ceres-solver expects depends::eigen")
  endif()
  FetchContent_Declare(
    depends-ceres-solver
    GIT_REPOSITORY https://github.com/ceres-solver/ceres-solver.git
    GIT_TAG        2.2.0
  )
  FetchContent_GetProperties(depends-ceres-solver)
  if(NOT depends-ceres-solver_POPULATED)
    message(STATUS "Fetching ceres-solver sources")
    FetchContent_Populate(depends-ceres-solver)
    message(STATUS "Fetching ceres-solver sources - done")
  endif()
  # TODO: clean up parameters and avoid pollution.
  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
  set(BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  set(MINIGLOG ON CACHE BOOL "" FORCE)
  set(GFLAGS OFF CACHE BOOL "" FORCE)
  set(SUITESPARSE OFF CACHE BOOL "" FORCE)
  set(CXSPARSE OFF CACHE BOOL "" FORCE)
  set(TBB OFF CACHE BOOL "" FORCE)
  set(OPENMP OFF CACHE BOOL "" FORCE)
  set(LAPACK ON CACHE BOOL "" FORCE)
  set(MINIGLOG_MAX_LOG_LEVEL 0 CACHE STRING "" FORCE)
  set(EIGEN_PREFER_EXPORTED_EIGEN_CMAKE_CONFIGURATION OFF CACHE BOOL "" FORCE)
  set(EIGEN_INCLUDE_DIR ${depends-eigen-source-dir} CACHE PATH "" FORCE)
  set(EIGEN3_INCLUDE_DIR ${depends-eigen-source-dir} CACHE PATH "" FORCE)

  set(depends-eigen3-config-dir ${depends-ceres-solver_BINARY_DIR}/eigen3-config)
  file(MAKE_DIRECTORY ${depends-eigen3-config-dir})
  file(WRITE ${depends-eigen3-config-dir}/Eigen3Config.cmake
"set(Eigen3_FOUND TRUE)
set(Eigen3_VERSION 5.0.1)
if(NOT TARGET Eigen3::Eigen)
  add_library(Eigen3::Eigen INTERFACE IMPORTED)
  set_target_properties(Eigen3::Eigen PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES \"${depends-eigen-source-dir}\")
endif()
")
  file(WRITE ${depends-eigen3-config-dir}/Eigen3ConfigVersion.cmake
"set(PACKAGE_VERSION 5.0.1)
set(PACKAGE_VERSION_COMPATIBLE TRUE)
if(PACKAGE_FIND_VERSION VERSION_EQUAL PACKAGE_VERSION)
  set(PACKAGE_VERSION_EXACT TRUE)
endif()
")
  set(Eigen3_DIR ${depends-eigen3-config-dir} CACHE PATH "" FORCE)
  add_subdirectory(${depends-ceres-solver_SOURCE_DIR} ${depends-ceres-solver_BINARY_DIR})
  add_library(depends::ceres-solver INTERFACE IMPORTED GLOBAL)
  target_include_directories(depends::ceres-solver
    INTERFACE
      ${depends-ceres-solver_BINARY_DIR}/include
      ${depends-ceres-solver_SOURCE_DIR}/internal/ceres/miniglog
      ${depends-ceres-solver_SOURCE_DIR}/include
  )
  target_link_libraries(depends::ceres-solver INTERFACE ceres depends::eigen)
  if(IOS)
    target_link_libraries(depends::ceres-solver INTERFACE "-framework Accelerate")
  endif()
  set(depends-ceres-solver-source-dir ${depends-ceres-solver_SOURCE_DIR} CACHE INTERNAL "" FORCE)
  set(depends-ceres-solver-binary-dir ${depends-ceres-solver_BINARY_DIR} CACHE INTERNAL "" FORCE)
  mark_as_advanced(depends-ceres-solver-source-dir)
  mark_as_advanced(depends-ceres-solver-binary-dir)
endif()
