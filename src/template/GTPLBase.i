/***************************************************************************
 *                         GTPLBase.i - [WHAT] class                       *
 * ----------------------------------------------------------------------- *
 *  copyright (C) [YEAR] by [AUTHOR]                                       *
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
 * @file GTPLBase.i
 * @brief [WHAT] class definition
 * @author [AUTHOR]
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GTPLBase.hpp"
%}


/***********************************************************************//**
 * @class GTPLBase
 *
 * @brief [WHAT] class
 ***************************************************************************/
class GTPLBase : public GBase {

public:
    // Constructors and destructors
    GTPLBase(void);
    GTPLBase(const GTPLBase& TPL_OBJECT);
    virtual ~GTPLBase(void);

    // Implemented pure virtual base class methods
    virtual void        clear(void);
    virtual GTPLBase*   clone(void) const;
    virtual std::string classname(void) const;

    // Other methods
    // TODO: Copy methods from GTPLBase.hpp file
};


/***********************************************************************//**
 * @brief GTPLBase class extension
 ***************************************************************************/
%extend GTPLBase {
    GTPLBase copy() {
        return (*self);
    }
};
