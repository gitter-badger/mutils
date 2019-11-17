#define BOOST_TEST_MODULE "test_error_detection"
#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif
#include <toml.hpp>
#include <iostream>
#include <fstream>

BOOST_AUTO_TEST_CASE(test_detect_empty_key)
{
    std::istringstream stream(std::string("= \"value\""));
    BOOST_CHECK_THROW(toml::parse(stream), toml::syntax_error);
}

BOOST_AUTO_TEST_CASE(test_detect_missing_value)
{
    std::istringstream stream(std::string("a ="));
    BOOST_CHECK_THROW(toml::parse(stream), toml::syntax_error);
}

BOOST_AUTO_TEST_CASE(test_detect_too_many_value)
{
    std::istringstream stream(std::string("a = 1 = \"value\""));
    BOOST_CHECK_THROW(toml::parse(stream), toml::syntax_error);
}

BOOST_AUTO_TEST_CASE(test_detect_duplicate_table)
{
    std::istringstream stream(std::string(
            "[table]\n"
            "a = 42\n"
            "[table]\n"
            "b = 42\n"
            ));
    BOOST_CHECK_THROW(toml::parse(stream), toml::syntax_error);
}

BOOST_AUTO_TEST_CASE(test_detect_conflict_array_table)
{
    std::istringstream stream(std::string(
            "[[table]]\n"
            "a = 42\n"
            "[table]\n"
            "b = 42\n"
            ));
    BOOST_CHECK_THROW(toml::parse(stream), toml::syntax_error);
}

BOOST_AUTO_TEST_CASE(test_detect_conflict_table_array)
{
    std::istringstream stream(std::string(
            "[table]\n"
            "a = 42\n"
            "[[table]]\n"
            "b = 42\n"
            ));
    BOOST_CHECK_THROW(toml::parse(stream), toml::syntax_error);
}

BOOST_AUTO_TEST_CASE(test_detect_duplicate_value)
{
    std::istringstream stream(std::string(
            "a = 1\n"
            "a = 2\n"
            ));
    BOOST_CHECK_THROW(toml::parse(stream), toml::syntax_error);
}

BOOST_AUTO_TEST_CASE(test_detect_conflicting_value)
{
    std::istringstream stream(std::string(
            "a.b   = 1\n"
            "a.b.c = 2\n"
            ));
    BOOST_CHECK_THROW(toml::parse(stream), toml::syntax_error);
}

BOOST_AUTO_TEST_CASE(test_detect_inhomogeneous_array)
{
    std::istringstream stream(std::string(
            "a = [1, 1.0]\n"
            ));
    BOOST_CHECK_THROW(toml::parse(stream), toml::syntax_error);
}

BOOST_AUTO_TEST_CASE(test_detect_appending_array_of_table)
{
    std::istringstream stream(std::string(
            "a = [{b = 1}]\n"
            "[[a]]\n"
            "b = 2\n"
            ));
    BOOST_CHECK_THROW(toml::parse(stream), toml::syntax_error);
}