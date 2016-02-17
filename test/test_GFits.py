# ==========================================================================
# This module performs unit tests for the GammaLib FITS module.
#
# Copyright (C) 2012-2016 Juergen Knoedlseder
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
import math
import sys
import gammalib


# =================================== #
# Test class for GammaLib FITS module #
# =================================== #
class Test(gammalib.GPythonTestSuite):
    """
    Test class for GammaLib FITS module.
    """
    def __init__(self):
        """
        Constructor.
        """
        # Call base class constructor
        gammalib.GPythonTestSuite.__init__(self)

        # Return
        return

    def set(self):
        """
        Set all test functions.
        """
        # Set test name
        self.name("fits")

        # Append tests
        self.append(self.test_fits, "Test GFits")
        self.append(self.test_fits_image, "Test GFitsImage")
        self.append(self.test_fits_table, "Test GFitsTable")
        self.append(self.test_fits_table_columns, "Test FITS table columns")

        # Return
        return

    def test_fits(self):
        """
        Test GFile class interface.
        """
        # Set test file names
        file = gammalib.GFilename("data/file.fits")

        # Test creation of FITS file
        self.test_try("Test GFits file constructor")
        try:
            fits = gammalib.GFits(file, True)
            self.test_try_success()
        except:
            self.test_try_failure("Unable to create file.")

        # Open FITS file
        fits = gammalib.GFits(file)
        self.test_value(fits["EVENTS"].nrows(), 1231, "Check number of rows")

        # Open FITS file with event selection
        fits = gammalib.GFits(file+"[EVENTS][ENERGY>1.0]")
        self.test_value(fits["EVENTS"].nrows(), 152, "Check number of rows")

        # Open FITS file with non-existing extension name
        self.test_try("Test non-existing extension name")
        try:
            fits = gammalib.GFits(file+"[DUMMY][ENERGY>1.0]")
            self.test_try_failure()
        except:
            self.test_try_success()

        # Return
        return

    def test_fits_image(self):
        """
        Test GFitsImage class interface.
        """
        # Set test file names
        file1 = gammalib.GFilename("test_python_fits_image_v1.fits")
        file2 = gammalib.GFilename("test_python_fits_image_v2.fits")

        # Remove test files
        file1.remove()
        file2.remove()

        # Create FITS file
        fits = gammalib.GFits(file1, True)

        # Create images
        nx   = 10
        ny   = 10
        img1 = gammalib.GFitsImageByte(nx, ny)
        img2 = gammalib.GFitsImageDouble(nx, ny)
        img3 = gammalib.GFitsImageFloat(nx, ny)
        img4 = gammalib.GFitsImageLong(nx, ny)
        img5 = gammalib.GFitsImageLongLong(nx, ny)
        img6 = gammalib.GFitsImageSByte(nx, ny)
        img7 = gammalib.GFitsImageShort(nx, ny)
        img8 = gammalib.GFitsImageULong(nx, ny)
        img9 = gammalib.GFitsImageUShort(nx, ny)
        for x in range(nx):
            for y in range(ny):
                img1[x, y] = x + y * nx
                img2[x, y] = x + y * nx
                img3[x, y] = x + y * nx
                img4[x, y] = x + y * nx
                img5[x, y] = x + y * nx
                img6[x, y] = x + y * nx
                img7[x, y] = x + y * nx
                img8[x, y] = x + y * nx
                img9[x, y] = x + y * nx
        img1.extname("Byte")
        img2.extname("Double")
        img3.extname("Float")
        img4.extname("Long")
        img5.extname("LongLong")
        img6.extname("SByte")
        img7.extname("Short")
        img8.extname("ULong")
        img9.extname("UShort")

        # Append images to FITS file
        fits.append(img1)
        fits.append(img2)
        fits.append(img3)
        fits.append(img4)
        fits.append(img5)
        # fits.append(img6) # Not supported in older cfitsio
        fits.append(img7)
        fits.append(img8)
        fits.append(img9)

        # Set header keywords
        img_byte = fits.image(0)
        img_byte.card("test", "test-value", "this is for testing")
        img_byte.card("real", 3.1415, "a real value")
        img_byte.card("int", 41, "an integer value")

        # Save FITS file
        fits.save()

        # Close FITS file
        fits.close()

        # Re-open FITS file
        fits = gammalib.GFits(file1)

        # Get double precision image, take square root of pixel and save in
        # another file
        img_double = fits.image("Double")
        for x in range(nx):
            for y in range(ny):
                img_double[x, y] = math.sqrt(img_double[x, y])

        # Save into another FITS file
        fits.saveto(file2)

        # Close FITS file
        fits.close()

        # Return
        return

    def test_fits_table(self):
        """
        Test GFitsTable class interface.
        """
        # Set test file names
        file1 = gammalib.GFilename("test_python_fits_table_v1.fits")
        file2 = gammalib.GFilename("test_python_fits_table_v2.fits")

        # Remove test files
        file1.remove()
        file2.remove()

        # Create FITS file
        fits = gammalib.GFits(file1, True)

        # Create table columns
        nrows = 10
        col1 = gammalib.GFitsTableBitCol("BIT", nrows)
        col2 = gammalib.GFitsTableBoolCol("BOOLEAN", nrows)
        col3 = gammalib.GFitsTableByteCol("BYTE", nrows)
        col4 = gammalib.GFitsTableDoubleCol("DOUBLE", nrows)
        col5 = gammalib.GFitsTableFloatCol("FLOAT", nrows)
        col6 = gammalib.GFitsTableLongCol("LONG", nrows)
        col7 = gammalib.GFitsTableLongLongCol("LONGLONG", nrows)
        col8 = gammalib.GFitsTableShortCol("SHORT", nrows)
        col9 = gammalib.GFitsTableStringCol("STRING", nrows, 20)
        col10 = gammalib.GFitsTableULongCol("ULONG", nrows)
        col11 = gammalib.GFitsTableUShortCol("USHORT", nrows)
        for i in range(nrows):
            #col1[i] = i % 2        # Old swig version
            #col2[i] = i % 2        # Old swig version
            col1[i] = bool(i % 2)  # New swig version (3.x.y)
            col2[i] = bool(i % 2)  # New swig version (3.x.y)
            col3[i] = i
            col4[i] = i * 0.01
            col5[i] = i * 0.01
            col6[i] = i * 100
            col7[i] = i * 10000
            col8[i] = i * 100
            col9[i] = str(i * 100)
            col10[i] = i * 100
            col11[i] = i * 100

        # Set ASCII table
        tbl_ascii = gammalib.GFitsAsciiTable()
        # tbl_ascii.append(col1) # Need to implement ?/!
        # tbl_ascii.append(col2) # Need to implement ?/!
        tbl_ascii.append(col3)
        tbl_ascii.append(col4)
        tbl_ascii.append(col5)
        tbl_ascii.append(col6)
        tbl_ascii.append(col7)
        tbl_ascii.append(col8)
        tbl_ascii.append(col9)
        tbl_ascii.append(col10)
        tbl_ascii.append(col11)
        tbl_ascii.extname("ASCII table")
        fits.append(tbl_ascii)

        # Set binary table
        tbl_bin = gammalib.GFitsBinTable()
        tbl_bin.append(col1)
        tbl_bin.append(col2)
        tbl_bin.append(col3)
        tbl_bin.append(col4)
        tbl_bin.append(col5)
        tbl_bin.append(col6)
        tbl_bin.append(col7)
        tbl_bin.append(col8)
        tbl_bin.append(col9)
        tbl_bin.append(col10)
        tbl_bin.append(col11)
        tbl_bin.extname("Binary table")
        fits.append(tbl_bin)

        # Save FITS file
        fits.save()

        # Close FITS file
        fits.close()

        # Return
        return

    def test_fits_table_columns(self):
        """
        Test FITS table columns
        """
        # Create table columns
        nrows = 10
        col1  = gammalib.GFitsTableBitCol("BIT", nrows)
        col2  = gammalib.GFitsTableBoolCol("BOOLEAN", nrows)
        col3  = gammalib.GFitsTableByteCol("BYTE", nrows)
        col4  = gammalib.GFitsTableDoubleCol("DOUBLE", nrows)
        col5  = gammalib.GFitsTableFloatCol("FLOAT", nrows)
        col6  = gammalib.GFitsTableLongCol("LONG", nrows)
        col7  = gammalib.GFitsTableLongLongCol("LONGLONG", nrows)
        col8  = gammalib.GFitsTableShortCol("SHORT", nrows)
        col9  = gammalib.GFitsTableStringCol("STRING", nrows, 20)
        col10 = gammalib.GFitsTableULongCol("ULONG", nrows)
        col11 = gammalib.GFitsTableUShortCol("USHORT", nrows)

        # Test iterators
        for row in col1:
            pass
        for row in col2:
            pass
        for row in col3:
            pass
        for row in col4:
            pass
        for row in col5:
            pass
        for row in col6:
            pass
        for row in col7:
            pass
        for row in col8:
            pass
        for row in col9:
            pass
        for row in col10:
            pass
        for row in col11:
            pass

        # Return
        return
        