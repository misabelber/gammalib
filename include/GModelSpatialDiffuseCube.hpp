/***************************************************************************
 *        GModelSpatialDiffuseCube.hpp - Spatial map cube model class      *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2016 by Juergen Knoedlseder                         *
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
 * @file GModelSpatialDiffuseCube.hpp
 * @brief Spatial map cube model class interface definition
 * @author Juergen Knoedlseder
 */

#ifndef GMODELSPATIALDIFFUSECUBE_HPP
#define GMODELSPATIALDIFFUSECUBE_HPP

/* __ Includes ___________________________________________________________ */
#include <string>
#include "GModelSpatialDiffuse.hpp"
#include "GModelSpectral.hpp"
#include "GModelSpectralNodes.hpp"
#include "GModelPar.hpp"
#include "GSkyDir.hpp"
#include "GSkyMap.hpp"
#include "GSkyRegionCircle.hpp"
#include "GNodeArray.hpp"
#include "GEbounds.hpp"
#include "GFilename.hpp"

/* __ Forward declarations _______________________________________________ */
class GFits;
class GEnergies;
class GXmlElement;


/***********************************************************************//**
 * @class GModelSpatialDiffuseCube
 *
 * @brief Spatial map cube model
 *
 * This class implements the spatial component of the factorised source
 * model for a map cube. A map cube is a set of sky maps for different
 * energies. It is assumed that the pixels of the sky map are given in the
 * units ph/cm2/s/sr/MeV. 
 ***************************************************************************/
class GModelSpatialDiffuseCube : public GModelSpatialDiffuse {

public:
    // Constructors and destructors
    GModelSpatialDiffuseCube(void);
    GModelSpatialDiffuseCube(const bool& dummy, const std::string& type);
    explicit GModelSpatialDiffuseCube(const GXmlElement& xml);
    GModelSpatialDiffuseCube(const GFilename& filename,
                             const double&    value = 1.0);
    GModelSpatialDiffuseCube(const GSkyMap&   cube,
                             const GEnergies& energies,
                             const double&    value = 1.0);
    GModelSpatialDiffuseCube(const GModelSpatialDiffuseCube& model);
    virtual ~GModelSpatialDiffuseCube(void);

    // Operators
    virtual GModelSpatialDiffuseCube& operator=(const GModelSpatialDiffuseCube& model);

    // Implemented pure virtual methods
    virtual void                      clear(void);
    virtual GModelSpatialDiffuseCube* clone(void) const;
    virtual std::string               classname(void) const;
    virtual std::string               type(void) const;
    virtual double                    eval(const GPhoton& photon,
                                           const bool& gradients = false) const;
    virtual GSkyDir                   mc(const GEnergy& energy,
                                         const GTime& time,
                                         GRan& ran) const;
    virtual double                    mc_norm(const GSkyDir& dir,
                                              const double&  radius) const;
    virtual bool                      contains(const GSkyDir& dir,
                                               const double&  margin = 0.0) const;
    virtual GSkyRegion*               region(void) const;
    virtual void                      read(const GXmlElement& xml);
    virtual void                      write(GXmlElement& xml) const;
    virtual std::string               print(const GChatter& chatter = NORMAL) const;

    // Other methods
    int                        maps(void) const;
    int                        pixels(void) const;
    double                     value(void) const;
    void                       value(const double& value);
    const GFilename&           filename(void) const;
    void                       filename(const GFilename& filename);
    const GSkyMap&             cube(void) const;
    void                       cube(const GSkyMap& cube);
    GEnergies                  energies(void);
    void                       energies(const GEnergies& energies);
    const GModelSpectralNodes& spectrum(void) const;
    void                       set_mc_cone(const GSkyDir& centre,
                                           const double&  radius) const;
    void                       load(const GFilename& filename);
    void                       save(const GFilename& filename,
                                    const bool&      clobber = false) const;
    void                       write(GFits& file) const;

protected:
    // Protected methods
    void   init_members(void);
    void   copy_members(const GModelSpatialDiffuseCube& model);
    void   free_members(void);
    void   fetch_cube(void) const;
    void   load_cube(const GFilename& filename);
    void   set_energy_boundaries(void);
    void   update_mc_cache(void);
    double cube_intensity(const GPhoton& photon) const;
    void   set_region(void) const;

