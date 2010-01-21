# Olivier Stasse, Francois Keith, JRL, CNRS/AIST
# Creation: 21/01/2010
# History:
#
# Copyright CNRS/AIST

# For tools, we include the same files as the src
SET(${PROJECT_NAME}_test_CXXFLAGS "-I${PROJECT_SOURCE_DIR}/include/ ${${PROJECT_NAME}_CXXFLAGS}")

# For tools, we link with the library corresponding to the src,  which is not installed yet
IF(UNIX)
  SET(${PROJECT_NAME}_test_CXXFLAGS "${${PROJECT_NAME}_test_CXXFLAGS} -DUNIX")
  SET(${PROJECT_NAME}_test_LDFLAGS "${LIBDIR_KW}${${PROJECT_NAME}_BINARY_DIR}/src ${LIBDIR_RP}${${PROJECT_NAME}_BINARY_DIR}/src ${LIBINCLUSION_KW}${PROJECT_NAME} ${${PROJECT_NAME}_src_LDFLAGS}")

  IF(CMAKE_BUILD_TYPE MATCHES "DEBUG")
    IF(CMAKE_COMPILER_IS_GNUCXX)
      SET (${PROJECT_NAME}_test_LDFLAGS " ${${PROJECT_NAME}_test_LDFLAGS} -lgcov")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
  ENDIF(CMAKE_BUILD_TYPE MATCHES "DEBUG")

ENDIF(UNIX)

IF(WIN32)
  SET(${PROJECT_NAME}_test_LDFLAGS "${LIBDIR_KW}${${PROJECT_NAME}_BINARY_DIR}/src  ${LIBINCLUSION_KW}${PROJECT_NAME}${LIB_EXT} ${${PROJECT_NAME}_src_LDFLAGS}")
ENDIF(WIN32)

MESSAGE(STATUS " unitTesting: ${UNITTESTING_CXXFLAGS}")

# Installing testing files.
MESSAGE(STATUS "${${PROJECT_NAME}_BINARY_DIR}/tools/sample.wrl")

ADD_EXECUTABLE(	ReadOpenHRPFileAndExport GenerateRobotForAMELIF.cpp ReadOpenHRPFileAndExport.cpp)
SET_TARGET_PROPERTIES(ReadOpenHRPFileAndExport
	              PROPERTIES
		      COMPILE_FLAGS ${${PROJECT_NAME}_test_CXXFLAGS}
		      LINK_FLAGS ${${PROJECT_NAME}_test_LDFLAGS}
		      DEPENDS ${${PROJECT_NAME}_BINARY_DIR}/tools/sample.wrl
)

ADD_CUSTOM_COMMAND(TARGET TestHumanoidDynamicRobot POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${${PROJECT_NAME}_SOURCE_DIR}/unitTesting/sample.wrl 
  ${${PROJECT_NAME}_BINARY_DIR}/tools
  COMMAND ${CMAKE_COMMAND} -E copy ${${PROJECT_NAME}_SOURCE_DIR}/unitTesting/sampleSpecificities.xml
  ${${PROJECT_NAME}_BINARY_DIR}/tools
  COMMAND ${CMAKE_COMMAND} -E copy ${${PROJECT_NAME}_SOURCE_DIR}/unitTesting/sampleLinkJointRank.xml
  ${${PROJECT_NAME}_BINARY_DIR}/tools
)

ADD_DEPENDENCIES (ReadOpenHRPFileAndExport ${PROJECT_NAME})

