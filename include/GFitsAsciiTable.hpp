/***************************************************************************
 *             GFitsAsciiTable.hpp  - FITS ASCII table class               *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2008-2010 by Jurgen Knodlseder                           *
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
 * @file GFitsAsciiTable.hpp
 * @brief GFitsAsciiTable class definition.
 * @author J. Knodlseder
 */

#ifndef GFITSASCIITABLE_HPP
#define GFITSASCIITABLE_HPP

/* __ Includes ___________________________________________________________ */
#include "GFitsTable.hpp"


/***********************************************************************//**
 * @class GFitsAsciiTable
 *
 * @brief Interface for FITS binary table
 *
 * The following ASCII tables are supported: 
 * TSTRING (A),
 * TLONG (I),
 * TDOUBLE (F,D)
 * TFLOAT (E)
 ***************************************************************************/
class GFitsAsciiTable : public GFitsTable {

public:
    // Constructors and destructors
    GFitsAsciiTable(void);
    GFitsAsciiTable(int nrows);
    GFitsAsciiTable(const GFitsAsciiTable& table);
    virtual ~GFitsAsciiTable(void);

    // Operators
    GFitsAsciiTable& operator= (const GFitsAsciiTable& table);

    // Implemented pure virtual methods
    HDUType exttype(void) const { return HT_ASCII_TABLE; }

    // Methods

private:
    // Private methods
    void             init_members(void);
    void             copy_members(const GFitsAsciiTable& table);
    void             free_members(void);
    GFitsAsciiTable* clone(void) const;

    // Private data area
};

#endif /* GFITSASCIITABLE_HPP */
