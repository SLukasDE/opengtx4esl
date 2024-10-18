include(CMakeFindDependencyMacro)

find_dependency(esa)
find_dependency(esl)
find_dependency(GnuTLS)

include("${CMAKE_CURRENT_LIST_DIR}/mhd4eslTargets.cmake")
