/***************************************************************************
 *         GSkyPixel.i  -  2D sky pixel index class SWIG definition        *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2011 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GSkyPixel.i
 * @brief GSkyPixel class SWIG file.
 * @author J. Knodlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GSkyPixel.hpp"
#include "GTools.hpp"
%}


/***********************************************************************//**
 * @class GSkyPixel
 *
 * @brief GSkyPixel class interface defintion
 ***************************************************************************/
class GSkyPixel {
public:
    // Constructors and destructors
    GSkyPixel(void);
    explicit GSkyPixel(double x, double y);
    GSkyPixel(const GSkyPixel& pixel);
    virtual ~GSkyPixel(void);

    // Methods
    void   x(const double& x);
    void   y(const double& y);
    double x(void) const;
    double y(void) const;
};


/***********************************************************************//**
 * @brief GSkyPixel class extension
 ***************************************************************************/
%extend GSkyPixel {
    char *__str__() {
        return tochar(self->print());
    }
};
