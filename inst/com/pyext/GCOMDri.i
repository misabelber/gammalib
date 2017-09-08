/***************************************************************************
 *                   GCOMDri.i - COMPTEL Data Space class                  *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2017 by Juergen Knoedlseder                              *
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
 * @file GCOMDri.i
 * @brief COMPTEL Data Space class definition
 * @author Juergen Knoedlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GCOMDri.hpp"
%}


/***********************************************************************//**
 * @class GCOMDri
 *
 * @brief COMPTEL Data Space class
 ***************************************************************************/
class GCOMDri : public GBase {

public:
    // Constructors and destructors
    GCOMDri(void);
    explicit GCOMDri(const GFilename& filename);
    GCOMDri(const GCOMDri& dri);
    virtual ~GCOMDri(void);

    // Implemented pure virtual base class methods
    virtual void        clear(void);
    virtual GCOMDri*    clone(void) const;
    virtual std::string classname(void) const;

    // Other methods
    int             size(void) const;
    int             nchi(void) const;
    int             npsi(void) const;
    int             nphibar(void) const;
    const GSkyMap&  map(void) const;
    const GEbounds& ebounds(void) const;
    void            ebounds(const GEbounds&);
    const GGti&     gti(void) const;
    void            gti(const GGti& gti);
    const double&   phimin(void) const;
    const double&   phibin(void) const;
    void            compute_dre(const GCOMEventList& events,
                                const GCOMOads&      oads,
                                const GCOMTim&       tim,
                                const double&        zeta = 5.0);
    void            compute_drg(const GCOMOads& oads,
                                const GCOMTim&  tim,
                                const double&   zeta = 5.0);
    void            compute_drx(const GCOMOads& oads,
                                const GCOMTim&  tim);
    void            load(const GFilename& filename);
    void            save(const GFilename& filename, const bool& clobber = false) const;
    void            read(const GFitsImage& image);
    void            write(GFits& fits, const std::string& extname = "") const;
};


/***********************************************************************//**
 * @brief GCOMDri class extension
 ***************************************************************************/
%extend GCOMDri {
    double __getitem__(const int& index) {
        // Counting from start, e.g. [2]
        if (index >= 0 && index < self->size()) {
            return (*self)[index];
        }
        // Counting from end, e.g. [-1]
        else if (index < 0 && self->size()+index >= 0) {
            return (*self)[self->size()+index];
        }
        else {
            throw GException::out_of_range("__getitem__(int)", index, self->size());
        }
    }
    void __setitem__(const int& index, const double& val) {
        // Counting from start, e.g. [2]
        if (index >= 0 && index < self->size()) {
            (*self)[index] = val;
        }
        // Counting from end, e.g. [-1]
        else if (index < 0 && self->size()+index >= 0) {
            (*self)[self->size()+index] = val;
        }
        else {
            throw GException::out_of_range("__setitem__(int)", index, self->size());
        }
    }
    GCOMDri copy() {
        return (*self);
    }
};
