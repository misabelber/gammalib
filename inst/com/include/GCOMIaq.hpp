/***************************************************************************
 *         GCOMIaq.hpp - COMPTEL instrument response representation        *
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
 * @file GCOMIaq.hpp
 * @brief COMPTEL instrument response representation class interface definition
 * @author Juergen Knoedlseder
 */

#ifndef GCOMIAQ_HPP
#define GCOMIAQ_HPP

/* __ Includes ___________________________________________________________ */
#include <string>
#include "GMath.hpp"
#include "GEbounds.hpp"
#include "GFunction.hpp"
#include "GFitsImageFloat.hpp"
#include "GCOMD1Response.hpp"
#include "GCOMD2Response.hpp"
#include "GCOMInstChars.hpp"

/* __ Type definitions ___________________________________________________ */

/* __ Forward declarations _______________________________________________ */
class GFilename;
class GModelSpectral;


/***********************************************************************//**
 * @class GCOMIaq
 *
 * @brief Interface for the COMPTEL instrument response representation class
 ***************************************************************************/
class GCOMIaq : public GBase {

public:
    // Constructors and destructors
    GCOMIaq(void);
    GCOMIaq(const GCOMIaq& iaq);
    GCOMIaq(const double&   phigeo_max, const double& phigeo_bin_size,
            const double&   phibar_max, const double& phibar_bin_size);
    ~GCOMIaq(void);

    // Operators
    GCOMIaq& operator=(const GCOMIaq & iaq);

    // Methods
    void        clear(void);
    GCOMIaq*    clone(void) const;
    std::string classname(void) const;
    void        set(const GEnergy& energy, const GEbounds& ebounds);
    void        set(const GModelSpectral& spectrum, const GEbounds& ebounds);
    void        save(const GFilename& filename, const bool& clobber = false) const;
    std::string print(const GChatter& chatter = NORMAL) const;

private:
    // Private methods
    void init_members(void);
    void copy_members(const GCOMIaq& iaq);
    void free_members(void);
    void init_response(void);
    void remove_cards(void);

    // RESPSI methods
    void                compton_kinematics(const double& energy);
    double              compute_iaq_bin(const double& etrue1,
                                        const double& etrue2,
                                        const double& phibar);
    void                klein_nishina(const double& energy);
    double              klein_nishina_bin(const double& energy,
                                          const double& phigeo);
    double              klein_nishina_integral(const double& v,
                                               const double& a);
    void                weight_iaq(const double& energy);
    void                location_smearing(const double& zenith);
    std::vector<double> location_spread(const double& zenith) const;

    // Reponse integration kernel
    class response_kernel : public GFunction {
    public:
        response_kernel(const GCOMD1Response& response_d1,
                        const GCOMD2Response& response_d2,
                        const double&         etrue1,
                        const double&         etrue2,
                        const double&         phibar,
                        const double&         etmin,
                        const double&         etmax,
                        const double&         e1min,
                        const double&         e1max,
                        const double&         e2min,
                        const double&         e2max) :
                        m_response_d1(response_d1),
                        m_response_d2(response_d2),
                        m_etrue1(etrue1),
                        m_etrue2(etrue2),
                        m_cos_phibar(std::cos(phibar*gammalib::deg2rad)),
                        m_sin_phibar(std::sin(phibar*gammalib::deg2rad)),
                        m_etmin(etmin),
                        m_etmax(etmax),
                        m_e1min(e1min),
                        m_e1max(e1max),
                        m_e2min(e2min),
                        m_e2max(e2max) {}
        double eval(const double& energy1);
    protected:
        const GCOMD1Response& m_response_d1; //!< Reference to D1 module response
        const GCOMD2Response& m_response_d2; //!< Reference to D2 module response
        double                m_etrue1;      //!< True D1 energy (MeV)
        double                m_etrue2;      //!< True D2 energy (MeV)
        double                m_cos_phibar;  //!< cos(phibar)
        double                m_sin_phibar;  //!< sin(phibar)
        double                m_etmin;       //!< Minimum total energy (MeV)
        double                m_etmax;       //!< Maximum total energy (MeV)
        double                m_e1min;       //!< Minimum D1 energy (MeV)
        double                m_e1max;       //!< Maximum D1 energy (MeV)
        double                m_e2min;       //!< Minimum D2 energy (MeV)
        double                m_e2max;       //!< Maximum D2 energy (MeV)
    };

    // Smearing integration kernel
    class smearing_kernel : public GFunction {
    public:
        smearing_kernel(const double&              phigeo,
                        const double&              sigma,
                        const GNodeArray&          phigeos,
                        const std::vector<double>& values) :
                        m_phigeos(phigeos),
                        m_values(values),
                        m_phigeo(phigeo),
                        m_wgt(1.0/sigma),
                        m_norm(1.0/(sigma*gammalib::sqrt_twopi)) {}
        double eval(const double& phigeo);
    protected:
        const GNodeArray&          m_phigeos; //!< Geometrical scatter angles
        const std::vector<double>& m_values;  //!< IAQ values
        double                     m_phigeo;  //!< Current phigeo value
        double                     m_wgt;     //!< Inverse of Gaussian std (1/deg)
        double                     m_norm;    //!< Gaussian normalisation
    };

    // Private data members
    GFitsImageFloat m_iaq;               //!< Response
    GEbounds        m_ebounds;           //!< Energy boundaries
    GCOMD1Response  m_response_d1;       //!< D1 module response
    GCOMD2Response  m_response_d2;       //!< D2 module response
    GCOMInstChars   m_ict;               //!< Instrument characteristics
    double          m_phigeo_max;        //!< Maximum geometrical scatter angle (deg)
    double          m_phibar_max;        //!< Maximum Compton scatter angle (deg)
    double          m_phigeo_bin_size;   //!< Bin size in geometrical scatter angle (deg)
    double          m_phibar_bin_size;   //!< Bin size in Compton scatter angle (deg)
    double          m_phibar_resolution; //!< Bin size for oversampling (deg)
    double          m_e1min;             //!< Minimum D1 energy (MeV)
    double          m_e1max;             //!< Maximum D1 energy (MeV)
    double          m_e2min;             //!< Minimum D2 energy (MeV)
    double          m_e2max;             //!< Maximum D2 energy (MeV)
    int             m_num_energies;      //!< Number of energies for continuum IAQ
    bool            m_psd_correct;       //!< PSD correction usage flag
    double          m_zenith;            //!< Zenith angle for location smearing (deg)
};


/***********************************************************************//**
 * @brief Return class name
 *
 * @return String containing the class name ("GCOMIaq").
 ***************************************************************************/
inline
std::string GCOMIaq::classname(void) const
{
    return ("GCOMIaq");
}

#endif /* GCOMIAQ_HPP */
