/***************************************************************************
 *          GFitsTableBoolCol.cpp - FITS table boolean column class        *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2017 by Juergen Knoedlseder                         *
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
 * @file GFitsTableBoolCol.cpp
 * @brief FITS table boolean column class implementation
 * @author Juergen Knoedlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <string>
#include "GException.hpp"
#include "GTools.hpp"
#include "GFitsCfitsio.hpp"
#include "GFitsTableBoolCol.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_INSERT                      "GFitsTableBoolCol::insert(int&, int&)"
#define G_REMOVE                      "GFitsTableBoolCol::remove(int&, int&)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                        Constructors/destructors                         =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Constructor
 ***************************************************************************/
GFitsTableBoolCol::GFitsTableBoolCol(void) : GFitsTableCol()
{
    // Initialise class members for clean destruction
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Constructor
 *
 * @param[in] name Name of column.
 * @param[in] nrows Number of rows in column.
 * @param[in] size Vector size of column.
 ***************************************************************************/
GFitsTableBoolCol::GFitsTableBoolCol(const std::string& name,
                                     const int&         nrows,
                                     const int&         size) :
                   GFitsTableCol(name, nrows, size, 1)
{
    // Initialise class members for clean destruction
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] column Table column.
 ***************************************************************************/
GFitsTableBoolCol::GFitsTableBoolCol(const GFitsTableBoolCol& column) :
                   GFitsTableCol(column)
{
    // Initialise class members for clean destruction
    init_members();

    // Copy members
    copy_members(column);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GFitsTableBoolCol::~GFitsTableBoolCol(void)
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                               Operators                                 =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] column Table column.
 * @return Table column.
 ***************************************************************************/
GFitsTableBoolCol& GFitsTableBoolCol::operator=(const GFitsTableBoolCol& column)
{
    // Execute only if object is not identical
    if (this != &column) {

        // Copy base class members
        this->GFitsTableCol::operator=(column);

        // Free members
        free_members();

        // Initialise private members for clean destruction
        init_members();

        // Copy members
        copy_members(column);

    } // endif: object was not identical

    // Return this object
    return *this;
}


/***********************************************************************//**
 * @brief Column data access operator
 *
 * @param[in] row Row of column to access.
 * @param[in] inx Vector index in column row to access.
 *
 * Provides access to data in a column.
 ***************************************************************************/
bool& GFitsTableBoolCol::operator()(const int& row, const int& inx)
{
    // If data are not available then load them now
    if (m_data == NULL) fetch_data();

    // Return data bin
    return m_data[offset(row, inx)];
}


/***********************************************************************//**
 * @brief Column data access operator (const variant)
 *
 * @param[in] row Row of column to access.
 * @param[in] inx Vector index in column row to access
 *
 * Provides access to data in a column.
 ***************************************************************************/
const bool& GFitsTableBoolCol::operator()(const int& row, const int& inx) const
{
    // If data are not available then load them now
    if (m_data == NULL) fetch_data();

    // Return data bin
    return m_data[offset(row, inx)];
}


/*==========================================================================
 =                                                                         =
 =                             Public methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Clear instance
 *
 * This method properly resets the object to an initial state.
 ***************************************************************************/
void GFitsTableBoolCol::clear(void)
{
    // Free class members (base and derived classes, derived class first)
    free_members();
    this->GFitsTableCol::free_members();

    // Initialise members
    this->GFitsTableCol::init_members();
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone column
 ***************************************************************************/
GFitsTableBoolCol* GFitsTableBoolCol::clone(void) const
{
    return new GFitsTableBoolCol(*this);
}


/***********************************************************************//**
 * @brief Get string value
 *
 * @param[in] row Table row.
 * @param[in] inx Table column vector index.
 *
 * Returns value of specified row and vector index as string.
 ***************************************************************************/
std::string GFitsTableBoolCol::string(const int& row, const int& inx) const
{
    // If data are not available then load them now
    if (m_data == NULL) fetch_data();

    // Convert bool into string
    std::string result = (m_data[offset(row,inx)]) ? "T" : "F";

    // Return value
    return result;
}


/***********************************************************************//**
 * @brief Get double precision value
 *
 * @param[in] row Table row.
 * @param[in] inx Table column vector index.
 *
 * Returns value of specified row and vector index as double precision.
 ***************************************************************************/
double GFitsTableBoolCol::real(const int& row, const int& inx) const
{
    // If data are not available then load them now
    if (m_data == NULL) fetch_data();

    // Convert bool into double
    double value = (double)m_data[offset(row,inx)];

    // Return value
    return value;
}


/***********************************************************************//**
 * @brief Get integer value
 *
 * @param[in] row Table row.
 * @param[in] inx Table column vector index.
 *
 * Returns value of specified row and vector index as integer.
 ***************************************************************************/
int GFitsTableBoolCol::integer(const int& row, const int& inx) const
{
    // If data are not available then load them now
    if (m_data == NULL) fetch_data();

    // Convert bool into int
    int value = (int)m_data[offset(row,inx)];

    // Return value
    return value;
}


/***********************************************************************//**
 * @brief Insert rows in column
 *
 * @param[in] row Row after which rows should be inserted (0=first row).
 * @param[in] nrows Number of rows to be inserted.
 *
 * @exception GException::fits_invalid_row
 *            Specified row is invalid.
 *
 * Inserts rows into a FITS table. This implies that the column will be
 * loaded into memory.
 ***************************************************************************/
void GFitsTableBoolCol::insert(const int& row, const int& nrows)
{
    // Make sure that row is valid
    if (row < 0 || row > m_length) {
        throw GException::fits_invalid_row(G_INSERT, row, m_length);
    }
    
    // Continue only if there are rows to be inserted
    if (nrows > 0) {
    
        // If we have no rows yet then simply set the length to the
        // number of rows to be inserted
        if (m_length == 0) {
            m_length = nrows;
            m_size   = m_number * m_length;
            alloc_data();
            init_data();
        }
        
        // ... otherwise fetch data, allocate new data and copy over
        // the existing items
        else {

            // If data are not available then load them now
            if (m_data == NULL) fetch_data();

            // Compute new column length
            int length = m_length + nrows;

            // Calculate size of new memory
            m_size = m_number * length;
        
            // Allocate new data to hold the column
            bool* new_data = new bool[m_size];

            // Compute the number of elements before the insertion point,
            // the number of elements that get inserted, and the total
            // number of elements after the insertion point
            int n_before = m_number * row;
            int n_insert = m_number * nrows;
            int n_after  = m_number * (m_length - row);
        
            // Copy and initialise data
            bool* src = m_data;
            bool* dst = new_data;
            for (int i = 0; i < n_before; ++i) {
                *dst++ = *src++;
            }
            for (int i = 0; i < n_insert; ++i) {
                *dst++ = 0;
            }
            for (int i = 0; i < n_after; ++i) {
                *dst++ = *src++;
            }
        
            // Free old data
            if (m_data != NULL) delete [] m_data;
            
            // Set pointer to new data and store length
            m_data   = new_data;
            m_length = length;
        
        } // endelse: there were already data
    
    } // endfor: there were rows to be inserted

    // Return
    return;
}


/***********************************************************************//**
 * @brief Remove rows from column
 *
 * @param[in] row Row after which rows should be removed (0=first row).
 * @param[in] nrows Number of rows to be removed.
 *
 * @exception GException::fits_invalid_row
 *            Specified row is invalid.
 * @exception GException::fits_invalid_nrows
 *            Invalid number of rows specified.
 *
 * This method removes rows from a FITS table. This implies that the column
 * will be loaded into memory.
 ***************************************************************************/
void GFitsTableBoolCol::remove(const int& row, const int& nrows)
{
    // Make sure that row is valid
    if (row < 0 || row >= m_length) {
        throw GException::fits_invalid_row(G_REMOVE, row, m_length-1);
    }
    
    // Make sure that we don't remove beyond the limit
    if (nrows < 0 || nrows > m_length-row) {
        throw GException::fits_invalid_nrows(G_REMOVE, nrows, m_length-row);
    }
    
    // Continue only if there are rows to be removed
    if (nrows > 0) {
    
        // If data are not available then load them now
        if (m_data == NULL) fetch_data();

        // Compute new column length
        int length = m_length - nrows;
        
        // Calculate size of new memory
        m_size = m_number * length;

        // If we have rows remaining then allocate new data to hold
        // the column
        if (m_size > 0) {
        
            // Allocate new data to hold the column
            bool* new_data = new bool[m_size];

            // Compute the number of elements before the removal point,
            // the number of elements that get removed, and the total
            // number of elements after the removal point
            int n_before = m_number * row;
            int n_remove = m_number * nrows;
            int n_after  = m_number * (length - row);

            // Copy data
            bool* src = m_data;
            bool* dst = new_data;
            for (int i = 0; i < n_before; ++i) {
                *dst++ = *src++;
            }
            src += n_remove;
            for (int i = 0; i < n_after; ++i) {
                *dst++ = *src++;
            }
        
            // Free old data
            if (m_data != NULL) delete [] m_data;
            
            // Set pointer to new data and store length
            m_data   = new_data;
            m_length = length;
        
        } // endif: there are still elements after removal
        
        // ... otherwise just remove all data
        else {

            // Free old data
            if (m_data != NULL) delete [] m_data;

            // Set pointer to new data and store length
            m_data   = NULL;
            m_length = length;
        }
    
    } // endfor: there were rows to be removed

    // Return
    return;
}


/***********************************************************************//**
 * @brief Set nul value
 *
 * @param[in] value Nul value.
 *
 * @todo To correctly reflect the nul value in the data, the column should
 * be reloaded. However, the column may have been changed, so in principle
 * saving is needed. However, we may not want to store the data, hence saving
 * is also not desired. We thus have to develop a method to update the
 * column information for a new nul value in place ...
 ***************************************************************************/
void GFitsTableBoolCol::nulval(const bool* value)
{
    // Allocate nul value
    alloc_nulval(value);

    // Update column
//    if (m_data != NULL) {
//        save();
//        load();
//    }

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                             Private methods                             =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GFitsTableBoolCol::init_members(void)
{
    // Initialise members
    m_type   = __TLOGICAL;
    m_data   = NULL;
    m_buffer = NULL;
    m_nulval = NULL;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] column Column.
 *
 * Sets the content of the vector column by copying from another column.
 * If the code is compiled with the small memory option, and if the source
 * column has not yet been loaded, then we only load the column temporarily
 * for copying purposes and release it again once copying is finished.
 ***************************************************************************/
void GFitsTableBoolCol::copy_members(const GFitsTableBoolCol& column)
{
    // Fetch data if necessary
    bool not_loaded = (!column.is_loaded());
    if (not_loaded) {
        column.fetch_data();
    }

    // Copy attributes
    m_type     = column.m_type;
    m_size     = column.m_size;
    m_varlen   = column.m_varlen;
    m_rowstart = column.m_rowstart;

    // Copy column data
    if (column.m_data != NULL && m_size > 0) {
        if (m_data != NULL) delete [] m_data;
        m_data = new bool[m_size];
        for (int i = 0; i < m_size; ++i) {
            m_data[i] = column.m_data[i];
        }
    }

    // Copy NULL value
    alloc_nulval(column.m_nulval);

    // Small memory option: release column if it was fetch above
    #if defined(G_SMALL_MEMORY)
    if (not_loaded) {
        const_cast<GFitsTableBoolCol*>(&column)->release_data();
    }
    #endif

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GFitsTableBoolCol::free_members(void)
{
    // Free memory
    if (m_data   != NULL) delete [] m_data;
    if (m_nulval != NULL) delete m_nulval;

    // Mark memory as freed
    m_data   = NULL;
    m_nulval = NULL;

    // Reset load flag
    m_size = 0;

    // Free buffer
    free_buffer();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Save table column into FITS file
 *
 * The table column is only saved if it is linked to a FITS file and if the
 * data are indeed present in the class instance. This avoids saving of data
 * that have not been modified.
 *
 * Refer to GFitsTableCol::save_column() for more information.
 ***************************************************************************/
void GFitsTableBoolCol::save(void)
{
    // Free buffer
    free_buffer();

    // Allocate buffer
    alloc_buffer();

    // Transfer string into buffer
    for (int i = 0; i < m_size; ++i) {
        m_buffer[i] = (char)m_data[i];
    }

    // Save column
    save_column();

    // Free buffer
    free_buffer();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Returns format string of ASCII table
 ***************************************************************************/
std::string GFitsTableBoolCol::ascii_format(void) const
{
    // Initialize format string
    std::string format;

    // Set type code
    format.append("L");

    // Set width
    format.append(gammalib::str(m_width));

    // Return format
    return format;
}


/***********************************************************************//**
 * @brief Allocates column data
 ***************************************************************************/
void GFitsTableBoolCol::alloc_data(void)
{
    // Free any existing memory
    if (m_data != NULL) delete [] m_data;

    // Mark pointer as free
    m_data = NULL;

    // Allocate new data
    if (m_size > 0) {
        m_data = new bool[m_size];
    }

    // Allocate also buffer
    free_buffer();
    alloc_buffer();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Fetch column data
 *
 * If a FITS file is attached to the column the data are loaded into memory
 * from the FITS file. If no FITS file is attached, memory is allocated
 * to hold the column data and all cells are set to 0.
 *
 * Refer to GFitsTableCol::load_column for more information.
 ***************************************************************************/
void GFitsTableBoolCol::fetch_data(void) const
{
    // Load column
    const_cast<GFitsTableBoolCol*>(this)->load_column();

    // Extract values from buffer
    for (int i = 0; i < m_size; ++i) {
        m_data[i] = (bool)m_buffer[i];
    }

    // Free buffer memory
    free_buffer();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Resize column data
 *
 * @param[in] index Start index.
 * @param[in] number Number of elements to add/remove.
 *
 * Adds or removes elements from specified index on. Adding is done if
 * @p number is a positive number, removing if @p number is negative.
 * Note that the method does not change the validity of the arguments.
 * This needs to be done by the client.
 ***************************************************************************/
void GFitsTableBoolCol::resize_data(const int& index, const int& number)
{
    // Continue only if number of elements is not zero
    if (number != 0) {

        // If data are not available then load them now
        if (m_data == NULL) fetch_data();

        // If elements should be removed then do not allocate new memory
        // but just move elements forward and change the logical size of
        // memory. Only if all elements should be removed the memory is
        // released.
        if (number < 0) {
            if (m_data != NULL) {
                int    left = index - number;
                bool*  dst  = m_data + index;
                bool*  src  = m_data + left;
                int    num  = m_size - left;
                for (int i = 0; i < num; ++i) {
                    *dst++ = *src++;
                }
                m_size += number;
                if (m_size < 1) {
                    release_data();
                }
            }
        }

        // If elements should be added then allocate new memory, copy over
        // the old data and initialise the new elements
        else {
            int left       = m_size - index;
            m_size        += number;
            bool* new_data = new bool[m_size];
            bool* dst      = new_data;
            bool* src      = m_data;
            for (int i = 0; i < index; ++i) {
                *dst++ = *src++;
            }
            for (int i = 0; i < number; ++i) {
                *dst++ = false;
            }
            for (int i = 0; i < left; ++i) {
                *dst++ = *src++;
            }
            if (m_data != NULL) delete [] m_data;
            m_data = new_data;
        }

    } // endif: number was non-zero

    // Return
    return;
}


/***********************************************************************//**
 * @brief Release column data
 ***************************************************************************/
void GFitsTableBoolCol::release_data(void)
{
    // Free any existing memory
    if (m_data != NULL) delete [] m_data;

    // Mark pointer as free and reset loaded vector size
    m_data = NULL;
    m_size = 0;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Allocate null value
 ***************************************************************************/
void GFitsTableBoolCol::alloc_nulval(const bool* value)
{
    // Free any existing memory
    if (m_nulval != NULL) delete m_nulval;

    // Mark pointer as free
    m_nulval = NULL;

    // If we have valid value, allocate and set null value
    if (value != NULL) {
        m_nulval  = new bool;
        *m_nulval = *value;
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Initialise column data
 ***************************************************************************/
void GFitsTableBoolCol::init_data(void)
{
    // Initialise data if they exist
    if (m_data != NULL) {
        for (int i = 0; i < m_size; ++i) {
            m_data[i] = false;
        }
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Allocate CFITSIO transfer buffer
 *
 * The CFITSIO transfer buffer allows transparent conversion from the CFITSIO
 * storage format to a vector of bool values.
 ***************************************************************************/
void GFitsTableBoolCol::alloc_buffer(void) const
{
    // Allocate and initialise buffer memory
    if (m_size > 0) {
        m_buffer = new char[m_size];
        for (int i = 0; i < m_size; ++i) {
            m_buffer[i] = 0;
        }
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Free CFITSIO transfer buffer
 *
 * Release memory that has been allocated for the CFITSIO transfer buffer.
 ***************************************************************************/
void GFitsTableBoolCol::free_buffer(void) const
{
    // Free buffer
    if (m_buffer != NULL) delete [] m_buffer;

    // Mark buffer as free
    m_buffer = NULL;

    // Return
    return;
}
