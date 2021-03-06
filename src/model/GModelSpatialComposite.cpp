/***************************************************************************
 *       GModelSpatialComposite.cpp - Spatial composite model class        *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2016-2017 by Domenico Tiziani                            *
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
 * @file GModelSpatialComposite.cpp
 * @brief Spatial composite model class implementation
 * @author Domenico Tiziani
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "GException.hpp"
#include "GTools.hpp"
#include "GMath.hpp"
#include "GModelSpatialComposite.hpp"
#include "GModelSpatialRegistry.hpp"

/* __ Constants __________________________________________________________ */


/* __ Globals ____________________________________________________________ */
const GModelSpatialComposite g_spatial_comp_seed;
const GModelSpatialRegistry  g_spatial_comp_registry(&g_spatial_comp_seed);

/* __ Method name definitions ____________________________________________ */
#define G_READ                   "GModelSpatialComposite::read(GXmlElement&)"
#define G_WRITE                 "GModelSpatialComposite::write(GXmlElement&)"
#define G_COMPONENT_INDEX           "GModelSpatialComposite::component(int&)"
#define G_COMPONENT_NAME    "GModelSpatialComposite::component(std::string&)"
#define G_APPEND            "GModelSpatialComposite::append(GModelSpatial&, "\
                                                  "std::string&, GModelPar&)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                        Constructors/destructors                         =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 *
 * Constructs empty spatial composite model.
 ***************************************************************************/
