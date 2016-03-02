/***************************************************************************
 *  GModelSpatialRadialProfileDMBurkert.i - Einasto radial profile class   *
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
 * @file GModelSpatialRadialProfileDMBurkert.hpp
 * @brief Radial Dark Matter halo for Einasto Density Profile
 * @author Nathan Kelley-Hoskins
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GModelSpatialRadialProfileDMBurkert.hpp"
%}


/**************************************************************************
 * @class GModelSpatialRadialProfileDMBurkert
 *
 * @brief Radial DM Einasto profile source model class
 *
 * This class implements the spatial component of the factorised source
 * model for a Dark Matter Einasto halo radial profile.
 ***************************************************************************/
class GModelSpatialRadialProfileDMBurkert : public GModelSpatialRadialProfile {

public:
    // Constructors and destructors
    GModelSpatialRadialProfileDMBurkert(void);
    explicit GModelSpatialRadialProfileDMBurkert(const GXmlElement& xml);
    GModelSpatialRadialProfileDMBurkert(const GModelSpatialRadialProfileDMBurkert& model);
    virtual ~GModelSpatialRadialProfileDMBurkert(void);

    // Implemented pure virtual base class methods
    virtual void                             clear(void);
    virtual GModelSpatialRadialProfileDMBurkert* clone(void) const;
    virtual std::string                      classname(void) const;
    virtual std::string                      type(void) const;
    virtual double                           theta_max(void) const;
    virtual void                             read(const GXmlElement& xml);
    virtual void                             write(GXmlElement& xml) const;
};


/***********************************************************************//**
 * @brief GModelSpatialRadialGauss class extension
 *
 * The eval(GSkyDir&) and eval_gradients(GSkyDir&) need to be defined in the
 * extension to force swig to build also the interface for these methods that
 * are implemented in the base class only. It's not clear to me why these
 * methods are not inherited automatically. Maybe this could also be handled
 * by a %typemap(typecheck) construct.
 ***************************************************************************/
%extend GModelSpatialRadialProfileDMBurkert {
    double eval(const GPhoton& photon) const {
        return self->GModelSpatialRadial::eval(photon);
    }
    double eval_gradients(const GPhoton& photon) const {
        return self->GModelSpatialRadial::eval_gradients(photon);
    }
};
