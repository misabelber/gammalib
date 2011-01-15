/***************************************************************************
 *                   GVector.i - Vector class SWIG file                    *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2008-2011 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GVector.hpp"
#include "GTools.hpp"
%}


/***************************************************************************
 *                         GVector class definition                        *
 ***************************************************************************/
class GVector {
public:

    // Constructors and destructors
    explicit GVector(int num);
    GVector(const GVector& v);
    virtual ~GVector(void);

    // Vector operators
    GVector& operator+= (const GVector& v);
    GVector& operator-= (const GVector& v);
    GVector& operator+= (const double& v);
    GVector& operator-= (const double& v);
    GVector& operator*= (const double& v);
    GVector& operator/= (const double& v);
    GVector  operator- () const;

    // Vector functions
    void clear(void);
    int  size() const;
    int  non_zeros() const;
};


/***************************************************************************
 *                      GVector class SWIG extension                       *
 ***************************************************************************/
%extend GVector {
    char *__str__() {
        return tochar(self->print());
    }
    double __getitem__(int index) {
        if (index >= 0 && index < (int)self->size())
            return (*self)(index);
        else
            throw GException::out_of_range("__getitem__(int)", index, (int)self->size());
    }
    void __setitem__(int index, const double val) {
        if (index>=0 && index < (int)self->size())
            (*self)(index) = val;
        else
            throw GException::out_of_range("__setitem__(int)", index, (int)self->size());
    }
    GVector __add__(const GVector &a) {
        return (*self) + a;
    }
    GVector __add__(const double &a) {
        return (*self) + a;
    }
    GVector __sub__(const GVector &a) {
        return (*self) - a;
    }
    GVector __sub__(const double &a) {
        return (*self) - a;
    }
    double __mul__(const GVector &a) {
        return (*self) * a;
    }
    GVector __mul__(const double &a) {
        return (*self) * a;
    }
    GVector __div__(const double &a) {
        return (*self) / a;
    }
    int __is__(const GVector &a) {
            return (*self) == a;
    }
    GVector copy() {
        return (*self);
    }
    GVector cross(const GVector &a) {
        return cross(*self, a);
    }
    double norm() {
        return norm(*self);
    }
    double min() {
        return min(*self);
    }
    double max() {
        return max(*self);
    }
    double sum() {
        return sum(*self);
    }
    GVector acos() {
        return acos(*self);
    }
    GVector acosh() {
        return acosh(*self);
    }
    GVector asin() {
        return asin(*self);
    }
    GVector asinh() {
        return asinh(*self);
    }
    GVector atan() {
        return atan(*self);
    }
    GVector atanh() {
        return atanh(*self);
    }
    GVector cos() {
        return cos(*self);
    }
    GVector cosh() {
        return cosh(*self);
    }
    GVector exp() {
        return exp(*self);
    }
    GVector abs() {
        return fabs(*self);
    }
    GVector log() {
        return log(*self);
    }
    GVector log10() {
        return log10(*self);
    }
    GVector sin() {
        return sin(*self);
    }
    GVector sinh() {
        return sinh(*self);
    }
    GVector sqrt() {
        return sqrt(*self);
    }
    GVector tan() {
        return tan(*self);
    }
    GVector tanh() {
        return tanh(*self);
    }
};
