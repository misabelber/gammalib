/***************************************************************************
 *  GFitsTableCFloatCol.hpp  - FITS table single precision complex column  *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2008-2011 by Jurgen Knodlseder                           *
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
 * @file GFitsTableCFloatCol.hpp
 * @brief FITS table float complex column class interface definition
 * @author J. Knodlseder
 */

#ifndef GFITSTABLECFLOATCOL_HPP
#define GFITSTABLECFLOATCOL_HPP

/* __ Includes ___________________________________________________________ */
#include <string>
#include "GFits.hpp"
#include "GFitsTableCol.hpp"


/***********************************************************************//**
 * @class GFitsTableCFloatCol
 *
 * @brief FITS table float complex column
 *
 * This class implements a FITS table float complex column.
 ***************************************************************************/
class GFitsTableCFloatCol : public GFitsTableCol {

public:
    // Constructors and destructors
    GFitsTableCFloatCol(void);
    GFitsTableCFloatCol(const std::string& name, const int& length,
                         const int& size = 1);
    GFitsTableCFloatCol(const GFitsTableCFloatCol& column);
    virtual ~GFitsTableCFloatCol(void);

    // Operators
    GFitsTableCFloatCol& operator= (const GFitsTableCFloatCol& column);
    GFits::cfloat&        operator() (const int& row, const int& inx = 0);
    const GFits::cfloat&  operator() (const int& row, const int& inx = 0) const;

    // Implement virtual methods
    virtual std::string string(const int& row, const int& col = 0);
    virtual double      real(const int& row, const int& col = 0);
    virtual int         integer(const int& row, const int& col = 0);
    virtual void        insert(const int& rownum, const int& nrows);
    virtual void        remove(const int& rownum, const int& nrows);
    
    // Other methods
    GFits::cfloat* data(void) { return m_data; }
    void           nulval(const GFits::cfloat* value);
    GFits::cfloat* nulval(void) { return m_nulval; }

private:
    // Private methods
    void                  init_members(void);
    void                  copy_members(const GFitsTableCFloatCol& column);
    void                  free_members(void);
    GFitsTableCFloatCol* clone(void) const;
    std::string           ascii_format(void) const;
    std::string           binary_format(void) const;
    void                  alloc_data(void);
    void                  release_data(void);
    void                  alloc_nulval(const GFits::cfloat* value);
    void                  init_data(void);
    void*                 ptr_data(void) { return m_data; }
    void*                 ptr_nulval(void) { return m_nulval; }

    // Private data area
    GFits::cfloat* m_data;       //!< Data vector
    GFits::cfloat* m_nulval;     //!< NULL value
};

#endif /* GFITSTABLECFLOATCOL_HPP */
