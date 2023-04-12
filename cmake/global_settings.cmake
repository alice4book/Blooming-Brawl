set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS    OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS     OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL        OFF CACHE INTERNAL "Generate installation target")

set(BUILD_SHARED_LIBS                       OFF CACHE INTERNAL "Build package with shared libraries.")
set(ASSIMP_BUILD_ASSIMP_TOOLS               OFF CACHE INTERNAL "If the supplementary tools for Assimp are built in addition to the library.")
set(ASSIMP_NO_EXPORT                        ON CACHE INTERNAL  "Disable Assimp export")
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT   OFF CACHE INTERNAL "Disable Assimp export")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT   OFF CACHE INTERNAL "Disable Assimp export")
set(ASSIMP_BUILD_TESTS                      OFF CACHE INTERNAL "If the test suite for Assimp is built in addition to the library.")
set(ASSIMP_BUILD_TESTS                      OFF CACHE INTERNAL "If the test suite for Assimp is built in addition to the library.")

# You can change supported importers here
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
#set(ASSIMP_BUILD_GLTF_IMPORTER ON) // GLB and gltf
#set(ASSIMP_BUILD_COLLADA_IMPORTER ON) // DAE
#set(ASSIMP_BUILD_FBX_IMPORTER ON)