GModelSpatialComposite::GModelSpatialComposite(void) : GModelSpatial()
{
    // Initialise members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Model type constructor
 *
 * @param[in] dummy Dummy flag.
 * @param[in] type Model type.
 *
 * Constructs empty spatial composite model by specifying a model @p type.
 ***************************************************************************/
GModelSpatialComposite::GModelSpatialComposite(const bool&        dummy,
                                               const std::string& type) :
                        GModelSpatial()
{
    // Initialise members
    init_members();

    // Set model type
    m_type = type;

    // Return
    return;
}


/***********************************************************************//**
 * @brief XML constructor
 *
 * @param[in] xml XML element.
 *
 * Construct a spatial composite model by extracting information from an
 * XML element. See the read() method for more information about the expected
 * structure of the XML element.
 ***************************************************************************/
GModelSpatialComposite::GModelSpatialComposite(const GXmlElement& xml) :
                        GModelSpatial()
{
    // Initialise members
    init_members();

    // Read information from XML element
    read(xml);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] model Spatial composite model.
 ***************************************************************************/
GModelSpatialComposite::GModelSpatialComposite(const GModelSpatialComposite& model) :
                        GModelSpatial(model)
{
    // Initialise members
    init_members();

    // Copy members
    copy_members(model);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GModelSpatialComposite::~GModelSpatialComposite(void)
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
 * @param[in] model Spatial composite model.
 * @return Spatial composite model.
 ***************************************************************************/
GModelSpatialComposite& GModelSpatialComposite::operator=(const GModelSpatialComposite& model)
{
    // Execute only if object is not identical
    if (this != &model) {

        // Copy base class members
        this->GModelSpatial::operator=(model);

        // Free members
        free_members();

        // Initialise members
        init_members();

        // Copy members
        copy_members(model);

    } // endif: object was not identical

    // Return
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                            Public methods                               =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Clear spatial composite model
 ***************************************************************************/
void GModelSpatialComposite::clear(void)
{
    // Free class members (base and derived classes, derived class first)
    free_members();
    this->GModelSpatial::free_members();

    // Initialise members
    this->GModelSpatial::init_members();
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone spatial composite model
 *
 * @return Pointer to deep copy of spatial composite model.
 ***************************************************************************/
GModelSpatialComposite* GModelSpatialComposite::clone(void) const
{
    // Clone point source model
    return new GModelSpatialComposite(*this);
}


/***********************************************************************//**
 * @brief Evaluate function
 *
 * @param[in] photon Incident photon.
 * @param[in] gradients Compute gradients?
 * @return Model value.
 *
 * Evaluates the spatial composite model by summing all model components,
 * weighting by their scale and dividing the sum of all scales.
 ***************************************************************************/
double GModelSpatialComposite::eval(const GPhoton& photon,
                                    const bool&    gradients) const
{
    // Initialise value
    double value = 0.0;

    // Sum over all components
    for (int i = 0; i < m_components.size(); ++i) {
        value += m_components[i]->eval(photon, gradients) *
                 m_scales[i]->value();
    }

    // Normalise sum by sum of scales
    if (sum_of_scales() > 0) {
        value /= sum_of_scales();
    }

    // Return value
    return value;
}


/***********************************************************************//**
 * @brief Returns MC sky direction
 *
 * @param[in] energy Photon energy.
 * @param[in] time Photon arrival time.
 * @param[in,out] ran Random number generator.
 * @return Sky direction.
 *
 * Draws an arbitrary model component and an arbitrary direction from that 
 * component.
 ***************************************************************************/
GSkyDir GModelSpatialComposite::mc(const GEnergy& energy,
                                   const GTime&   time,
                                   GRan&          ran) const
{
    // Randomly choose one model component weighted by scale

    // Calculate random number
    double random = ran.uniform() * sum_of_scales();

    // Find index of chosen model component
    double sum   = 0.0;
    int    index = 0;
    for (int i = 0; i < m_scales.size(); ++i) {
        sum += m_scales[i]->value();
        if (random <= sum) {
            index = i;
            break;
        }
    }

    if (index >= m_components.size()) {
        index = m_components.size();
    }

    // Draw random sky direction from the selected component
    GSkyDir sky_dir = m_components[index]->mc(energy, time, ran);

    // Return sky direction
    return (sky_dir);
}


/***********************************************************************//**
 * @brief Checks where model contains specified sky direction
 *
 * @param[in] dir Sky direction.
 * @param[in] margin Margin to be added to sky direction (degrees)
 * @return True if the model contains the sky direction.
 *
 * Signals whether a sky direction is contained in one of the 
 * model components.
 ***************************************************************************/
bool GModelSpatialComposite::contains(const GSkyDir& dir,
                                      const double&  margin) const
{
    // Initialise containment flag
    bool containment = false;

    // Loop over all components
    for (int i = 0; i < m_components.size(); ++i) {
        if (m_components[i]->contains(dir, margin) &&
            m_scales[i]->value() != 0.0) {
            containment = true;
            break;
        }
    }

    // Return containment
    return containment;
}


/***********************************************************************//**
 * @brief Read model from XML element
 *
 * @param[in] xml XML element.
 *
 * Reads the spatial information from an XML element.
 ***************************************************************************/
void GModelSpatialComposite::read(const GXmlElement& xml)
{
    // Get number of spatial components
    int n_comp = xml.elements("spatialModel");

    // Loop over spatial elements
    for (int i = 0; i < n_comp; ++i) {

        // Get spatial XML element
        const GXmlElement* spec = xml.element("spatialModel", i);

        // Initialise a spatial registry object
        GModelSpatialRegistry registry;

        // Read spatial model
        GModelSpatial* ptr = registry.alloc(*spec);

        // Get component attribute from XML file
        std::string name = spec->attribute("component");

        // Initialise scale parameter
        GModelPar scale("", 1.0);
        scale.range(1.0e-10, 1.0e10);
        scale.fix();

        // Get scale value
        if (spec->has_attribute("scale")) {

            // Get scale value
            scale.value(gammalib::todouble(spec->attribute("scale")));

            // If there is a "free_scale" attribute the fix or free the scaling
            // parameter accordingly
            if (spec->has_attribute("free_scale")) {
                if (gammalib::toint(spec->attribute("free_scale")) == 1) {
                    scale.free();
                }
                else {
                    scale.fix();
                }
            }

            // If there is a "scale_min" attribute set the minimum accordingly
            if (spec->has_attribute("scale_min")) {
                scale.min(gammalib::todouble(spec->attribute("scale_min")));
            }

            // If there is a "scale_max" attribute set the maximum accordingly
            if (spec->has_attribute("scale_max")) {
                scale.max(gammalib::todouble(spec->attribute("scale_max")));
            }

        } // endif: there was a scale attribute

        // Append spatial component to container
        append(*ptr, name, scale);

        // Free spatial model
        delete ptr;

    } // endfor: loop over components

    // Return
    return;
}


/***********************************************************************//**
 * @brief Write model into XML element
 *
 * @param[in] xml XML element into which model information is written.
 *
 * @exception GException::model_invalid_spatial
 *            Existing XML element is not of appropriate type
 * @exception GException::model_invalid_parnum
 *            Invalid number of model parameters found in XML element.
 * @exception GException::model_invalid_parnames
 *            Invalid model parameter names found in XML element.
 *
 * Write the spatial information into an XML element.
 ***************************************************************************/
void GModelSpatialComposite::write(GXmlElement& xml) const
{
    // Set model type
    if (xml.attribute("type") == "") {
        xml.attribute("type", type());
    }

    // Verify model type
    if (xml.attribute("type") != type()) {
        throw GException::model_invalid_spatial(G_WRITE, xml.attribute("type"),
              "Spatial model is not of type \""+type()+"\".");
    }

    // Write all model components
    for (int i = 0; i < m_components.size(); ++i) {

        // Get spatial component
        GModelSpatial* component = m_components[i];

        // Fall through if component is empty
        if (component == NULL) {
            continue;
        }

        // Create copy of the spatial model
        GModelSpatial* spatial = component->clone();

        // Loop over all parameters of component and strip prefix
        for (int k = 0; k < spatial->size(); ++k) {

            // Get model parameter
            GModelPar& par = (*spatial)[k];

            // If name contains colon then strip the prefix and the colon from
            // the name
            std::string name  = par.name();
            int         found = name.find(":");
            if (found != std::string::npos) {
                name = name.substr(found+1, std::string::npos);
                par.name(name);
            }

        } // endfor: looped over all parameters

        // Find XML element with matching name
        GXmlElement *matching_model = NULL;
        for (int k = 0; k < xml.elements("spatialModel"); ++k) {
            if (xml.element("spatialModel", k)->attribute("component") ==
                m_names[i]) {
                matching_model = xml.element("spatialModel", k);
                break;
            }
        } // endfor: loop over all XML elements

        // If no XML element with matching name was found then create a new
        // XML element and append it
        if (matching_model == NULL) {

            // Create XML element
            GXmlElement element("spatialModel");

            // Write component name
            element.attribute("component", m_names[i]);

            // Append XML element
            xml.append(element);

            // Set pointer to XML element
            matching_model = xml.element("spatialModel",
                                         xml.elements("spatialModel")-1);

        } // endif: XML element created

        // Write component into XML element
        spatial->write(*matching_model);

        // Write scale to XML element if needed
        if (m_scales[i]->value() != 1.0 ||
            m_scales[i]->is_free()      ||
            matching_model->has_attribute("scale")) {
            matching_model->attribute("scale",
                                      gammalib::str(m_scales[i]->value()));
            if (m_scales[i]->is_free()) {
                matching_model->attribute("scale_error",
            	                          gammalib::str(m_scales[i]->error()));
                matching_model->attribute("scale_min",
            	                          gammalib::str(m_scales[i]->min()));
                matching_model->attribute("scale_max",
                                          gammalib::str(m_scales[i]->max()));
            }
            matching_model->attribute("free_scale",
                                      gammalib::str(m_scales[i]->is_free()));
        }

        // Delete clone
        delete spatial;

    } // endfor: loop over all components

    // TODO: Remove unused xml elements

    // Return
    return;
}


/***********************************************************************//**
 * @brief Append spatial component
 *
 * @param[in] component Spatial model component to append.
 * @param[in] name Name of spatial model.
 * @param[in] par Model scaling parameter.
 *
 * Appends a spatial component to the composite model
 ***************************************************************************/
void GModelSpatialComposite::append(const GModelSpatial& component,
                                    const std::string&   name,
                                    const GModelPar&     par)
{
    // Append model container
    m_components.push_back(component.clone());

    // Get index of latest model
    int index = m_components.size()-1;

    // Use model index if component name is empty
    std::string component_name = !name.empty() ? name
                                               : gammalib::str(m_components.size());

    // Check if component name is unique, throw exception if not
    if (gammalib::contains(m_names, component_name)) {
    	std::string msg = "Attempt to append component \""+component_name+"\" "
                          "to composite spatial model, but a component with the "
                          "same name exists already. Each component needs a "
                          "unique name.";
        throw GException::invalid_value(G_APPEND, msg);
    }

    // Add component name
    m_names.push_back(component_name);

    // Get number of spectral parameters from model
    int npars = m_components[index]->size();

    // Loop over model parameters
    for (int ipar = 0; ipar < npars; ++ipar) {

        // Get model parameter
        GModelPar* p = &(m_components[index]->operator[](ipar));

        // Modify parameter name
        p->name(component_name+":"+p->name());

        // Append model parameter with new name to internal container
        m_pars.push_back(p);

    } // endfor: loop over model parameters

    // Set scaling parameter
    GModelPar* scale = new GModelPar(par);
    scale->name(component_name + ":scale");
    scale->scale(1.0);
    scale->gradient(0.0);
    scale->has_grad(false); // for the moment use numerical gradient

    // Push scale parameter in vector
    m_scales.push_back(scale);

    // Push scale parameter in parameter stack
    m_pars.push_back(scale);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Returns pointer to spatial component element
 *
 * @param[in] index Index of spatial component [0,...,components()-1].
 * @return Spatial model.
 *
 * @exception GException::out_of_range
 *            Index is out of range.
 *
 * Returns a spatial component to the composite model
 ***************************************************************************/
const GModelSpatial* GModelSpatialComposite::component(const int& index) const
{
    // Check if index is in validity range
    if (index < 0 || index >= m_components.size()) {
        throw GException::out_of_range(G_COMPONENT_INDEX, "Component Index",
                                       index, m_components.size());
    }

    // Return spatial component
    return m_components[index];
}


/***********************************************************************//**
 * @brief Returns pointer to specific spatial component
 *
 * @param[in] name Name of spatial component.
 * @return Spatial model.
 *
 * @exception GException::invalid_argument
 *            Spatial model not found.
 *
 * Returns a spatial component of the composite model
 ***************************************************************************/
const GModelSpatial* GModelSpatialComposite::component(const std::string& name) const
{
    // Check if model name is found
    int index = -1;
    for (int i = 0; i < m_names.size(); ++i) {
        if (m_names[i] == name) {
            index = i;
            break;
        }
    }

    // Check if component name was found
    if (index == -1) {
        std::string msg = "Model component \""+name+"\" not found in composite "
                          "spatial model.";
        throw GException::invalid_argument(G_COMPONENT_NAME, msg);
    }

    // Return spatial component
    return m_components[index];
}


/***********************************************************************//**
 * @brief Returns scale of spatial component
 *
 * @param[in] index Index of spatial component [0,...,components()-1].
 * @return Scale.
 *
 * @exception GException::out_of_range
 *            Index is out of range.
 *
 * Returns the scale of a spatial component to the composite model
 ***************************************************************************/
double GModelSpatialComposite::scale(const int& index) const
{
    // Check if index is in validity range
    if (index < 0 || index >= m_scales.size()) {
        throw GException::out_of_range(G_COMPONENT_INDEX, "Component Index",
                index, m_scales.size());
    }

    // Return spatial component scale
    return m_scales[index]->value();
}


/***********************************************************************//**
 * @brief Returns sum of all model scales
 *
 * @return Sum of all model scales
 ***************************************************************************/
double GModelSpatialComposite::sum_of_scales(void) const
{
    // Initialise sum
    double sum = 0.0;

    // Loop over all component scales
    for (int i = 0; i < m_scales.size(); ++i) {

        // Add scale to sum
        sum += m_scales[i]->value();
    }

    // Return sum
    return sum;
}


/***********************************************************************//**
 * @brief Print composite spatial model information
 *
 * @param[in] chatter Chattiness.
 * @return String containing composite spatial model information.
 ***************************************************************************/
std::string GModelSpatialComposite::print(const GChatter& chatter) const
{
    // Initialise result string
    std::string result;

    // Continue only if chatter is not silent
    if (chatter != SILENT) {

    	// Append header
        result.append("=== GModelSpatialComposite ===");

        // Append information
        result.append("\n"+gammalib::parformat("Number of components"));
        result.append(gammalib::str(components()));
        result.append("\n"+gammalib::parformat("Number of parameters"));
        result.append(gammalib::str(size()));

        // Print parameter information
        for (int i = 0; i < size(); ++i) {
            result.append("\n"+m_pars[i]->print(chatter));
        }

    } // endif: chatter was not silent

    // Return result
    return result;
}


/*==========================================================================
 =                                                                         =
 =                            Private methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GModelSpatialComposite::init_members(void)
{
    // Initialise model type
    m_type = "Composite";

    // Initialise other members
    m_components.clear();
    m_names.clear();
    m_region.clear();
    m_pars.clear();
    m_scales.clear();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] model Spatial composite model.
 ***************************************************************************/
void GModelSpatialComposite::copy_members(const GModelSpatialComposite& model)
{
    // Copy members
    m_type   = model.m_type;
    m_names  = model.m_names;
    m_region = model.m_region;

    // Initialise components and scales
    m_components.clear();
    m_scales.clear();
    m_pars.clear();

    // Copy components
    for (int i = 0; i < model.m_components.size(); ++i) {

        // Clone component
        m_components.push_back(model.m_components[i]->clone());

        // Get number of parameters to append
        int npars = m_components[i]->size();

        // Append parameter references
        for (int ipar = 0; ipar < npars; ++ipar) {
            GModelPar& par = (*m_components[i])[ipar];
            m_pars.push_back(&par);
        }

    } // endfor: looped over all components

    // Copy scales
    for (int i = 0; i < model.m_scales.size(); ++i) {

        // Clone scale
        GModelPar* scale = model.m_scales[i]->clone();

        // Push scale parameter in vector
        m_scales.push_back(scale);

        // Push scale parameter in parameter stack
        m_pars.push_back(scale);

    } // endfor: looped over scales

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GModelSpatialComposite::free_members(void)
{
    // Free model components
    for (int i = 0; i < m_components.size(); ++i) {

        // Delete component i
        if (m_components[i] != NULL) {
            delete m_components[i];
        }

        // Signal free pointer
        m_components[i] = NULL;

    }

    // Free scaling parameters
    for (int i = 0; i < m_scales.size(); ++i) {

        // Delete component i
        if (m_scales[i] != NULL) {
            delete m_scales[i];
        }

        // Signal free pointer
        m_scales[i] = NULL;

    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Set boundary sky region
 *
 * @todo Implement computation of sky boundary region
 ***************************************************************************/
void GModelSpatialComposite::set_region(void) const
{
    // Set sky region centre to (0,0)
    m_region.centre(0.0, 0.0);

    // Set sky region radius to 180 degrees (all points included)
    m_region.radius(180.0);

    // Return
    return;
}
