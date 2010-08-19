# An option to build/run the long tests
option ( RUN_LONG_TESTS "Run long tests.  Some tests have been identified as long running, should these be run?" OFF )

include ( TestHarness.cmake )

IF(ITK_BINARY_DIR)
  SET(ITK_TEST_OUTPUT_DIR "${ITK_BINARY_DIR}/Testing/Temporary")
ELSE(ITK_BINARY_DIR)
  SET(ITK_TEST_OUTPUT_DIR "${ITKTesting_BINARY_DIR}/Testing/Temporary")
ENDIF(ITK_BINARY_DIR)


subdirs( IO BasicFilters )
