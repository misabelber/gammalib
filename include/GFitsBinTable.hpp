/***************************************************************************
 *              GFitsBinTable.hpp  - FITS binary table class               *
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
 * @file GFitsBinTable.hpp
 * @brief GFitsBinTable class definition.
 * @author J. Knodlseder
 */

#ifndef GFITSBINTABLE_HPP
#define GFITSBINTABLE_HPP

/* __ Includes ___________________________________________________________ */
#include "GFitsTable.hpp"


/***********************************************************************//**
 * @class GFitsBinTable
 *
 * @brief Interface for FITS binary table
 ***************************************************************************/
class GFitsBinTable : public GFitsTable {

public:
    // Constructors and destructors
    GFitsBinTable(void);
    GFitsBinTable(int nrows);
    GFitsBinTable(const GFitsBinTable& table);
    virtual ~GFitsBinTable(void);

    // Operators
    GFitsBinTable& operator= (const GFitsBinTable& table);

    // Implemented pure virtual methods
    HDUType exttype(void) const { return HT_BIN_TABLE; }

    // Methods

private:
    // Private methods
    void           init_members(void);
    void           copy_members(const GFitsBinTable& table);
    void           free_members(void);
    GFitsBinTable* clone(void) const;

    // Private data area
};

#endif /* GFITSBINTABLE_HPP */
