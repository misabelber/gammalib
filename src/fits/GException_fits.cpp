/***************************************************************************
 *                GException_fits.cpp - FITS exception handlers            *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2006-2013 by Jurgen Knodlseder                           *
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

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "GException.hpp"
#include "GTools.hpp"
#include "GFitsCfitsio.hpp"


/***********************************************************************//**
 * @brief General FITS error
 *
 * @param[in] origin Method that throws the error.
 * @param[in] status cfitsio status.
 * @param[in] message Optional error message.
 ***************************************************************************/
GException::fits_error::fits_error(const std::string& origin, 
                                   const int&         status,
                                   const std::string& message)
{
    // Set origin
    m_origin  = origin;

    // Set FITS message
    char err_text[31];
    __ffgerr(status, err_text);
    m_message  = std::string(err_text);
    m_message += " (status=" + gammalib::str(status) + ").";

    // Set message
    /*
    std::ostringstream s_error;
    char err_text[31];
    __ffgerr(status, err_text);
    s_error << err_text << " (status=" << status << ")";
    m_message = s_error.str();
    */

    // Add optional error message
    if (message.length() > 0) {
        m_message += " " + message;
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: unable to open FITS file
 *
 * @param[in] origin Method that throws the error.
 * @param[in] filename Name of the file for which opening was attempted.
 * @param[in] status cfitsio status.
 * @param[in] message Optional error message.																		
 ***************************************************************************/
GException::fits_open_error::fits_open_error(std::string origin,
                                             std::string filename,
                                             int         status,
											 std::string message)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "Unable to open FITS file \"" + filename + "\"";

    // Add status
    if (status != 0)
        m_message += " (status=" + gammalib::str(status) + ")";

    // Add optional error message
    if (message.length() > 0)
        m_message += ". " + message;

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: attempted to overwrite FITS file
 *
 * @param[in] origin Method that throws the error.
 * @param[in] filename Name of the file for which overwrite attempt was made.
 * @param[in] status cfitsio status.
 ***************************************************************************/
GException::fits_file_exist::fits_file_exist(std::string origin,
                                             std::string filename,
                                             int         status)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "Attempted to overwrite FITS file \"" + filename + "\"";

    // Add status
    if (status != 0)
        m_message += " (status=" + gammalib::str(status) + ")";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: file not open
 *
 * @param[in] origin Method that throws the error.
 * @param[in] message Optional error message.
 ***************************************************************************/
GException::fits_file_not_open::fits_file_not_open(std::string origin,
                                                   std::string message)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "FITS file not open";


    // Add optional error message
    if (message.length() > 0)
        m_message += ". " + message;

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: file already open
 *
 * @param[in] origin Method that throws the error.
 * @param[in] filename Name of file that is already open.
 ***************************************************************************/
GException::fits_already_opened::fits_already_opened(std::string origin,
                                                     std::string filename)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "FITS file \"" + filename + "\" is already open";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: Table column not found
 *
 * @param[in] origin Method that throws the error.
 * @param[in] colname Name of the table column that was not found.
 * @param[in] status cfitsio status.
 ***************************************************************************/
GException::fits_column_not_found::fits_column_not_found(std::string origin,
                                                         std::string colname,
                                                         int         status)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "Column \"" + colname + "\" not found in table";

    // Add status
    if (status != 0)
        m_message += " (status=" + gammalib::str(status) + ")";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: No header
 *
 * @param[in] origin Method that throws the error.
 * @param[in] message Error message.
 * @param[in] status cfitsio status.
 ***************************************************************************/
GException::fits_no_header::fits_no_header(std::string origin,
                                           std::string message,
                                           int         status)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = message;

    // Add status
    if (status != 0)
        m_message += " (status=" + gammalib::str(status) + ")";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: No data
 *
 * @param[in] origin Method that throws the error.
 * @param[in] message Error message.
 * @param[in] status cfitsio status.
 ***************************************************************************/
GException::fits_no_data::fits_no_data(std::string origin,
                                       std::string message,
                                       int         status)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = message;

    // Add status
    if (status != 0)
        m_message += " (status=" + gammalib::str(status) + ")";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: HDU not found in FITS file
 *
 * @param[in] origin Method that throws the error.
 * @param[in] extname Name of the extension that was not found.
 * @param[in] status cfitsio status.
 ***************************************************************************/
GException::fits_hdu_not_found::fits_hdu_not_found(std::string origin,
                                                   std::string extname,
                                                   int         status)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "HDU \"" + extname + "\" not found in FITS file";

    // Add status
    if (status != 0)
        m_message += " (status=" + gammalib::str(status) + ")";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: HDU not found in FITS file
 *
 * @param[in] origin Method that throws the error.
 * @param[in] extno Number of the extension that was not found.
 * @param[in] status cfitsio status.
 ***************************************************************************/
GException::fits_hdu_not_found::fits_hdu_not_found(std::string origin,
                                                   int         extno,
                                                   int         status)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "HDU number " + gammalib::str(extno) + " not found in FITS file";

    // Add status
    if (status != 0)
        m_message += " (status=" + gammalib::str(status) + ")";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: HDU not an image
 *
 * @param[in] origin Method that throws the error.
 * @param[in] extname Extension name.
 * @param[in] type Specified HDU type.
 ***************************************************************************/
GException::fits_hdu_not_image::fits_hdu_not_image(std::string origin,
                                                   std::string extname,
                                                   int         type)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "HDU \""+extname+"\" is not an image (type="+gammalib::str(type)+")";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: HDU not a table
 *
 * @param[in] origin Method that throws the error.
 * @param[in] extname Extension name.
 * @param[in] type Specified HDU type.
 ***************************************************************************/
GException::fits_hdu_not_table::fits_hdu_not_table(std::string origin,
                                                   std::string extname,
                                                   int         type)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "HDU \""+extname+"\" is not a table (type="+gammalib::str(type)+")";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: HDU of unknown type found
 *
 * @param[in] origin Method that throws the error.
 * @param[in] type Specified HDU type.
 ***************************************************************************/
GException::fits_unknown_HDU_type::fits_unknown_HDU_type(std::string origin,
                                                         int         type)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "HDU type (type="+gammalib::str(type)+") is not defined";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: Invalid type
 *
 * @param[in] origin Method that throws the error.
 * @param[in] message Error message.
 ***************************************************************************/
GException::fits_invalid_type::fits_invalid_type(std::string origin,
                                                 std::string message)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = message;

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: Table type is unknow
 *
 * @param[in] origin Method that throws the error.
 * @param[in] type Specified table type.
 ***************************************************************************/
GException::fits_unknown_tabtype::fits_unknown_tabtype(std::string origin,
                                                       int         type)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "Table type \"" + gammalib::str(type) + "\" is unknown";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: Column type is unknown
 *
 * @param[in] origin Method that throws the error.
 * @param[in] colname Column name.
 * @param[in] type Specified column type.
 ***************************************************************************/
GException::fits_unknown_coltype::fits_unknown_coltype(std::string origin,
                                                       std::string colname,
                                                       int         type)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "Column \""+colname+"\" has unsupported typecode="+gammalib::str(type);

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: Bad column length
 *
 * @param[in] origin Method that throws the error.
 * @param[in] length Length of the column.
 * @param[in] rows Number of rows in table.
 ***************************************************************************/
GException::fits_bad_col_length::fits_bad_col_length(std::string origin,
                                                     int         length,
                                                     int         rows)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "Column length (" + gammalib::str(length) + ") is not compatible"
                " with the number of rows (" + gammalib::str(rows) + ") in the"
                " table";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: invalid number of bits per pixel
 *
 * @param[in] origin Method that throws the error.
 * @param[in] bitpix Bitpix value that was not 8,16,32,64,-32, or -64.
 ***************************************************************************/
GException::fits_bad_bitpix::fits_bad_bitpix(std::string origin,
                                             int         bitpix)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "Invalid number of bits per pixel (bitpix="+gammalib::str(bitpix)+")";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: wrong image operator has been used
 *
 * @param[in] origin Method that throws the error.
 * @param[in] naxis Dimension of image.
 * @param[in] nargs Number of arguments of the image operator.
 ***************************************************************************/
GException::fits_wrong_image_operator::fits_wrong_image_operator(std::string origin,
                                                                 int         naxis,
                                                                 int         nargs)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message = "Wrong image pixel access operator has been used (dimension=" +
                gammalib::str(naxis) + " < arguments=" + gammalib::str(nargs) + ")";

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: Invalid row number
 *
 * @param[in] origin Method that throws the error.
 * @param[in] row Row number.
 * @param[in] nrows Number of rows in table.
 * @param[in] message Error message.
 ***************************************************************************/
GException::fits_invalid_row::fits_invalid_row(std::string origin,
                                               int         row,
                                               int         nrows,
                                               std::string message)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message  = "Invalid row number "+gammalib::str(row)+" specified";
    m_message += " (must be within [0,"+gammalib::str(nrows)+"]).";

    // Set optional message
    if (message.length() > 0)
        m_message += " "+message;

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: Invalid number of rows
 *
 * @param[in] origin Method that throws the error.
 * @param[in] nrows Number of rows.
 * @param[in] max_rows Maximum number of rows allowed.
 * @param[in] message Error message.
 ***************************************************************************/
GException::fits_invalid_nrows::fits_invalid_nrows(std::string origin,
                                                   int         nrows,
                                                   int         max_rows,
                                                   std::string message)
{
    // Set origin
    m_origin  = origin;

    // Set message
    m_message  = "Invalid number of rows ("+gammalib::str(nrows)+") specified";
    m_message += " (must be within [0,"+gammalib::str(max_rows)+"]).";

    // Set optional message
    if (message.length() > 0)
        m_message += " "+message;

    // Return
    return;
}


/***********************************************************************//**
 * @brief FITS error: Inconsistent TDIM information
 *
 * @param[in] origin Method that throws the error.
 * @param[in] tdim Number of rows.
 * @param[in] number Expected total number of elements.
 * @param[in] message Error message.
 ***************************************************************************/
GException::fits_inconsistent_tdim::fits_inconsistent_tdim(std::string      origin,
                                                           std::vector<int> tdim,
                                                           int              number,
                                                           std::string      message)
{
    // Set origin
    m_origin  = origin;

    // Set message
    if (tdim.size() < 1) {
        m_message  = "Empty TDIM keyword encountered.";
    }
    else {
        // Compute expectation
        std::string sdim = "("+gammalib::str(tdim[0]);
        int         num  = tdim[0];
        for (int k = 1; k < tdim.size(); ++k) {
            sdim += ","+gammalib::str(tdim[k]);
            num  *= tdim[k];
        }
        sdim += ")";
        
        // Set message
        m_message  = "TDIM keyword "+sdim+" predicts "+gammalib::str(num)+" column"
                     " elements, while there are "+gammalib::str(number)+" elements"
                     " in the column.";
    }

    // Set optional message
    if (message.length() > 0)
        m_message += " "+message;

    // Return
    return;
}


