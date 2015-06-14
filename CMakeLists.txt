# cmake requirements
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# project id
PROJECT(factory)

IF("${CMAKE_BUILD_TYPE}" STREQUAL "")
SET(CMAKE_BUILD_TYPE "Debug")
ENDIF("${CMAKE_BUILD_TYPE}" STREQUAL "")
MESSAGE(STATUS "Build mode is ${CMAKE_BUILD_TYPE}")

# force output into several folders
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
     ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE} )
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
     ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE} )
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
     ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE} )
SET(CMAKE_PROJECT_DOC_DIR
    ${PROJECT_BINARY_DIR}/doc )
SET(CMAKE_CURRENT_BINARY_DIR
    ${PROJECT_BINARY_DIR}/moc  )

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/include
)

########### Test Target #############
ADD_EXECUTABLE(factory_test src/factory_test.cpp)


########### Compiler Flags #############
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# Set clang as standard compiler
ADD_DEFINITIONS(-fPIC -fsigned-char -Wall -fPIC  -std=c++11)

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  ADD_DEFINITIONS("-g -DDEBUG -O1")
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Debug")

IF(${CMAKE_BUILD_TYPE} MATCHES "Release")
  ADD_DEFINITIONS("-O3")
  set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Release")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++ -mmacosx-version-min=10.8 -arch x86_64 -Wno-unused-variable")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -std=c++0x -stdlib=libc++ -mmacosx-version-min=10.8 -arch x86_64")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wno-unused-variable -std=c++0x -stdlib=libc++ -mmacosx-version-min=10.8 -arch x86_64")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

IF (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
  ADD_DEFINITIONS(-ferror-limit=5 -fcolor-diagnostics -fdiagnostics-show-template-tree  -Wno-deprecated )
ENDIF()

########### Doxygen #############
# add a target to generate API documentation with Doxygen
FIND_PACKAGE(Doxygen)

IF(DOXYGEN_FOUND MATCHES YES)
  SET(DOXYGEN_FILE_ABS_PATH ${CMAKE_PROJECT_DOC_DIR}/${PROJECT_NAME}.doxygen)

  if (EXISTS ${DOXYGEN_FILE_ABS_PATH})
    CONFIGURE_FILE(${DOXYGEN_FILE_ABS_PATH} ${CMAKE_PROJECT_DOC_DIR} @ONLY)
    ADD_CUSTOM_TARGET(doc
      ${DOXYGEN_EXECUTABLE} ${DOXYGEN_FILE_ABS_PATH}
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen (see ./doc/html/index.html)" VERBATIM
    )
    MESSAGE(STATUS "Doxygen is setup. Use 'make doc' to generate documentation.")
  endif()
ENDIF()
