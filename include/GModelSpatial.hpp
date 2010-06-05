/***************************************************************************
 *         GModelSpatial.hpp  -  Abstract spatial model base class         *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2009-2010 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GModelSpatial.hpp
 * @brief GModelSpatial abstract base class interface definition.
 * @author J. Knodlseder
 */

#ifndef GMODELSPATIAL_HPP
#define GMODELSPATIAL_HPP

/* __ Includes ___________________________________________________________ */
#include "GModelPar.hpp"
#include "GInstDir.hpp"
#include "GSkyDir.hpp"
#include "GEnergy.hpp"
#include "GTime.hpp"
#include "GResponse.hpp"
#include "GPointing.hpp"


/***********************************************************************//**
 * @class GModelSpatial
 *
 * @brief Abstract interface definition for the spatial model class.
 *
 * This class implements the spatial component of gamma-ray data model.
 ***************************************************************************/
class GModelSpatial {

    // Friend classes
    friend class GModel;

public:
    // Constructors and destructors
    GModelSpatial(void);
    GModelSpatial(const GModelSpatial& model);
    virtual ~GModelSpatial();
 
    // Operators
    virtual GModelSpatial& operator= (const GModelSpatial& model);

    // Virtual methods
    virtual int        npars(void) const = 0;
    virtual GModelPar* par(int index) const = 0;
    virtual double     eval(const GInstDir& obsDir, const GSkyDir& srcDir,
                            const GEnergy& srcEng, const GTime& srcTime,
                            const GResponse& rsp, const GPointing& pnt) = 0;
    virtual double     eval_gradients(const GInstDir& obsDir,
                                      const GSkyDir& srcDir,
                                      const GEnergy& srcEng,
                                      const GTime& srcTime,
                                      const GResponse& rsp,
                                      const GPointing& pnt) = 0;
  
protected:
    // Protected methods
    void                   init_members(void);
    void                   copy_members(const GModelSpatial& model);
    void                   free_members(void);
    virtual GModelSpatial* clone(void) const = 0;
};

#endif /* GMODELSPATIAL_HPP */
