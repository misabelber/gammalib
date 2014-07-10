/***************************************************************************
 *            GCTAMeanPsf.hpp - CTA point spread function cube class       *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2014 by Chia-Chun Lu                                     *
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
 * @file GCTAMeanPsf.hpp
 * @brief CTA point spread function cube class definition
 * @author Chia-Chun Lu
 */

#ifndef GCTAMEANPSF_HPP
#define GCTAMEANPSF_HPP

/* __ Includes ___________________________________________________________ */
#include <string>
#include "GBase.hpp"
#include "GFits.hpp"
#include "GSkymap.hpp"
#include "GObservations.hpp"
#include "GCTAObservation.hpp"
#include "GNodeArray.hpp"


/***********************************************************************//**
 * @class GCTAMeanPsf
 *
 * @brief class for the CTA point spread function
 *
 * This class implements a mean CTA point spread function which provides the
 * average point spread function for binned analysis as function of sky
 * position, log10 energy and delta angle between true and measured photon
 * direction.
 ***************************************************************************/
class GCTAMeanPsf : public GBase {

public:
   
    // Constructors and destructors
    GCTAMeanPsf(void);
    GCTAMeanPsf(const GCTAMeanPsf& cube);
    GCTAMeanPsf(const GObservations& obs,
                const std::string&   wcs,
                const std::string&   coords,
                const double&        x,
                const double&        y,
                const double&        dx,
                const double&        dy,
                const int&           nx,
                const int&           ny,
                const GEbounds&      ebounds,
                const double&        dmax,
                const int&           ndbins);
    virtual ~GCTAMeanPsf(void);

    // Operators
    GCTAMeanPsf& operator=(const GCTAMeanPsf& cube);

    // Methods
    void              clear(void);
    GCTAMeanPsf*      clone(void) const;
    void              set(const GCTAObservation& obs);
    void              fill(const GObservations& obs);
    const GSkymap&    map(void) const;
    const GEbounds&   ebounds(void) const;
    const GNodeArray& deltas(void) const;
    void              write(GFits& file) const;
    void              load(const std::string& filename);
    void              save(const std::string& filename, const bool& clobber) const;
    std::string       print(const GChatter& chatter = NORMAL) const;

protected:
    // Methods
    void init_members(void);
    void copy_members(const GCTAMeanPsf& cube);
    void free_members(void);
    void clear_cube(void);
    int  offset(const int& idelta, const int& iebin) const;
    
    // Data
    GSkymap    m_cube;     //!< PSF cube
    GEbounds   m_ebounds;  //!< Energy bounds for the PSF cube
    GNodeArray m_deltas;   //!< Delta bins for the PSF cube
};


/***********************************************************************//**
 * @brief Return psf cube sky map
 *
 * @return psf cube sky map.
 *
 * The GCTAMeanPsf represents the psf cube as a sky map. This methods
 * returns the sky map that is stored internally by GCTAMeanPsf as psf
 * cube.
 ***************************************************************************/
inline
const GSkymap& GCTAMeanPsf::map(void) const
{
    return (m_cube);
}

/***********************************************************************//**
 * @brief Return energy boundaries
 *
 * @return Energy boundaris
 *
 ***************************************************************************/
inline
const GEbounds& GCTAMeanPsf::ebounds(void) const
{
    return (m_ebounds);
}

/***********************************************************************//**
 * @brief Return deltas nodes
 *
 * @return deltas
 ***************************************************************************/
inline
const GNodeArray& GCTAMeanPsf::deltas(void) const
{
    return (m_deltas);
}


/***********************************************************************//**
 * @brief Return map offset
 *
 * @return Map offset.
 ***************************************************************************/
inline
int GCTAMeanPsf::offset(const int& idelta, const int& iebin) const
{
    return (idelta + iebin*m_ebounds.size());
}

#endif /* GCTAMEANPSF_HPP */
