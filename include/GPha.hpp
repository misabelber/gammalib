/***************************************************************************
 *                GPha.hpp - XSPEC Pulse Height Analyzer class             *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2013-2018 by Juergen Knoedlseder                         *
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
 * @file GPha.hpp
 * @brief XSPEC Pulse Height Analyzer class definition
 * @author Juergen Knoedlseder
 */

#ifndef GPHA_HPP
#define GPHA_HPP

/* __ Includes ___________________________________________________________ */
#include <string>
#include "GBase.hpp"
#include "GEbounds.hpp"
#include "GFilename.hpp"
#include "GNdarray.hpp"

/* __ Forward declarations _______________________________________________ */
class GEnergy;
class GFits;
class GFitsTable;

/* __ Constants __________________________________________________________ */
namespace gammalib {
    const std::string extname_pha = "SPECTRUM";
}


/***********************************************************************//**
 * @class GPha
 *
 * @brief Pulse Height Analyzer class
 *
 * This class implements a Pulse Height Analyzer (PHA) spectrum that is used
 * as data container for an XSPEC analysis. A PHA spectrum is a vector that
 * provides the number of measured counts as function of the channel number.
 *
 * As an extension to the PHA format, the GPha class may also store the
 * energy boundaries for all PHA channels. If defined, the energy boundaries
 * will be written as a EBOUNDS extension to the same file where the PHA
 * spectrum resides. Upon loading, GPha will also load the energy boundaries
 * from an EBOUNDS extension if they are present.
 ***************************************************************************/
class GPha : public GBase {

    // Operator friends
    friend GPha operator+(const GPha& a,       const GPha& b);
    friend GPha operator-(const GPha& a,       const GPha& b);
    friend GPha operator*(const GPha& pha,     const double& scale);
    friend GPha operator*(const double& scale, const GPha& pha);
    friend GPha operator/(const GPha& pha,     const double& scale);

public:
    // Constructors and destructors
    GPha(void);
    explicit GPha(const GFilename& filename);
    explicit GPha(const GEbounds& ebds);
    explicit GPha(const int& bins);
    GPha(const GPha& pha);
    virtual ~GPha(void);

    // Operators
    GPha&         operator=(const GPha& pha);
    GPha&         operator+=(const GPha& pha);
    GPha&         operator-=(const GPha& pha);
    GPha&         operator*=(const double& scale);
    GPha&         operator/=(const double& scale);
    double&       operator[](const int& index);
    const double& operator[](const int& index) const;

    // Methods
    void             clear(void);
    GPha*            clone(void) const;
    std::string      classname(void) const;
    int              size(void) const;
    double&          at(const int& index);
    const double&    at(const int& index) const;
    const GEbounds&  ebounds(void) const;
    double           counts(void) const;
    GNdarray         counts_spectrum(void) const;
    void             areascal(const int& index, const double& areascal);
    const double&    areascal(const int& index) const;
    void             backscal(const int& index, const double& backscal);
    const double&    backscal(const int& index) const;
    GNdarray         backscal_spectrum(void) const;
    const double&    underflow(void) const;
    const double&    overflow(void) const;
    const double&    outflow(void) const;
    void             exposure(const double& exposure);
    const double&    exposure(void) const;
    void             fill(const GEnergy& energy, const double& value = 1.0);
    void             load(const GFilename& filename);
    void             save(const GFilename& filename,
                          const bool&      clobber = false) const;
    void             read(const GFitsTable& table);
    void             write(GFits& fits) const;
    const GFilename& filename(void) const;
    std::string      print(const GChatter& chatter = NORMAL) const;

protected:
    // Protected methods
    void init_members(void);
    void copy_members(const GPha& pha);
    void free_members(void);
    void alloc(const int& size);

    // Protected members
    mutable GFilename   m_filename;  //!< Filename of origin
    std::vector<double> m_counts;    //!< Counts data
    std::vector<double> m_areascal;  //!< Area scaling
    std::vector<double> m_backscal;  //!< Background scaling
    double              m_underflow; //!< Number of underflowing events
    double              m_overflow;  //!< Number of overflowing events
    double              m_outflow;   //!< Number of outflowing events
    double              m_exposure;  //!< Deadtime corrected exposure time (sec)
    GEbounds            m_ebounds;   //!< Energy boundaries
};


/***********************************************************************//**
 * @brief Return class name
 *
 * @return String containing the class name ("GPha").
 ***************************************************************************/
inline
std::string GPha::classname(void) const
{
    return ("GPha");
}


/***********************************************************************//**
 * @brief Return content of spectral bin
 *
 * @param[in] index Bin index [0,...,size()-1].
 *
 * Returns reference to content of spectral bin with specified @p index.
 ***************************************************************************/
inline
double& GPha::operator[](const int& index)
{
    return (m_counts[index]);
}


