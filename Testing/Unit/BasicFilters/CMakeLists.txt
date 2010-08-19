set(BasicFiltersTestSource
  itkRecursiveGaussianImageFilterUnitTests.cxx )

add_executable(itkBasicFiltersUnitTests ${BasicFiltersTestSource} )

target_link_libraries(itkBasicFiltersUnitTests ITKTestHarness ITKIO ITKBasicFilters)
add_google_tests(${EXECUTABLE_OUTPUT_PATH}/itkBasicFiltersUnitTests ${BasicFiltersTestSource})
