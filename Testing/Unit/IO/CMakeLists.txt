set(IOTestSource
  itkNrrdImageReadWriteUnitTest.cxx )

add_executable(itkIOUnitTests ${IOTestSource} )
target_link_libraries(itkIOUnitTests ITKTestHarness ITKIO)
add_google_tests(${EXECUTABLE_OUTPUT_PATH}/itkIOUnitTests ${IOTestSource})
