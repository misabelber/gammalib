/***************************************************************************
 *          GCTAPsfTable.i - CTA point spread function table class         *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2016 by Juergen Knoedlseder                              *
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
 * @file GCTAPsfTable.i
 * @brief CTA point spread function table class definition
 * @author Juergen Knoedlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GCTAPsfTable.hpp"
%}


/***********************************************************************//**
 * @class GCTAPsfTable
 *
 * @brief CTA point spread function table class
 ***************************************************************************/
class GCTAPsfTable : public GCTAPsf {

public:
    // Constructors and destructors
    GCTAPsfTable(void);
    explicit GCTAPsfTable(const GFilename& filename);
    GCTAPsfTable(const GCTAPsfTable& psf);
    virtual ~GCTAPsfTable(void);

    // Operators
    double operator()(const double& delta,
                      const double& logE, 
                      const double& theta = 0.0, 
                      const double& phi = 0.0,
                      const double& zenith = 0.0,
                      const double& azimuth = 0.0,
                      const bool&   etrue = true) const;

    // Implemented pure virtual methods
    void          clear(void);
    GCTAPsfTable* clone(void) const;
    std::string   classname(void) const;
    void          load(const GFilename& filename);
    GFilename     filename(void) const;
    double        mc(GRan&         ran,
                     const double& logE,
                     const double& theta = 0.0,
                     const double& phi = 0.0,
                     const double& zenith = 0.0,
                     const double& azimuth = 0.0,
                     const bool&   etrue = true) const;
    double        delta_max(const double& logE,
                            const double& theta = 0.0,
                            const double& phi = 0.0,
                            const double& zenith = 0.0,
                            const double& azimuth = 0.0,
                            const bool&   etrue = true) const;
    double        containment_radius(const double& fraction,
                                     const double& logE,
                                     const double& theta = 0.0,
                                     const double& phi = 0.0,
                                     const double& zenith = 0.0,
                                     const double& azimuth = 0.0,
                                     const bool&   etrue = true) const;
    // Methods
    const GCTAResponseTable& table(void) const;
    void                     table(const GCTAResponseTable& table);
    void                     read(const GFitsTable& table);
    void                     write(GFitsBinTable& table) const;
    void                     save(const GFilename& filename,
                                  const bool&      clobber = false) const;
};


/***********************************************************************//**
 * @brief GCTAPsfTable class extension
 ***************************************************************************/
%extend GCTAPsfTable {
    GCTAPsfTable copy() {
        return (*self);
    }
};
