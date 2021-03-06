# Olivier Stasse, Francois Keith, JRL, CNRS/AIST
# Creation: 4/11/2008
# History:
#
# Copyright CNRS/AIST

INCLUDE(CTest)

# For tools, we include the same files as the src

# Import jrl-mal flags.
INCLUDE_DIRECTORIES(${JRL_MAL_INCLUDEDIR})

# Import abstract-robot-dynamics flags.
INCLUDE_DIRECTORIES(${ABSTRACT_ROBOT_DYNAMICS_INCLUDEDIR})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/private)

# JRL_DYNAMICS_ADD_TEST(TEST)
# ---------------------------
#
# - Declare a new program called ``TEST''.
# - Compile it and link it against jrl-dynamics.
# - Add it to the TEST suite.
#
# TEST : test name
# PRIVATE_SOURCES: private sources used in the test
MACRO(JRL_DYNAMICS_ADD_TEST TEST PRIVATE_SOURCES)
	ADD_EXECUTABLE(${TEST} ${TEST}.cpp ${PRIVATE_SOURCES})
	target_link_libraries(${TEST} jrl-dynamics)
	ADD_DEPENDENCIES (${TEST} jrl-dynamics)
	PKG_CONFIG_USE_DEPENDENCY(${TEST} jrl-mal)
	PKG_CONFIG_USE_DEPENDENCY(${TEST} abstract-robot-dynamics)

	ADD_TEST(${TEST} ${TEST})

	# Make sure jrl-dynamics shared library is compiled before the test.
	#ADD_DEPENDENCIES(${TEST} dynJRL)
	#MESSAGE(FATAL_ERROR ${${PROJECT_NAME}_test_CXXFLAGS})
ENDMACRO(JRL_DYNAMICS_ADD_TEST TEST PRIVATE_SOURCES)

ADD_EXECUTABLE(	TestHumanoidDynamicRobot CommonTools.cpp TestHumanoidDynamicRobot.cpp )
SET_TARGET_PROPERTIES(TestHumanoidDynamicRobot
	              PROPERTIES
		      DEPENDS ${CMAKE_BINARY_DIR}/unitTesting/sample.wrl
)

ADD_CUSTOM_COMMAND(TARGET TestHumanoidDynamicRobot POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/unitTesting/sample.wrl
  ${CMAKE_BINARY_DIR}/unitTesting
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/unitTesting/sampleSpecificities.xml
  ${CMAKE_BINARY_DIR}/unitTesting
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/unitTesting/sampleLinkJointRank.xml
  ${CMAKE_BINARY_DIR}/unitTesting
)

target_link_libraries(TestHumanoidDynamicRobot jrl-dynamics)
ADD_DEPENDENCIES (TestHumanoidDynamicRobot jrl-dynamics)
PKG_CONFIG_USE_DEPENDENCY(TestHumanoidDynamicRobot jrl-mal)
PKG_CONFIG_USE_DEPENDENCY(TestHumanoidDynamicRobot abstract-robot-dynamics)

ADD_EXECUTABLE(	TestHumanoidDynamicRobotDebugLayale CommonTools.cpp TestHumanoidDynamicRobotDebugLayale.cpp)
SET_TARGET_PROPERTIES(TestHumanoidDynamicRobotDebugLayale
	              PROPERTIES
		      DEPENDS ${CMAKE_BINARY_DIR}/unitTesting/sample.wrl
)

ADD_CUSTOM_COMMAND(TARGET TestHumanoidDynamicRobotDebugLayale POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/unitTesting/sample.wrl
  ${CMAKE_BINARY_DIR}/unitTesting
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/unitTesting/sampleSpecificities.xml
  ${CMAKE_BINARY_DIR}/unitTesting
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/unitTesting/sampleLinkJointRank.xml
  ${CMAKE_BINARY_DIR}/unitTesting
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/unitTesting/examples.CMakeList
  ${CMAKE_BINARY_DIR}/unitTesting/CMakeLists.txt.example
)
target_link_libraries(TestHumanoidDynamicRobotDebugLayale jrl-dynamics)
ADD_DEPENDENCIES (TestHumanoidDynamicRobotDebugLayale jrl-dynamics)
PKG_CONFIG_USE_DEPENDENCY(TestHumanoidDynamicRobotDebugLayale jrl-mal)
PKG_CONFIG_USE_DEPENDENCY(TestHumanoidDynamicRobotDebugLayale abstract-robot-dynamics)

ADD_DEPENDENCIES (TestHumanoidDynamicRobotDebugLayale ${PROJECT_NAME})


ADD_EXECUTABLE(	TestAbstractInterface
  CommonTools.cpp
  HumanoidCopy.cpp
  TestAbstractInterface.cpp)
target_link_libraries(TestAbstractInterface jrl-dynamics)
ADD_DEPENDENCIES (TestAbstractInterface jrl-dynamics)
PKG_CONFIG_USE_DEPENDENCY(TestAbstractInterface jrl-mal)
PKG_CONFIG_USE_DEPENDENCY(TestAbstractInterface abstract-robot-dynamics)


