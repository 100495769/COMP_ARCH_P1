# Executable for all unit tests with list of sources
# For example, you may have one ∗_test.cpp for each ∗.cpp in img−aos
add_executable(ftest-aos
        ftest-aos.cpp
        #test.cpp añadir aqui
)
# Library dependencies
target_link_libraries (ftest-aos
        PRIVATE
        imtool−aos
        GTest::gtest_main
        Microsoft.GSL::GSL)