/***********************************************************************//**
 * @brief Return content of spectral bin (const version)
 *
 * @param[in] index Bin index [0,...,size()-1].
 *
 * Returns reference to content of spectral bin with specified @p index.
 ***************************************************************************/
inline
const double& GPha::operator[](const int& index) const
{
    return (m_counts[index]);
}


/***********************************************************************//**
 * @brief Return number of bins in spectrum
 *
 * @return Number of bins in spectrum.
 *
 * Returns the number of bins in the spectrum.
 ***************************************************************************/
inline
int GPha::size(void) const
{
    return (int)m_counts.size();
}


/***********************************************************************//**
 * @brief Return energy boundaries
 *
 * @return Energy boundaries for all spectral bins.
 *
 * Returns the energy boundaries for all spectral bins.
 ***************************************************************************/
inline
const GEbounds& GPha::ebounds(void) const
{
    return (m_ebounds);
}


/***********************************************************************//**
 * @brief Return number of underflow counts
 *
 * @return Number of counts with energies lower than the first energy bin.
 *
 * Returns the number of counts with energies lower than the first energy
 * bin.
 ***************************************************************************/
inline
const double& GPha::underflow(void) const
{
    return (m_underflow);
}


/***********************************************************************//**
 * @brief Return number of overflow counts
 *
 * @return Number of counts with energies larger than the last energy bin.
 *
 * Returns the number of counts with energies larger than the last energy
 * bin.
 ***************************************************************************/
inline
const double& GPha::overflow(void) const
{
    return (m_overflow);
}


/***********************************************************************//**
 * @brief Return number of outflow counts
 *
 * @return Number of counts with energies between energy bins.
 *
 * Returns the number of counts with energies between energy bins.
 ***************************************************************************/
inline
const double& GPha::outflow(void) const
{
    return (m_outflow);
}


/***********************************************************************//**
 * @brief Return exposure time
 *
 * @return Exposure time (seconds).
 *
 * Returns the exposure time in seconds.
 ***************************************************************************/
inline
const double& GPha::exposure(void) const
{
    return (m_exposure);
}


/***********************************************************************//**
 * @brief Set exposure time
 *
 * @param[in] exposure Exposure time (seconds).
 *
 * Set the exposure time in seconds.
 ***************************************************************************/
inline
void GPha::exposure(const double& exposure)
{
    m_exposure = exposure;
    return;
}


/***********************************************************************//**
 * @brief Return file name
 *
 * @return File name from which the PHA information has been read or into
 *         which PHA information has been saved.
 *
 * Returns the file name from which the PHA information has been read or into
 * which PHA information has been saved. The returned string will be empty if
 * no load() or save() method has been called before.
 ***************************************************************************/
inline
const GFilename& GPha::filename(void) const
{
    return (m_filename);
}


/***********************************************************************//**
 * @brief Spectrum addition operator friend
 *
 * @param[in] a First Pulse Height Analyzer spectrum.
 * @param[in] b Second Pulse Height Analyzer spectrum.
 * @return Sum of Pulse Height Analyzer spectra.
 ***************************************************************************/
inline
GPha operator+(const GPha& a, const GPha& b)
{
    GPha result = a;
    result     += b;
    return result;
}


/***********************************************************************//**
 * @brief Spectrum subtraction operator friend
 *
 * @param[in] a First Pulse Height Analyzer spectrum.
 * @param[in] b Second Pulse Height Analyzer spectrum.
 * @return Difference of Pulse Height Analyzer spectra.
 ***************************************************************************/
inline
GPha operator-(const GPha& a, const GPha& b)
{
    GPha result = a;
    result     -= b;
    return result;
}


/***********************************************************************//**
 * @brief Spectrum scaling operator friend
 *
 * @param[in] pha Pulse Height Analyzer spectrum.
 * @param[in] scale Scale factor.
 * @return Scaled Pulse Height Analyzer spectrum.
 ***************************************************************************/
inline
GPha operator*(const GPha& pha, const double& scale)
{
    GPha result = pha;
    result     *= scale;
    return result;
}


/***********************************************************************//**
 * @brief Spectrum scaling operator friend
 *
 * @param[in] scale Scale factor.
 * @param[in] pha Pulse Height Analyzer spectrum.
 * @return Scaled Pulse Height Analyzer spectrum.
 ***************************************************************************/
inline
GPha operator*(const double& scale, const GPha& pha)
{
    GPha result = pha;
    result     *= scale;
    return result;
}


/***********************************************************************//**
 * @brief Spectrum division operator friend
 *
 * @param[in] pha Pulse Height Analyzer spectrum.
 * @param[in] scale Division factor.
 * @return Divided Pulse Height Analyzer spectrum.
 ***************************************************************************/
inline
GPha operator/(const GPha& pha, const double& scale)
{
    GPha result = pha;
    result     /= scale;
    return result;
}

#endif /* GPHA_HPP */