    // Protected members
    std::string              m_type;     //!< Model type
    GModelPar                m_value;    //!< Value
    GFilename                m_filename; //!< Name of map cube
    bool                     m_loaded;   //!< Signals if map cube has been loaded
    GSkyMap                  m_cube;     //!< Map cube
    GNodeArray               m_logE;     //!< Log10(energy) values of the maps
    GEbounds                 m_ebounds;  //!< Energy bounds of the maps
    mutable GSkyRegionCircle m_region;   //!< Bounding circle

    // Monte Carlo cache
    mutable GSkyDir             m_mc_centre;           //!< Centre of MC cone
    mutable double              m_mc_radius;           //!< Radius of MC cone (degrees)
    mutable double              m_mc_one_minus_cosrad; //!< 1-cosine of radius
    mutable std::vector<double> m_mc_max;              //!< Maximum values for MC
    mutable GModelSpectralNodes m_mc_spectrum;         //!< Map cube spectrum
};


/***********************************************************************//**
 * @brief Return class name
 *
 * @return String containing the class name ("GModelSpatialDiffuseCube").
 ***************************************************************************/
inline
std::string GModelSpatialDiffuseCube::classname(void) const
{
    return ("GModelSpatialDiffuseCube");
}


/***********************************************************************//**
 * @brief Return spatial model type
 *
 * @return Model type.
 *
 * Returns the type of the spatial map cube model.
 ***************************************************************************/
inline
std::string GModelSpatialDiffuseCube::type(void) const
{
    return (m_type);
}


/***********************************************************************//**
 * @brief Return number of maps in cube
 *
 * @return Number of maps in cube.
 *
 * Returns the number of maps in the cube.
 ***************************************************************************/
inline
int GModelSpatialDiffuseCube::maps(void) const
{
    return (m_cube.nmaps());
}


/***********************************************************************//**
 * @brief Return number of pixels in cube
 *
 * @return Number of pixels in cube.
 *
 * Returns the number of pixels in the cube.
 ***************************************************************************/
inline
int GModelSpatialDiffuseCube::pixels(void) const
{
    return (m_cube.npix());
}


/***********************************************************************//**
 * @brief Get model value
 *
 * @return Model value.
 *
 * Returns the value of the spatial map cube model.
 ***************************************************************************/
inline
double GModelSpatialDiffuseCube::value(void) const
{
    return (m_value.value());
}


/***********************************************************************//**
 * @brief Set model value
 *
 * @param[in] value Model value.
 *
 * Set the value of the spatial map cube model.
 ***************************************************************************/
inline
void GModelSpatialDiffuseCube::value(const double& value)
{
    m_value.value(value);
    return;
}


/***********************************************************************//**
 * @brief Get file name
 *
 * @return File name.
 *
 * Returns the file name of the spatial map cube model.
 ***************************************************************************/
inline
const GFilename& GModelSpatialDiffuseCube::filename(void) const
{
    return (m_filename);
}


/***********************************************************************//**
 * @brief Set file name
 *
 * @param[in] filename File name.
 *
 * Set the file name of the spatial map cube model.
 ***************************************************************************/
inline
void GModelSpatialDiffuseCube::filename(const GFilename& filename)
{
    m_filename = filename;
    return;
}


/***********************************************************************//**
 * @brief Get map cube
 *
 * @return Map cube.
 *
 * Returns the map cube.
 ***************************************************************************/
inline
const GSkyMap& GModelSpatialDiffuseCube::cube(void) const
{
    return (m_cube);
}


/***********************************************************************//**
 * @brief Get map cube spectrum
 *
 * @return Map cube spectrum.
 *
 * Returns the map cube spectrum.
 ***************************************************************************/
inline
const GModelSpectralNodes& GModelSpatialDiffuseCube::spectrum(void) const
{
    return (m_mc_spectrum);
}


/***********************************************************************//**
 * @brief Return normalization of diffuse cube for Monte Carlo simulations
 *
 * @param[in] dir Centre of simulation cone.
 * @param[in] radius Radius of simulation cone (degrees).
 * @return Normalization.
 *
 * Returns the normalization of a diffuse cube. The normalization is given
 * by the model value. The @p dir and @p radius parameters are not used.
 ***************************************************************************/
inline
double GModelSpatialDiffuseCube::mc_norm(const GSkyDir& dir,
                                         const double&  radius) const
{
    return (value());
}


/***********************************************************************//**
 * @brief Return boundary sky region
 *
 * @return Boundary sky region.
 *
 * Returns a sky region that fully encloses the point source.
 ***************************************************************************/
inline
GSkyRegion* GModelSpatialDiffuseCube::region(void) const
{
    set_region();
    return (&m_region);
}

#endif /* GMODELSPATIALDIFFUSECUBE_HPP */
