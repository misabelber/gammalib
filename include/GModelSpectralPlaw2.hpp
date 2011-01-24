/***************************************************************************
 *       GModelSpectralPlaw2.hpp  -  Spectral power law model class        *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2011 by Jurgen Knodlseder                                *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GModelSpectralPlaw2.hpp
 * @brief GModelSpectralPlaw2 class interface definition.
 * @author J. Knodlseder
 */

#ifndef GMODELSPECTRALPLAW2_HPP
#define GMODELSPECTRALPLAW2_HPP

/* __ Includes ___________________________________________________________ */
#include "GModelPar.hpp"
#include "GModelSpectral.hpp"
#include "GEnergy.hpp"
#include "GXmlElement.hpp"


/***********************************************************************//**
 * @class GModelSpectralPlaw2
 *
 * @brief Spectral power law interface definition.
 *
 * This class implements a power law as the spectral component of the
 * gamma-ray data model.
 * The power law function is defined as
 * \f[I(E)=integral (index+1)/(emax^{index+1}-emin^{index+1}) E^{index}\f]
 * for \f$index \ne -1\f$ and
 * \f[I(E)=integral / (\log(emax)-\log(emin)) E^{index}\f]
 * for \f$index = -1\f$, where
 * \f$integral\f$ is the integral flux between \f$emin\f$ and \f$emax\f$,
 * \f$index\f$ is the spectral index,
 * \f$emin\f$ is the lower energy limit, and
 * \f$emax\f$ is the upper energy limit.
 ***************************************************************************/
class GModelSpectralPlaw2  : public GModelSpectral {

public:
    // Constructors and destructors
    GModelSpectralPlaw2(void);
    explicit GModelSpectralPlaw2(const double& integral, const double& index);
    explicit GModelSpectralPlaw2(const GXmlElement& xml);
    GModelSpectralPlaw2(const GModelSpectralPlaw2& model);
    virtual ~GModelSpectralPlaw2(void);

    // Operators
    GModelSpectralPlaw2& operator= (const GModelSpectralPlaw2& model);

    // Implemented pure virtual methods
    void                 clear(void);
    GModelSpectralPlaw2* clone(void) const;
    int                  size(void) const { return m_npars; }
    std::string          type(void) const { return "PowerLaw2"; }
    double               eval(const GEnergy& srcEng);
    double               eval_gradients(const GEnergy& srcEng);
    double               flux(const GEnergy& emin, const GEnergy& emax) const;
    GEnergy              mc(const GEnergy& emin, const GEnergy& emax, GRan& ran) const;
    void                 read(const GXmlElement& xml);
    void                 write(GXmlElement& xml) const;
    std::string          print(void) const;

    // Other methods
    double integral(void) const { return m_integral.real_value(); }
    double index(void) const { return m_index.real_value(); }
    double emin(void) const { return m_emin.real_value(); }
    double emax(void) const { return m_emax.real_value(); }

protected:
    // Protected methods
    void init_members(void);
    void copy_members(const GModelSpectralPlaw2& model);
    void free_members(void);
    void update(const GEnergy& srcEng);

    // Implemented pure virtual methods
    GModelPar** par(void) { return m_par; }

    // Protected members
    int        m_npars;           //!< Number of parameters
    GModelPar* m_par[4];          //!< Pointers to parameters
    GModelPar  m_integral;        //!< Integral flux
    GModelPar  m_index;           //!< Spectral index
    GModelPar  m_emin;            //!< Lower energy limit (MeV)
    GModelPar  m_emax;            //!< Upper energy limit (MeV)
    double     m_log_emin;        //!< Log(emin)
    double     m_log_emax;        //!< Log(emax)
    double     m_pow_emin;        //!< emin^(index+1)
    double     m_pow_emax;        //!< emax^(index+1)
    double     m_norm;            //!< Power-law normalization (for pivot energy 1 MeV)
    double     m_g_norm;          //!< Power-law normalization gradient
    double     m_power;           //!< Power-law factor
    double     m_last_integral;   //!< Last integral flux
    double     m_last_index;      //!< Last spectral index (MeV)
    double     m_last_emin;       //!< Last lower energy limit (MeV)
    double     m_last_emax;       //!< Last upper energy limit (MeV)
    GEnergy    m_last_energy;     //!< Last source energy
    double     m_last_value;      //!< Last function value
    double     m_last_g_integral; //!< Last integral flux gradient
    double     m_last_g_index;    //!< Last spectral index gradient
};

#endif /* GMODELSPECTRALPLAW2_HPP */
