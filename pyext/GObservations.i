/***************************************************************************
 *              GObservations.i - Observations container class             *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2008-2017 by Juergen Knoedlseder                         *
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
 * @file GObservations.i
 * @brief Observations container class interface definition
 * @author Juergen Knoedlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GObservations.hpp"
%}


/***********************************************************************//**
 * @class GObservations
 *
 * @brief Observation container class
 ***************************************************************************/
class GObservations : public GContainer {
public:
    // Constructors and destructors
    GObservations(void);
    GObservations(const GObservations& obs);
    explicit GObservations(const GFilename& filename);
    virtual ~GObservations(void);

    // Methods
    void           clear(void);
    GObservations* clone(void) const;
    std::string    classname(void) const;
    int            size(void) const;
    bool           is_empty(void) const;
    GObservation*  set(const int& index, const GObservation& obs);
    GObservation*  append(const GObservation& obs);
    GObservation*  insert(const int& index, const GObservation& obs);
    void           remove(const int& index);
    void           reserve(const int& num);
    void           extend(const GObservations& obs);
    bool           contains(const std::string& instrument,
                            const std::string& id) const;
    void           load(const GFilename& filename);
    void           save(const GFilename& filename) const;
    void           read(const GXml& xml);
    void           write(GXml& xml) const;
    void           models(const GModels& models);
    void           models(const GFilename& filename);
    const GModels& models(void);
    void           optimize(GOptimizer& opt);
    void           errors(GOptimizer& opt);
    void           errors_hessian(void);
    void           eval(void);
    double         logL(void) const;
    int            nobserved(void) const;
    double         npred(void) const;

    // Optimizer function access method
    const GObservations::likelihood& function(void) const;
};
    
// Likelihood function
class likelihood : public GOptimizerFunction {
public:
    // Constructors and destructors
    likelihood(void);
    likelihood(GObservations* obs);
    likelihood(const likelihood& fct);
    ~likelihood(void);

    // Implemented pure virtual base class methods
    virtual void           eval(const GOptimizerPars& pars);
    virtual double         value(void) const;
    virtual GVector*       gradient(void);
    virtual GMatrixSparse* curvature(void);

    // Other methods
    void          set(GObservations* obs);
    double        npred(void) const;
    GMatrixSparse hessian(const GOptimizerPars& pars);
    GMatrixSparse covariance(void) const;
    void          save(const GFilename& filename) const;
};
%nestedworkaround GObservations::likelihood;
%{
// SWIG thinks that likelihood is a global class, so we need to trick the C++
// compiler into understanding this so called global type.
typedef GObservations::likelihood likelihood;
%}


/***********************************************************************//**
 * @brief GObservations class extension
 ***************************************************************************/
%extend GObservations {
    GObservation* __getitem__(const int& index) {
        // Counting from start, e.g. [2]
        if (index >= 0 && index < self->size()) {
            return (*self)[index];
        }
        // Counting from end, e.g. [-1]
        else if (index < 0 && self->size()+index >= 0) {
            return (*self)[self->size()+index];
        }
        else {
            throw GException::out_of_range("__getitem__(int)", index, self->size());
        }
    }
    GObservations* __getitem__(PyObject *param) {
        if (PySlice_Check(param)) {
            Py_ssize_t start = 0;
            Py_ssize_t stop  = 0;
            Py_ssize_t step  = 0;
            Py_ssize_t len   = self->size();
            if (PythonSlice_GetIndices(param, len, &start, &stop, &step) == 0) {
                GObservations* obs = new GObservations;
                obs->models(self->models()); // Copy over model container
                if (step > 0) {
                    for (int i = (int)start; i < (int)stop; i += (int)step) {
                        obs->append(*(*self)[i]);
                    }
                }
                else {
                    for (int i = (int)start; i > (int)stop; i += (int)step) {
                        obs->append(*(*self)[i]);
                    }
                }
                return obs;
            }
            else {
                throw GException::invalid_argument("__getitem__(PyObject)",
                                                   "Invalid slice indices");
            }
        }
        else {
            throw GException::invalid_argument("__getitem__(PyObject)","");
        }
    }
    void __setitem__(const int& index, const GObservation& val) {
        // Counting from start, e.g. [2]
        if (index >= 0 && index < self->size()) {
            self->set(index, val);
        }
        // Counting from end, e.g. [-1]
        else if (index < 0 && self->size()+index >= 0) {
            self->set(self->size()+index, val);
        }
        else {
            throw GException::out_of_range("__setitem__(int)", index, self->size());
        }
    }
    GObservations copy() {
        return (*self);
    }
};
