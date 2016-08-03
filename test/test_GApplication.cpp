/***************************************************************************
 *            test_GApplication.cpp - test GApplication classes            *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2012-2016 by Juergen Knoedlseder                         *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/
/**
 * @file test_GApplication.cpp
 * @brief Testing of application classes
 * @author Juergen Knoedlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <cstdlib>     // setenv
#include <cstdio>
#include "test_GApplication.hpp"

/* __ Globals ____________________________________________________________ */


/***********************************************************************//**
 * @brief Set parameters and tests
 **************************************************************************/
void TestGApplication::set(void)
{
    // Test name
    name("GApplication");

    // Append tests
    append(static_cast<pfunction>(&TestGApplication::test_constructor),
           "Test GLog constructor");
    append(static_cast<pfunction>(&TestGApplication::test_stream_logger),
           "Test stream logger");
    append(static_cast<pfunction>(&TestGApplication::test_C_logger),
           "Test C logger");
    append(static_cast<pfunction>(&TestGApplication::test_GApplicationPar),
           "Test GApplicationPar class");
    append(static_cast<pfunction>(&TestGApplication::test_GApplication),
           "Test GApplication class");

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone test suite
 *
 * @return Pointer to deep copy of test suite.
 ***************************************************************************/
TestGApplication* TestGApplication::clone(void) const
{
    // Clone test suite
    return new TestGApplication(*this);
}


/***********************************************************************//**
 * @brief Test GLog constructor
 **************************************************************************/
void TestGApplication::test_constructor(void)
{
    // Void constructor
    GLog log1;

    // Copy constructor
    GLog log2 = log1;

    // Return
    return; 
}


/***********************************************************************//**
 * @brief Test stream logger
 **************************************************************************/
void TestGApplication::test_stream_logger(void)
{
    // Test
    GLog log;
    log.date(true);
    log.name("Test");
    log.open("test_GApplication.log", true);
    log << "1. This is a C++ string" << std::endl;
    log << "2. This is an integer: " << int(1) << std::endl;
    log << "3. This is a single precision: " << float(1.23456789) << std::endl;
    log << "4. This is a double precision: " << double(1.23456789) << std::endl;
    log << "5. This is a character: " << 'a' << std::endl;
    log << "6. This is a C string: " << "a" << std::endl;
    log << "7. This is a Boolean: " << true << std::endl;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Test C logger
 **************************************************************************/
void TestGApplication::test_C_logger(void)
{
    // Test
    GLog log;
    log.date(true);
    log.name("Test");
    log.open("test_GApplication.log");
    log("%s", "8. This is a C++ string");
    log("%s %d", "9. This is an integer:", int(1));
    log("%s %f", "10. This is a single precision:", float(1.23456789));
    log("%s %f", "11. This is a double precision:", double(1.23456789));

    // Return
    return; 
}


/***********************************************************************//**
 * @brief Test GApplicationPar class
 **************************************************************************/
void TestGApplication::test_GApplicationPar(void)
{
    // Test void constructor
    test_try("Void constructor");
    try {
        GApplicationPar par;
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }

    // Test copy constructor
    test_try("Copy constructor");
    try {
        GApplicationPar par;
        GApplicationPar par2(par);
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }

    // Test parameter constructor
    test_try("Parameter constructor");
    try {
        GApplicationPar par("name", "r", "a", "1.0", "0.0", "2.0", "Parameter name");
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }

    // Test integer parameter validity
    test_try("Integer parameter in valid range");
    try {
        GApplicationPar par("name", "i", "a", "1", "0", "2", "Parameter name");
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }
    test_try("Integer parameter outside valid range");
    try {
        GApplicationPar par("name", "i", "a", "3", "0", "2", "Parameter name");
        test_try_failure("Integer parameter outside validity range shall throw"
                         " an exception.");
    }
    catch (GException::invalid_value &e) {
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }
    test_try("Integer parameter with valid option");
    try {
        GApplicationPar par("name", "i", "a", "1", "0|1|2", "", "Parameter name");
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }
    test_try("Integer parameter with invalid option");
    try {
        GApplicationPar par("name", "i", "a", "3", "0|1|2", "", "Parameter name");
        test_try_failure("Integer parameter outside validity range shall throw"
                         " an exception.");
    }
    catch (GException::invalid_value &e) {
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }

    // Test real parameter validity
    test_try("Real parameter in valid range");
    try {
        GApplicationPar par("name", "r", "a", "1.0", "0.0", "2.0", "Parameter name");
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }
    test_try("Real parameter outside valid range");
    try {
        GApplicationPar par("name", "r", "a", "3.0", "0.0", "2.0", "Parameter name");
        test_try_failure("Real parameter outside validity range shall throw"
                         " an exception.");
    }
    catch (GException::invalid_value &e) {
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }
    test_try("Real parameter with valid option");
    try {
        GApplicationPar par("name", "r", "a", "1.0", "0.0|1.0|2.0", "", "Parameter name");
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }
    test_try("Real parameter with invalid option");
    try {
        GApplicationPar par("name", "r", "a", "3.0", "0.0|1.0|2.0", "", "Parameter name");
        test_try_failure("Real parameter outside validity range shall throw"
                         " an exception.");
    }
    catch (GException::invalid_value &e) {
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }

    // Test string parameter validity
    test_try("String parameter with valid option");
    try {
        GApplicationPar par("name", "s", "a", "WaN", "Obi|Wan|Joda", "", "Parameter name");
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }
    test_try("String parameter with invalid option");
    try {
        GApplicationPar par("name", "s", "a", "Kenobi", "Obi|Wan|Joda", "", "Parameter name");
        test_try_failure("String parameter outside validity range shall throw"
                         " an exception.");
    }
    catch (GException::invalid_value &e) {
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }

    // Test filename parameter validity
    test_try("String parameter with valid option");
    try {
        GApplicationPar par("name", "f", "a", "Wan", "Obi|Wan|Joda", "", "Parameter name");
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }
    test_try("String parameter with invalid option");
    try {
        GApplicationPar par("name", "f", "a", "WaN", "Obi|Wan|Joda", "", "Parameter name");
        test_try_failure("String parameter outside validity range shall throw"
                         " an exception.");
    }
    catch (GException::invalid_value &e) {
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }
    test_try("String parameter with invalid option");
    try {
        GApplicationPar par("name", "f", "a", "Kenobi", "Obi|Wan|Joda", "", "Parameter name");
        test_try_failure("String parameter outside validity range shall throw"
                         " an exception.");
    }
    catch (GException::invalid_value &e) {
        test_try_success();
    }
    catch (std::exception &e) {
        test_try_failure(e);
    }

    // Test integer parameter exceptions
    GApplicationPar par;
    par = GApplicationPar("name", "i", "a", "INDEF", "", "", "Parameter name");
    test_assert(par.is_undefined(), "Check integer parameter INDEF.",
                par.value()+" found instead of undefined value.");
    par = GApplicationPar("name", "i", "a", "NONE", "", "", "Parameter name");
    test_assert(par.is_undefined(), "Check integer parameter NONE.",
                par.value()+" found instead of undefined value.");
    par = GApplicationPar("name", "i", "a", "UNDEF", "", "", "Parameter name");
    test_assert(par.is_undefined(), "Check integer parameter UNDEF.",
                par.value()+" found instead of undefined value.");
    par = GApplicationPar("name", "i", "a", "UNDEFINED", "", "", "Parameter name");
    test_assert(par.is_undefined(), "Check integer parameter UNDEFINED.",
                par.value()+" found instead of undefined value.");
    par = GApplicationPar("name", "i", "a", "INF", "", "", "Parameter name");
    test_assert(par.is_valid(), "Check integer point parameter INF.",
                par.value()+" found instead of infinite value.");
    par = GApplicationPar("name", "i", "a", "INFINITY", "", "", "Parameter name");
    test_assert(par.is_valid(), "Check integer point parameter INFINITY.",
                par.value()+" found instead of infinite value.");
    par = GApplicationPar("name", "i", "a", "NAN", "", "", "Parameter name");
    test_assert(par.is_valid(), "Check integer point parameter NAN.",
                par.value()+" found instead of not a number.");

    // Test floating point parameter exceptions
    par = GApplicationPar("name", "r", "a", "INDEF", "", "", "Parameter name");
    test_assert(par.is_undefined(), "Check floating point parameter INDEF.",
                par.value()+" found instead of undefined value.");
    par = GApplicationPar("name", "r", "a", "NONE", "", "", "Parameter name");
    test_assert(par.is_undefined(), "Check floating point parameter NONE.",
                par.value()+" found instead of undefined value.");
    par = GApplicationPar("name", "r", "a", "UNDEF", "", "", "Parameter name");
    test_assert(par.is_undefined(), "Check floating point parameter UNDEF.",
                par.value()+" found instead of undefined value.");
    par = GApplicationPar("name", "r", "a", "UNDEFINED", "", "", "Parameter name");
    test_assert(par.is_undefined(), "Check floating point parameter UNDEFINED.",
                par.value()+" found instead of undefined value.");
    par = GApplicationPar("name", "r", "a", "INF", "", "", "Parameter name");
    test_assert(par.is_notanumber(), "Check floating point parameter INF.",
                par.value()+" found instead of infinite value.");
    par = GApplicationPar("name", "r", "a", "INFINITY", "", "", "Parameter name");
    test_assert(par.is_notanumber(), "Check floating point parameter INFINITY.",
                par.value()+" found instead of infinite value.");
    par = GApplicationPar("name", "r", "a", "NAN", "", "", "Parameter name");
    test_assert(par.is_notanumber(), "Check floating point parameter NAN.",
                par.value()+" found instead of not a number.");

    // Return
    return; 
}


/***********************************************************************//**
 * @brief Test GApplication class
 **************************************************************************/
void TestGApplication::test_GApplication(void)
{
    // Allocate application
    GApplication app1("test_GApplication", "1.0.0");

    // Test name and version
    test_value(app1.name(), "test_GApplication", "Check application name");
    test_value(app1.version(), "1.0.0", "Check application version");

    // Open log file
    app1.logFileOpen();

    // Test log filename
    test_value(app1.par_filename(), "test_GApplication.par",
               "Check application parameter file name");
    test_value(app1.log_filename(), "test_logfile.log",
               "Check application log file name");

    // Write something into the logfile and close it
    app1.log_header();
    app1.log_string(SILENT, "Silent");
    app1.log_string(TERSE, "Terse");
    app1.log_string(NORMAL, "Normal");
    app1.log_string(EXPLICIT, "Explicit");
    app1.log_string(VERBOSE, "Verbose");
    app1.log_string(TERSE, "\n");
    app1.log_value(NORMAL, "User parameter", "3.14");
    app1.log_value(VERBOSE, "Verbose parameter", "3.14!!");
    app1.log_header1(NORMAL, "Header 1");
    app1.log_header2(NORMAL, "Header 2");
    app1.log_header3(NORMAL, "Header 3");
    app1.log_header1(VERBOSE, "Verbose header 1");
    app1.log_header2(VERBOSE, "Verbose header 2");
    app1.log_header3(VERBOSE, "Verbose header 3");
    app1.log_parameters(NORMAL);
    app1.log_parameters(VERBOSE);
    app1.log_trailer();
    app1.logFileClose();

    // Check if log file exists
    char line[200];
    FILE *fp = fopen("test_logfile.log", "r");
    test_assert(fp != NULL, "Check if logfile exists");

    // If log file exists then check it line by line
    if (fp != NULL) {

        // Test header
        fgets(line, 100, fp);
        test_value(line, "***************************************************"
                         "*****************************\n",
                         "Check log file line 1");
        fgets(line, 100, fp);
        test_value(line, "*                               test_GApplication  "
                         "                            *\n",
                         "Check log file line 2");
        fgets(line, 100, fp);
        test_value(line, "* -------------------------------------------------"
                         "--------------------------- *\n",
                         "Check log file line 3");
        fgets(line, 100, fp);
        test_value(line, "* Version: 1.0.0                                   "
                         "                            *\n",
                         "Check log file line 4");
        fgets(line, 100, fp);
        test_value(line, "***************************************************"
                         "*****************************\n",
                         "Check log file line 5");

        // Test logging of strings
        fgets(line, 100, fp);
        test_value(line, "SilentTerseNormal\n", "Check log file line 6");

        // Test logging of user parameter
        fgets(line, 100, fp);
        test_value(line, " User parameter ............: 3.14\n",
                         "Check log file line 7");

        // Test logging of header 1
        fgets(line, 100, fp);
        test_value(line, "\n", "Check log file line 8");
        fgets(line, 100, fp);
        test_value(line, "+==========+\n", "Check log file line 9");
        fgets(line, 100, fp);
        test_value(line, "| Header 1 |\n", "Check log file line 10");
        fgets(line, 100, fp);
        test_value(line, "+==========+\n", "Check log file line 11");

        // Test logging of header 2
        fgets(line, 100, fp);
        test_value(line, "\n", "Check log file line 12");
        fgets(line, 100, fp);
        test_value(line, "+----------+\n", "Check log file line 13");
        fgets(line, 100, fp);
        test_value(line, "| Header 2 |\n", "Check log file line 14");
        fgets(line, 100, fp);
        test_value(line, "+----------+\n", "Check log file line 15");

        // Test logging of header 3
        fgets(line, 100, fp);
        test_value(line, "\n", "Check log file line 16");
        fgets(line, 100, fp);
        test_value(line, "=== Header 3 ===\n", "Check log file line 17");

        // Test logging of parameters
        fgets(line, 100, fp);
        test_value(line, "+============+\n", "Check log file line 18");
        fgets(line, 100, fp);
        test_value(line, "| Parameters |\n", "Check log file line 19");
        fgets(line, 100, fp);
        test_value(line, "+============+\n", "Check log file line 20");
        fgets(line, 100, fp);
        test_value(line, " chatter ...................: 2\n",
                         "Check log file line 21");
        fgets(line, 100, fp);
        test_value(line, " clobber ...................: yes\n",
                         "Check log file line 22");
        fgets(line, 100, fp);
        test_value(line, " debug .....................: no\n",
                         "Check log file line 23");
        fgets(line, 100, fp);
        test_value(line, " mode ......................: ql\n",
                         "Check log file line 24");
        fgets(line, 100, fp);
        test_value(line, " logfile ...................: test_logfile.log\n",
                         "Check log file line 25");
        fgets(line, 32, fp);
        test_value(line, "Application \"test_GApplication\"",
                         "Check log file line 26");
        // Empty remaining characters (not checked since they are machine
        // dependent)
        fgets(line, 200, fp);
        fgets(line, 32, fp);
        test_value(line, "Application \"test_GApplication\"",
                         "Check log file line 27");

        // Close log file
        fclose(fp);
        
    } // endif: log file existed

    // Return
    return; 
}


/***************************************************************************
 * @brief Main entry point for test executable
 ***************************************************************************/
int main(void)
{
    // Set PFILES environment variable
    setenv("PFILES", "data", 1);

    // Allocate test suit container
    GTestSuites testsuites("GApplication");

    // Initially assume that we pass all tests
    bool success = true;

    // Create a test suite
    TestGApplication test;

    // Append test suite to the container
    testsuites.append(test);

    // Run the testsuites
    success = testsuites.run();

    // Save test report
    testsuites.save("reports/GApplication.xml");

    // Return success status
    return (success ? 0 : 1);
}
