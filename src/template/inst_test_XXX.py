# ==========================================================================
# This module performs unit tests for the GammaLib [INSTRUMENT] module.
#
# Copyright (C) [YEAR] by [AUTHOR]
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# ==========================================================================
import gammalib


# ====================================== #
# Test class for GammaLib [INSTRUMENT] module #
# ====================================== #
class Test(gammalib.GPythonTestSuite):
    """
    Test class for GammaLib [INSTRUMENT] module.
    """
    # Constructor
    def __init__(self):
        """
        Constructor.
        """
        # Call base class constructor
        gammalib.GPythonTestSuite.__init__(self)

        # Return
        return

    # Set test functions
    def set(self):
        """
        Set all test functions.
        """
        # Set test name
        self.name("[INSTRUMENT]")

        # Append tests
        self.append(self.test, "[INSTRUMENT] dummy test")

        # Return
        return

    # Test function
    def test(self):
        """
        Test function.
        """
        # Return
        return
