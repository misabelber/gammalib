/***************************************************************************
 *                  GModels.cpp  -  Model container class                  *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2009-2011 by Jurgen Knodlseder                           *
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
 * @file GModels.cpp
 * @brief Model container class implementation
 * @author J. Knodlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "GTools.hpp"
#include "GException.hpp"
#include "GModels.hpp"
#include "GModel.hpp"
#include "GModelRegistry.hpp"
#include "GXml.hpp"
#include "GXmlElement.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_ACCESS1                                 "GModels::operator[](int&)"
#define G_ACCESS2                         "GModels::operator[](std::string&)"
#define G_READ                                         "GModels::read(GXml&)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                         Constructors/destructors                        =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 ***************************************************************************/
GModels::GModels(void) : GOptimizerPars()
{
    // Initialise members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] models Model container.
 ***************************************************************************/
GModels::GModels(const GModels& models) : GOptimizerPars(models)
{
    // Initialise members
    init_members();

    // Copy members
    copy_members(models);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Load constructor
 *
 * @param[in] filename XML filename.
 ***************************************************************************/
GModels::GModels(const std::string& filename)
{
    // Initialise members
    init_members();

    // Load XML file
    load(filename);

    // Return
    return;
}




/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GModels::~GModels(void)
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                               Operators                                 =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] models Model container.
 ***************************************************************************/
GModels& GModels::operator= (const GModels& models)
{
    // Execute only if object is not identical
    if (this != &models) {

        // Copy base class members
        this->GOptimizerPars::operator=(models);

        // Free members
        free_members();

        // Initialise members
        init_members();

        // Copy members
        copy_members(models);

    } // endif: object was not identical

    // Return
    return *this;
}


/***********************************************************************//**
 * @brief Returns reference to model
 *
 * @param[in] index Model index [0,...,size()-1].
 *
 * @exception GException::out_of_range
 *            Model index is out of range.
 ***************************************************************************/
GModel& GModels::operator[](const int& index)
{
    // Compile option: raise exception if index is out of range
    #if defined(G_RANGE_CHECK)
    if (index < 0 || index >= size())
        throw GException::out_of_range(G_ACCESS1, index, 0, size()-1);
    #endif

    // Return reference
    return *(m_models[index]);
}


/***********************************************************************//**
 * @brief Returns reference to model
 *
 * @param[in] index Model index [0,...,size()-1].
 *
 * @exception GException::out_of_range
 *            Model index is out of range.
 ***************************************************************************/
const GModel& GModels::operator[](const int& index) const
{
    // Compile option: raise exception if index is out of range
    #if defined(G_RANGE_CHECK)
    if (index < 0 || index >= size())
        throw GException::out_of_range(G_ACCESS1, index, 0, size()-1);
    #endif

    // Return reference
    return *(m_models[index]);
}


/***********************************************************************//**
 * @brief Returns reference to model
 *
 * @param[in] name Model name.
 *
 * @exception GException::model_not_found
 *            Model with specified name not found in container.
 ***************************************************************************/
GModel& GModels::operator[](const std::string& name)
{
    // Get parameter index
    int index = 0;
    for (; index < size(); ++index) {
        if (m_models[index]->name() == name)
            break;
    }

    // Throw exception if parameter name was not found
    if (index >= size())
        throw GException::model_not_found(G_ACCESS2, name);

    // Return reference
    return *(m_models[index]);
}


/***********************************************************************//**
 * @brief Returns reference to model (const version)
 *
 * @param[in] name Model name.
 *
 * @exception GException::model_not_found
 *            Model with specified name not found in container.
 ***************************************************************************/
const GModel& GModels::operator[](const std::string& name) const
{
    // Get parameter index
    int index = 0;
    for (; index < size(); ++index) {
        if (m_models[index]->name() == name)
            break;
    }

    // Throw exception if parameter name was not found
    if (index >= size())
        throw GException::model_not_found(G_ACCESS2, name);

    // Return reference
    return *(m_models[index]);
}


/*==========================================================================
 =                                                                         =
 =                            Public methods                               =
 =                                                                         =
 ==========================================================================*/

 /***********************************************************************//**
 * @brief Clear object.
 *
 * This method properly resets the object to an initial state.
 ***************************************************************************/
void GModels::clear(void)
{
    // Free class members (base and derived classes, derived class first)
    free_members();
    this->GOptimizerPars::free_members();

    // Initialise members
    this->GOptimizerPars::init_members();
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone instance
 ***************************************************************************/
GModels* GModels::clone(void) const
{
    return new GModels(*this);
}


/***********************************************************************//**
 * @brief Append model to container
 *
 * @param[in] model Model.
 *
 * This method appends one model to the container by cloning the model. The
 * model provided to the method can be deleted after calling this method.
 ***************************************************************************/
void GModels::append(const GModel& model)
{
    // Append model
    m_models.push_back(model.clone());

    // Set parameter pointers
    set_pointers();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Load models from XML file.
 *
 * @param[in] filename Name of XML file.
 ***************************************************************************/
void GModels::load(const std::string& filename)
{
    // Clear any existing models
    clear();

    // Load XML document
    GXml xml(filename);

    // Read models from XML document
    read(xml);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Save models into XML file.
 *
 * @param[in] filename Name of XML file.
 ***************************************************************************/
void GModels::save(const std::string& filename) const
{
    // Declare empty XML document
    GXml xml;

    // Write models into XML file
    write(xml);

    // Save XML document
    xml.save(filename);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Read models from XML document
 *
 * @param[in] xml XML document.
 *
 * @exception GException::model_invalid
 *            Invalid model type encountered.
 *
 * Read models from the first source library found in the XML document. It is
 * assumed that each model is composed of a spectral and a spatial model
 * (Fermi-LAT style). The decoding of the spatial and spectral XML elements
 * is done within a GModel constructor.
 *
 * @todo Sources names are not verified so far for uniqueness. This would be
 *       required to achieve an unambiguous update of parameters in an already
 *       existing XML file when using the write method. Also, no control is
 *       performed that only a single spectral and spatial component exists in
 *       each source definition.
 ***************************************************************************/
void GModels::read(const GXml& xml)
{
    // Get pointer on source library
    GXmlElement* lib = xml.element("source_library", 0);

    // Loop over all sources
    int n = lib->elements("source");
    for (int i = 0; i < n; ++i) {

        // Get pointer on source
        GXmlElement* src = (GXmlElement*)lib->element("source", i);

        // Get model type
        std::string type = src->attribute("type");

        // Get model
        GModelRegistry registry;
        GModel*        ptr = registry.alloc(type);

        // If model if valid then read model from XML file
        if (ptr != NULL)
            ptr->read(*src);

        // ... otherwise throw an exception
        else
            throw GException::model_invalid(G_READ, type);

        // Append model
        append(*ptr);

        // Free model (appending clones the model)
        delete ptr;

    } // endfor: looped over all sources

    // Return
    return;
}


/***********************************************************************//**
 * @brief Write models into XML document
 *
 * @param[in] xml XML document.
 *
 * Write models into the first source library that is found in the XML
 * document. In case that no source library exists, one is added to the
 * document.
 ***************************************************************************/
void GModels::write(GXml& xml) const
{
    // If there is no source library then append one
    if (xml.elements("source_library") == 0) {
        xml.append(new GXmlElement("source_library title=\"source library\""));
    }

    // Get pointer on source library
    GXmlElement* lib = xml.element("source_library", 0);

    // Write all sources into library
    for (int i = 0; i < size(); ++i)
        m_models[i]->write(*lib);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Evaluate function
 *
 * @param[in] event Observed event.
 * @param[in] obs Observation.
 ***************************************************************************/
double GModels::eval(const GEvent& event, const GObservation& obs) const
{
    // Initialise function value
    double value = 0.0;

    // Evaluate function for all models
    for (int i = 0; i < size(); ++i)
        value += m_models[i]->eval(event, obs);

    // Return
    return value;
}


/***********************************************************************//**
 * @brief Evaluate function and gradients
 *
 * @param[in] event Observed event.
 * @param[in] obs Observation.
 ***************************************************************************/
double GModels::eval_gradients(const GEvent& event,
                               const GObservation& obs) const
{
    // Initialise function value
    double value = 0.0;

    // Evaluate function for all models
    for (int i = 0; i < size(); ++i)
        value += m_models[i]->eval_gradients(event, obs);

    // Return
    return value;
}


/***********************************************************************//**
 * @brief Print models
 ***************************************************************************/
std::string GModels::print(void) const
{
    // Initialise result string
    std::string result;

    // Append header
    result.append("=== GModels ===");
    result.append("\n"+parformat("Number of models")+str(size()));
    result.append("\n"+parformat("Number of parameters")+str(npars()));

    // Append models
    for (int i = 0; i < size(); ++i)
        result.append("\n"+m_models[i]->print());

    // Return result
    return result;
}


/*==========================================================================
 =                                                                         =
 =                              Private methods                            =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GModels::init_members(void)
{
    // Initialise members
    m_models.clear();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] models Model container.
 *
 * This method clones all models.
 ***************************************************************************/
void GModels::copy_members(const GModels& models)
{
    // Copy models
    m_models.clear();
    for (int i = 0; i < models.m_models.size(); ++i)
        m_models.push_back((models.m_models[i]->clone()));

    // Set parameter pointers
    set_pointers();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 *
 * As container classes that hold pointers need to handle themselves the
 * proper deallocation of memory, we loop here over all pointers and make
 * sure that we deallocate all models in the container.
 ***************************************************************************/
void GModels::free_members(void)
{
    // Free models
    for (int i = 0; i < m_models.size(); ++i) {
        delete m_models[i];
        m_models[i] = NULL;
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Set parameter pointers
 *
 * Gathers all parameter pointers from the models into a linear array of
 * GModelPar pointers. This allows the optimizer to see all parameters in a
 * linear array.
 ***************************************************************************/
void GModels::set_pointers(void)
{
    // Clear parameters
    m_pars.clear();

    // Gather all pointers
    for (int i = 0; i < size(); ++i) {
        for (int k = 0; k < m_models[i]->size(); ++k)
            m_pars.push_back(&(*m_models[i])[k]);
    }

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                                  Friends                                =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Output operator
 *
 * @param[in] os Output stream.
 * @param[in] models Model container.
 ***************************************************************************/
std::ostream& operator<< (std::ostream& os, const GModels& models)
{
     // Write models in output stream
    os << models.print();

    // Return output stream
    return os;
}


/***********************************************************************//**
 * @brief Log operator
 *
 * @param[in] log Logger.
 * @param[in] models Model container.
 ***************************************************************************/
GLog& operator<< (GLog& log, const GModels& models)
{
    // Write models into logger
    log << models.print();

    // Return logger
    return log;
}