ADD_EXECUTABLE(	TestTwoLinksModel
  TwoLinksModel.cpp
  TestTwoLinksModel.cpp )
target_link_libraries(TestTwoLinksModel jrl-dynamics)
ADD_DEPENDENCIES (TestTwoLinksModel jrl-dynamics)
PKG_CONFIG_USE_DEPENDENCY(TestTwoLinksModel jrl-mal)
PKG_CONFIG_USE_DEPENDENCY(TestTwoLinksModel abstract-robot-dynamics)

ADD_EXECUTABLE(	Test2Models Test2Models.cpp)
target_link_libraries(Test2Models jrl-dynamics)
ADD_DEPENDENCIES (Test2Models jrl-dynamics)
PKG_CONFIG_USE_DEPENDENCY(Test2Models jrl-mal)
PKG_CONFIG_USE_DEPENDENCY(Test2Models abstract-robot-dynamics)

ADD_EXECUTABLE(	ComputeRealZMP ComputeRealZMP.cpp)
target_link_libraries(ComputeRealZMP jrl-dynamics)
ADD_DEPENDENCIES (ComputeRealZMP jrl-dynamics)
PKG_CONFIG_USE_DEPENDENCY(ComputeRealZMP jrl-mal)
PKG_CONFIG_USE_DEPENDENCY(ComputeRealZMP abstract-robot-dynamics)

ADD_EXECUTABLE(	ComputeTorqueOnTrajectory ComputeTorqueOnTrajectory.cpp)
target_link_libraries(ComputeTorqueOnTrajectory jrl-dynamics)
ADD_DEPENDENCIES (ComputeTorqueOnTrajectory jrl-dynamics)
PKG_CONFIG_USE_DEPENDENCY(ComputeTorqueOnTrajectory jrl-mal)
PKG_CONFIG_USE_DEPENDENCY(ComputeTorqueOnTrajectory abstract-robot-dynamics)

ADD_EXECUTABLE(	TestFreeFlyer TestFreeFlyer.cpp CommonTools.cpp)
target_link_libraries(TestFreeFlyer jrl-dynamics)
ADD_DEPENDENCIES(TestFreeFlyer jrl-dynamics)
PKG_CONFIG_USE_DEPENDENCY(TestFreeFlyer jrl-mal)
PKG_CONFIG_USE_DEPENDENCY(TestFreeFlyer abstract-robot-dynamics)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/unitTesting/output.ascii.cmake
		 ${CMAKE_BINARY_DIR}/unitTesting/output.ascii )

ADD_TEST(TestHumanoidDynamicRobot TestHumanoidDynamicRobot "output.ascii")

ADD_TEST(TestAbstractInterface TestAbstractInterface)

ADD_TEST(TestTwoLinksModel TestTwoLinksModel)

SET(TESTS_SPATIAL_SOURCES ../src/private/Spatial.cpp)

JRL_DYNAMICS_ADD_TEST(TestSpatial_doubleByv ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_vBydouble ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_vPlusv ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_vPlusvector ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_vectorPlusv ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_vMoinsv ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_vEgalvector ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_vCrossm ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_vCrossvector ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_dvPlusdv ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_dvPlusvector ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_vectorPlusdv ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_dvMoinsdv ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_dvEgalvector ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_fPlusf ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_fMoinsf ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_fBydouble ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_fEgalvector ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_iPlusi ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_iByv ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_iBydv ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_XByX ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_XByv ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_XBydv ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_XByf ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_XEgalX ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_XInverse ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_XTByf ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestSpatial_XTByv ${TESTS_SPATIAL_SOURCES})
JRL_DYNAMICS_ADD_TEST(TestRobotDynamicsImpl ${TESTS_SPATIAL_SOURCES})

SET(${PROJECT_NAME}_EXAMPLES
  CommonTools.cpp
  CommonTools.h
  TestHumanoidDynamicRobot.cpp
  TestAbstractInterface.cpp
  TestFreeFlyer.cpp
  ComputeRealZMP.cpp
  TwoLinksModel.cpp
  TwoLinksModel.h
  TestTwoLinksModel.cpp
  HumanoidCopy.cpp
  HumanoidCopy.h
  )

INSTALL(FILES ${${PROJECT_NAME}_EXAMPLES}
  DESTINATION share/jrl-dynamics/examples/
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE )


INSTALL(FILES ${CMAKE_BINARY_DIR}/unitTesting/CMakeLists.txt.example
  DESTINATION share/jrl-dynamics/examples/
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE )

SET(${PROJECT_NAME}_EXAMPLES_DATA
  output.ascii.cmake
  sample.wrl
  sampleSpecificities.xml
  sampleLinkJointRank.xml
  sampleInitConfig.dat
  )
INSTALL(FILES ${${PROJECT_NAME}_EXAMPLES_DATA}
  DESTINATION share/jrl-dynamics/examples/data
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE )

