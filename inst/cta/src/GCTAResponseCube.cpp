/***************************************************************************
 *      GCTAResponseCube.cpp - CTA cube-style response function class      *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2014 by Juergen Knoedlseder                              *
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
 * @file GCTAResponseCube.cpp
 * @brief CTA cube-style response function class implementation
 * @author Juergen Knoedlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <cmath>
#include <string>
#include "GTools.hpp"
#include "GCTAResponseCube.hpp"
#include "GCTAResponse_helpers.hpp"
#include "GCTASourceCubePointSource.hpp"
#include "GCTASourceCubeDiffuse.hpp"
#include "GModelSpatialPointSource.hpp"
#include "GModelSpatialRadial.hpp"
#include "GModelSpatialElliptical.hpp"
#include "GModelSpatialDiffuse.hpp"
#include "GPhoton.hpp"
#include "GEvent.hpp"
#include "GSkyDir.hpp"
#include "GEnergy.hpp"
#include "GTime.hpp"
#include "GIntegral.hpp"
#include "GObservation.hpp"
#include "GCTAInstDir.hpp"
#include "GCTAEventBin.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_IRF        "GCTAResponseCube::irf(GEvent&, GPhoton& GObservation&)"
#define G_IRF_PTSRC          "GCTAResponseCube::irf_ptsrc(GEvent&, GSource&,"\
                                                            " GObservation&)"
#define G_IRF_RADIAL        "GCTAResponseCube::irf_radial(GEvent&, GSource&,"\
                                                            " GObservation&)"
#define G_IRF_DIFFUSE      "GCTAResponseCube::irf_diffuse(GEvent&, GSource&,"\
                                                            " GObservation&)"
#define G_NPRED            "GCTAResponseCube::npred(GPhoton&, GObservation&)"
#define G_READ                         "GCTAResponseCube::read(GXmlElement&)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */

/* __ Constants __________________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                       Constructors/destructors                          =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 *
 * Constructs void CTA response.
 ***************************************************************************/
GCTAResponseCube::GCTAResponseCube(void) : GCTAResponse()
{
    // Initialise members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] rsp CTA response.
 *
 * Constructs CTA cube-style response by making a deep copy of an existing
 * object.
 **************************************************************************/
GCTAResponseCube::GCTAResponseCube(const GCTAResponseCube& rsp) :
                  GCTAResponse(rsp)
{
    // Initialise members
    init_members();

    // Copy members
    copy_members(rsp);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Response constructor
 *
 * @param[in] exposure CTA exposure.
 * @param[in] psf CTA mean point spread function.
 *
 * Constructs CTA cube-style response from a CTA exposure and a mean point
 * spread function.
 **************************************************************************/
GCTAResponseCube::GCTAResponseCube(const GCTAExposure& exposure,
                                   const GCTAMeanPsf&  psf) :
                  GCTAResponse()
{
    // Initialise members
    init_members();

    // Set members
    m_exposure = exposure;
    m_psf      = psf;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 *
 * Destroys instance of CTA response object.
 ***************************************************************************/
GCTAResponseCube::~GCTAResponseCube(void)
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                                Operators                                =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] rsp CTA response.
 * @return CTA response.
 *
 * Assigns CTA response object to another CTA response object. The assignment
 * performs a deep copy of all information, hence the original object from
 * which the assignment has been performed can be destroyed after this
 * operation without any loss of information.
 ***************************************************************************/
GCTAResponseCube& GCTAResponseCube::operator=(const GCTAResponseCube& rsp)
{
    // Execute only if object is not identical
    if (this != &rsp) {

        // Copy base class members
        this->GCTAResponse::operator=(rsp);

        // Free members
        free_members();

        // Initialise members
        init_members();

        // Copy members
        copy_members(rsp);

    } // endif: object was not identical

    // Return this object
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                             Public methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Clear instance
 *
 * Clears CTA response object by resetting all members to an initial state.
 * Any information that was present in the object before will be lost.
 ***************************************************************************/
void GCTAResponseCube::clear(void)
{
    // Free class members (base and derived classes, derived class first)
    free_members();
    this->GCTAResponse::free_members();
    this->GResponse::free_members();

    // Initialise members
    this->GResponse::init_members();
    this->GCTAResponse::init_members();
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone instance
 *
 * @return Pointer to deep copy of CTA response.
 *
 * Creates a clone (deep copy) of a CTA response object.
 ***************************************************************************/
GCTAResponseCube* GCTAResponseCube::clone(void) const
{
    return new GCTAResponseCube(*this);
}


/***********************************************************************//**
 * @brief Return value of instrument response function
 *
 * @param[in] event Observed event.
 * @param[in] photon Incident photon.
 * @param[in] obs Observation.
 ***************************************************************************/
double GCTAResponseCube::irf(const GEvent&       event,
                             const GPhoton&      photon,
                             const GObservation& obs) const
{
    // Retrieve event instrument direction
    const GCTAInstDir& dir = retrieve_dir(G_IRF, event);

    // Get event attributes
    const GSkyDir& obsDir = dir.dir();
    const GEnergy& obsEng = event.energy();

    // Get photon attributes
    const GSkyDir& srcDir  = photon.dir();
    const GEnergy& srcEng  = photon.energy();
    const GTime&   srcTime = photon.time();

    // Determine angular separation between true and measured photon
    // direction in radians
    double delta = obsDir.dist(srcDir);

    // Get maximum angular separation for PSF (in radians)
    double delta_max = psf().delta_max();

    // Initialise IRF value
    double irf = 0.0;

    // Compute only if we're sufficiently close to PSF
    if (delta <= delta_max) {

        // Get exposure
        irf = exposure()(srcDir, srcEng);

        // Multiply-in PSF
        if (irf > 0.0) {

            // Get PSF component
            irf *= psf()(srcDir, delta, srcEng);

            // Divide by ontime as the binned likelihood function is
            // later multiplying by ontime
            irf /= obs.ontime();

            // Apply deadtime correction
            irf *= obs.deadc(srcTime);

        } // endif: Aeff was non-zero

    } // endif: we were sufficiently close to PSF

    // Compile option: Check for NaN/Inf
    #if defined(G_NAN_CHECK)
    if (gammalib::is_notanumber(irf) || gammalib::is_infinite(irf)) {
        std::cout << "*** ERROR: GCTAResponseCube::irf:";
        std::cout << " NaN/Inf encountered";
        std::cout << " irf=" << irf;
        std::cout << std::endl;
    }
    #endif

    // Return IRF value
    return irf;
}


/***********************************************************************//**
 * @brief Return value of instrument response function
 *
 * @param[in] event Event.
 * @param[in] source Source.
 * @param[in] obs Observation.
 *
 * Returns the instrument response function for a given event, source and
 * observation.
 ***************************************************************************/
double GCTAResponseCube::irf(const GEvent&       event,
                             const GSource&      source,
                             const GObservation& obs) const
{
    // Initialise IRF value
    double irf = 0.0;

    // Select IRF depending on the spatial model type
    switch (source.model()->code()) {
        case GMODEL_SPATIAL_POINT_SOURCE:
            irf = irf_ptsrc(event, source, obs);
            break;
        case GMODEL_SPATIAL_RADIAL:
            irf = irf_radial(event, source, obs);
            break;
        case GMODEL_SPATIAL_ELLIPTICAL:
            irf = irf_elliptical(event, source, obs);
            break;
        case GMODEL_SPATIAL_DIFFUSE:
            irf = irf_diffuse(event, source, obs);
            break;
        default:
            break;
    }

    // Return IRF value
    return irf;
}


/***********************************************************************//**
 * @brief Return value of point source instrument response function
 *
 * @param[in] event Observed event.
 * @param[in] source Source.
 * @param[in] obs Observation.
 * @return Value of instrument response function for a point source.
 *
 * This method returns the value of the instrument response function for a
 * point source.
 ***************************************************************************/
double GCTAResponseCube::irf_ptsrc(const GEvent&       event,
                                   const GSource&      source,
                                   const GObservation& obs) const
{
    // Initialise IRF
    double irf = 0.0;

    // Get pointer to model source model
    const GModelSpatialPointSource* ptsrc = static_cast<const GModelSpatialPointSource*>(source.model());

    // Get point source direction
    GSkyDir srcDir = ptsrc->dir();

    // Get pointer on CTA event bin
    if (!event.is_bin()) {
        std::string msg = "The current event is not a CTA event bin. "
                          "This method only works on binned CTA data. Please "
                          "make sure that a CTA observation containing binned "
                          "CTA data is provided.";
        throw GException::invalid_value(G_IRF_PTSRC, msg);
    }
    const GCTAEventBin* bin = static_cast<const GCTAEventBin*>(&event);
    
    // Determine angular separation between true and measured photon
    // direction in radians
    double delta = bin->dir().dir().dist(srcDir);

    // Get maximum angular separation for PSF (in radians)
    double delta_max = psf().delta_max();

    // Compute only if we're sufficiently close to PSF
    if (delta <= delta_max) {

        // Get exposure
        irf = exposure()(srcDir, source.energy());

        // Multiply-in PSF
        if (irf > 0.0) {

            // Revover effective area from exposure
            irf /= obs.livetime();

            // Get PSF component
            irf *= psf()(srcDir, delta, source.energy());

            // Perform deadtime correction
            irf *= obs.deadc(source.time());

        } // endif: exposure was non-zero

    } // endif: we were sufficiently close to PSF

    // Compile option: Check for NaN/Inf
    #if defined(G_NAN_CHECK)
    if (gammalib::is_notanumber(irf) || gammalib::is_infinite(irf)) {
        std::cout << "*** ERROR: GCTAResponseCube::irf_ptsrc:";
        std::cout << " NaN/Inf encountered";
        std::cout << " irf=" << irf;
        std::cout << std::endl;
    }
    #endif

    // Return IRF value
    return irf;
}


/***********************************************************************//**
 * @brief Return value of radial source instrument response function
 *
 * @param[in] event Observed event.
 * @param[in] source Source.
 * @param[in] obs Observation.
 * @return Value of instrument response function for a radial source.
 *
 * This method returns the value of the instrument response function for a
 * radial source. Response values are computed on the fly.
 ***************************************************************************/
double GCTAResponseCube::irf_radial(const GEvent&       event,
                                    const GSource&      source,
                                    const GObservation& obs) const
{
    // Initialise IRF
    double irf = 0.0;

    // Get pointer to CTA event bin
    if (!event.is_bin()) {
        std::string msg = "The current event is not a CTA event bin. "
                          "This method only works on binned CTA data. Please "
                          "make sure that a CTA observation containing binned "
                          "CTA data is provided.";
        throw GException::invalid_value(G_IRF_RADIAL, msg);
    }
    const GCTAEventBin* bin = static_cast<const GCTAEventBin*>(&event);

    // Get event attribute references
    const GSkyDir& obsDir  = bin->dir().dir();
    const GEnergy& obsEng  = bin->energy();
    const GTime&   obsTime = bin->time();

    // Get pointer to radial model
    const GModelSpatialRadial* model = static_cast<const GModelSpatialRadial*>(source.model());

    // Compute angle between model centre and measured photon direction
    // (radians)
    double zeta = model->dir().dist(obsDir);

    // Continue only if we're sufficiently close to the model centre to get a
    // non-zero response
    if (zeta <= model->theta_max()+psf().delta_max()) {

        // Get exposure
        irf = exposure()(obsDir, obsEng);

        // Continue only if exposure is positive
        if (irf > 0.0) {
            irf /= obs.livetime(); //!< Revover effective area from exposure
            irf *= psf_radial(model, zeta, obsDir, obsEng, obsTime);
            irf *= obs.deadc(obsTime);
        }
        
    } // endif: we were sufficiently close

    // Compile option: Check for NaN/Inf
    #if defined(G_NAN_CHECK)
    if (gammalib::is_notanumber(irf) || gammalib::is_infinite(irf)) {
        std::cout << "*** ERROR: GCTAResponseCube::irf_radial:";
        std::cout << " NaN/Inf encountered";
        std::cout << " irf=" << irf;
        std::cout << std::endl;
    }
    #endif

    // Return IRF value
    return irf;
}


/***********************************************************************//**
 * @brief Return value of elliptical source instrument response function
 *
 * @param[in] event Observed event.
 * @param[in] source Source.
 * @param[in] obs Observation.
 * @return Value of instrument response function for an elliptical source.
 *
 * This method returns the value of the instrument response function for an
 * elliptical source. Response values are computed on the fly.
 ***************************************************************************/
double GCTAResponseCube::irf_elliptical(const GEvent&       event,
                                        const GSource&      source,
                                        const GObservation& obs) const
{
    // Initialise IRF
    double irf = 0.0;

    // Get pointer to CTA event bin
    if (!event.is_bin()) {
        std::string msg = "The current event is not a CTA event bin. "
                          "This method only works on binned CTA data. Please "
                          "make sure that a CTA observation containing binned "
                          "CTA data is provided.";
        throw GException::invalid_value(G_IRF_RADIAL, msg);
    }
    const GCTAEventBin* bin = static_cast<const GCTAEventBin*>(&event);

    // Get event attribute references
    const GSkyDir& obsDir  = bin->dir().dir();
    const GEnergy& obsEng  = bin->energy();
    const GTime&   obsTime = bin->time();

    // Get pointer to elliptical model
    const GModelSpatialElliptical* model = static_cast<const GModelSpatialElliptical*>(source.model());

    // Compute angle between model centre and measured photon direction and
    // position angle (radians)
    double zeta  = model->dir().dist(obsDir);
    double omega = model->dir().posang(obsDir);

    // Continue only if we're sufficiently close to the model centre to get a
    // non-zero response
    if (zeta <= model->theta_max()+psf().delta_max()) {

        // Get exposure
        irf = exposure()(obsDir, obsEng);

        // Continue only if exposure is positive
        if (irf > 0.0) {
            irf /= obs.livetime(); //!< Revover effective area from exposure
            irf *= psf_elliptical(model, zeta, omega, obsDir, obsEng, obsTime);
            irf *= obs.deadc(obsTime);
        }
        
    } // endif: we were sufficiently close

    // Compile option: Check for NaN/Inf
    #if defined(G_NAN_CHECK)
    if (gammalib::is_notanumber(irf) || gammalib::is_infinite(irf)) {
        std::cout << "*** ERROR: GCTAResponseCube::irf_elliptical:";
        std::cout << " NaN/Inf encountered";
        std::cout << " irf=" << irf;
        std::cout << std::endl;
    }
    #endif

    // Return IRF value
    return irf;
}


/***********************************************************************//**
 * @brief Return value of extended source instrument response function
 *
 * @param[in] event Observed event.
 * @param[in] source Source.
 * @param[in] obs Observation.
 * @return Value of instrument response function for an extended source.
 *
 * This method returns the value of the instrument response function for an
 * extended source. It uses a pre-computation cache to store the IRF for
 * the spatial model component.
 *
 * The pre-computation cache is initialised if no cache has yet been
 * allocated, or if at the beginning of a scan over the events, the model
 * parameters have changed. The beginning of a scan is defined by an event
 * bin index of 0.
 ***************************************************************************/
double GCTAResponseCube::irf_diffuse(const GEvent&       event,
                                     const GSource&      source,
                                     const GObservation& obs) const
{
    // Initialise IRF
    double irf = 0.0;

    // Get pointer to CTA event bin
    if (!event.is_bin()) {
        std::string msg = "The current event is not a CTA event bin. "
                          "This method only works on binned CTA data. Please "
                          "make sure that a CTA observation containing binned "
                          "CTA data is provided.";
        throw GException::invalid_value(G_IRF_DIFFUSE, msg);
    }
    const GCTAEventBin* bin = static_cast<const GCTAEventBin*>(&event);

    // Get pointer to source cache. We first search the cache for a model
    // with the source name. If no model was found we initialise a new
    // cache entry for that model. Otherwise, we simply return the actual
    // cache entry.
    GCTASourceCubeDiffuse* cache(NULL);
    int index = cache_index(source.name());
    if (index == -1) {
    
        // No cache entry was found, thus allocate and initialise a new one
        cache = new GCTASourceCubeDiffuse;
        cache->set(source.name(), *source.model(), obs);
        m_cache.push_back(cache);

    } // endif: no cache entry was found
    else {
    
        // Check that the cache entry is of the expected type
        if (m_cache[index]->code() != GCTA_SOURCE_CUBE_DIFFUSE) {
            std::string msg = "Cached model \""+source.name()+"\" is not "
                              "an extended source model. This method only "
                              "applies to extended source models.";
            throw GException::invalid_value(G_IRF_DIFFUSE, msg);
        }
        cache = static_cast<GCTASourceCubeDiffuse*>(m_cache[index]);

    } // endelse: there was a cache entry for this model

    // Determine IRF value
    irf = cache->irf(bin->ipix(), bin->ieng());

    // Compile option: Check for NaN/Inf
    #if defined(G_NAN_CHECK)
    if (gammalib::is_notanumber(irf) || gammalib::is_infinite(irf)) {
        std::cout << "*** ERROR: GCTAResponseCube::irf_diffuse:";
        std::cout << " NaN/Inf encountered";
        std::cout << " irf=" << irf;
        std::cout << std::endl;
    }
    #endif

    // Return IRF value
    return irf;
}


/***********************************************************************//**
 * @brief Return spatial integral of point spread function
 *
 * @param[in] photon Incident photon.
 * @param[in] obs Observation.
 *
 * @exception GException::feature_not_implemented
 *            Method not implemented.
 ***************************************************************************/
double GCTAResponseCube::npred(const GPhoton&      photon,
                               const GObservation& obs) const
{
    // Feature not yet implemented
    throw GException::feature_not_implemented(G_NPRED,
          "Npred computation not implemented for cube analysis.");

    // Return Npred
    return 0.0;
}


/***********************************************************************//**
 * @brief Read response
 *
 * @param[in] xml XML element.
 *
 * @exception GException::xml_invalid_parnames
 *            Invalid parameter names found in XML element.
 *
 * Reads information for a CTA observation from an XML element. The exposure
 * and PSF cubes are specified using
 *
 *     <observation name="..." id="..." instrument="...">
 *       ...
 *       <parameter name="ExposureCube" file="..."/>
 *       <parameter name="PsfCube"      file="..."/>
 *     </observation>
 *
 ***************************************************************************/
void GCTAResponseCube::read(const GXmlElement& xml)
{
    // Determine number of parameter nodes in XML element
    int npars = xml.elements("parameter");

    // Extract parameters
    int npar[] = {0,0};
    for (int i = 0; i < npars; ++i) {

        // Get parameter element
        const GXmlElement* par = xml.element("parameter", i);

        // Handle ExposureCube
        if (par->attribute("name") == "ExposureCube") {

            // Get filename
            std::string filename = gammalib::strip_whitespace(par->attribute("file"));

            // Load exposure cube
            m_exposure.load(filename);

            // Increment parameter counter
            npar[0]++;
        }

        // Handle PsfCube
        else if (par->attribute("name") == "PsfCube") {

            // Get filename
            std::string filename = gammalib::strip_whitespace(par->attribute("file"));

            // Load PSF cube
            m_psf.load(filename);

            // Increment parameter counter
            npar[1]++;
        }

    } // endfor: looped over observation parameters

    // Verify that all required parameters were found
    if (npar[0] != 1 || npar[1] != 1) {
        throw GException::xml_invalid_parnames(G_READ, xml,
              "Require \"ExposureCube\" and \"PsfCube\" parameters.");
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Write response
 *
 * @param[in] xml XML element.
 *
 * Writes information for a CTA observation into an XML element. The exposure
 * and PSF cubes are specified using
 *
 *     <observation name="..." id="..." instrument="...">
 *       ...
 *       <parameter name="ExposureCube" file="..."/>
 *       <parameter name="PsfCube"      file="..."/>
 *     </observation>
 *
 ***************************************************************************/
void GCTAResponseCube::write(GXmlElement& xml) const
{
    // Add exposure cube filename
    std::string filename = gammalib::strip_whitespace(m_exposure.filename());
    if (!(filename.empty())) {
        GXmlElement* par = gammalib::parameter(xml, "ExposureCube");
        par->attribute("file", filename);
    }

    // Add PSF cube filename
    filename = gammalib::strip_whitespace(m_psf.filename());
    if (!(filename.empty())) {
        GXmlElement* par = gammalib::parameter(xml, "PsfCube");
        par->attribute("file", filename);
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Print CTA response information
 *
 * @param[in] chatter Chattiness (defaults to NORMAL).
 * @return String containing CTA response information.
 ***************************************************************************/
std::string GCTAResponseCube::print(const GChatter& chatter) const
{
    // Initialise result string
    std::string result;

    // Continue only if chatter is not silent
    if (chatter != SILENT) {

        // Append header
        result.append("=== GCTAResponseCube ===");

        // Append response information
        result.append("\n"+gammalib::parformat("Energy dispersion"));
        if (use_edisp()) {
            result.append("Used");
        }
        else {
            if (apply_edisp()) {
                result.append("Not available");
            }
            else {
                result.append("Not used");
            }
        }

        // Append exposure cube information
        result.append("\n"+m_exposure.print(chatter));

        // Append point spread function information
        result.append("\n"+m_psf.print(chatter));

    } // endif: chatter was not silent

    // Return result
    return result;
}


/*==========================================================================
 =                                                                         =
 =              Model type dependent CTA response methods                  =
 =                                                                         =
 ==========================================================================*/


/*==========================================================================
 =                                                                         =
 =                    Low-level CTA response methods                       =
 =                                                                         =
 ==========================================================================*/


/*==========================================================================
 =                                                                         =
 =                             Private methods                             =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GCTAResponseCube::init_members(void)
{
    // Initialise members
    m_exposure.clear();
    m_psf.clear();
    m_apply_edisp = false;

    // Initialise cache
    m_cache.clear();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] rsp Response to be copied
 ***************************************************************************/
void GCTAResponseCube::copy_members(const GCTAResponseCube& rsp)
{
    // Copy members
    m_exposure    = rsp.m_exposure;
    m_psf         = rsp.m_psf;
    m_apply_edisp = rsp.m_apply_edisp;

    // Copy cache
    for (int i = 0; i < rsp.m_cache.size(); ++i) {
        m_cache.push_back((rsp.m_cache[i]->clone()));
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GCTAResponseCube::free_members(void)
{
    // Free cache
    for (int i = 0; i < m_cache.size(); ++i) {
        if (m_cache[i] != NULL) delete m_cache[i];
        m_cache[i] = NULL;
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Determines the cache index for a given model name
 *
 * @param[in] name Model name.
 * @return Cache index (-1 if model has not been found).
 ***************************************************************************/
int GCTAResponseCube::cache_index(const std::string& name) const
{
    // Initialise index
    int index = -1;

    // Continue only if there are models in cache
    if (!m_cache.empty()) {

         // Search for model name
         for (int i = 0; i < m_cache.size(); ++i) {
             if (m_cache[i]->name() == name) {
                 index = i;
                 break;
             }
         }

    } // endif: there were models in cache

    // Return index
    return index;
}


/***********************************************************************//**
 * @brief Integrate PSF over radial model
 *
 * @param[in] model Radial model.
 * @param[in] zeta Angle between model centre and measured photon direction
 *                 (radians).
 * @param[in] obsDir Observed event direction.
 * @param[in] srcEng True photon energy.
 * @param[in] srcTime True photon arrival time.
 *
 * Computes the integral
 * 
 * \f[
 *    \int_0^{\delta_{\rm max}}
 *    {\rm PSF}(\delta) \times
 *    \int_0^{2\pi} {\rm M}(\delta, \phi) \sin \delta
 *    {\rm d}\phi {\rm d}\delta
 * \f]
 *
 * where \f${\rm M}(\delta, \phi)\f$ is a radial model in the coordinate
 * system of the point spread function, defined by the angle \f$\delta\f$
 * between the true and the measured photon direction and the azimuth angle
 * \f$\phi\f$ around the measured photon direction.
 * \f${\rm PSF}(\delta)\f$ is the azimuthally symmetric point spread
 * function.
 ***************************************************************************/
double GCTAResponseCube::psf_radial(const GModelSpatialRadial* model,
                                    const double&              zeta,
                                    const GSkyDir&             obsDir,
                                    const GEnergy&             srcEng,
                                    const GTime&               srcTime) const
{
    // Set integration precision and Romberg order. These values have been
    // determined after careful testing, see
    // https://cta-redmine.irap.omp.eu/issues/1291
    const double eps_delta   = 1.0e-2;
    const int    order_delta =      5;
    const double eps_phi     = 1.0e-2;
    const int    order_phi   =      5;

    // Initialise value
    double value = 0.0;

    // Compute sin and cosine of angle between model centre and measured
    // photon direction
    double sin_zeta = std::sin(zeta);
    double cos_zeta = std::cos(zeta);

    // Get offset angle integration interval in radians
    double delta_min = 0.0;
    double delta_max = psf().delta_max();

    // Setup integration kernel. We take here the observed photon arrival
    // direction as the true photon arrival direction because the PSF does
    // not vary significantly over a small region.
    cta_psf_radial_kern_delta integrand(this, model, obsDir, srcEng, srcTime,
                                        sin_zeta, cos_zeta,
                                        eps_phi, order_phi);

    // Integrate over PSF delta angle
    GIntegral integral(&integrand);
    integral.eps(eps_delta);
    value = integral.romberg(delta_min, delta_max, order_delta);

    // Return PSF
    return value;
}


/***********************************************************************//**
 * @brief Integrate PSF over elliptical model
 *
 * @param[in] model Elliptical model.
 * @param[in] zeta Angle between model centre and measured photon direction
 *                 (radians).
 * @param[in] omega Position angle of measured photon direction with
 *                  respect to model centre (radians)
 * @param[in] obsDir Observed event direction.
 * @param[in] srcEng True photon energy.
 * @param[in] srcTime True photon arrival time.
 *
 * Computes the integral
 * 
 * \f[
 *    \int_0^{\delta_{\rm max}}
 *    {\rm PSF}(\delta) \times
 *    \int_0^{2\pi} {\rm M}(\delta, \phi) \sin \delta
 *    {\rm d}\phi {\rm d}\delta
 * \f]
 *
 * where \f${\rm M}(\delta, \phi)\f$ is an elliptical model in the coordinate
 * system of the point spread function, defined by the angle \f$\delta\f$
 * between the true and the measured photon direction and the azimuth angle
 * \f$\phi\f$ around the measured photon direction.
 * \f${\rm PSF}(\delta)\f$ is the azimuthally symmetric point spread
 * function.
 ***************************************************************************/
double GCTAResponseCube::psf_elliptical(const GModelSpatialElliptical* model,
                                        const double&                  zeta,
                                        const double&                  omega,
                                        const GSkyDir&                 obsDir,
                                        const GEnergy&                 srcEng,
                                        const GTime&                   srcTime) const
{
    // Set integration precision and Romberg order. These values have been
    // determined after careful testing, see
    // https://cta-redmine.irap.omp.eu/issues/1291
    const double eps_delta   = 1.0e-2;
    const int    order_delta =      5;
    const double eps_phi     = 1.0e-2;
    const int    order_phi   =      5;

    // Initialise value
    double value = 0.0;

    // Compute sin and cosine of angle between model centre and measured
    // photon direction
    double sin_zeta = std::sin(zeta);
    double cos_zeta = std::cos(zeta);

    // Get offset angle integration interval in radians
    double delta_min = 0.0;
    double delta_max = psf().delta_max();

    // Setup integration kernel. We take here the observed photon arrival
    // direction as the true photon arrival direction because the PSF does
    // not vary significantly over a small region.
    cta_psf_elliptical_kern_delta integrand(this, model, obsDir, srcEng, srcTime,
                                            sin_zeta, cos_zeta, omega,
                                            eps_phi, order_phi);

    // Integrate over PSF delta angle
    GIntegral integral(&integrand);
    integral.eps(eps_delta);
    value = integral.romberg(delta_min, delta_max, order_delta);

    // Return PSF
    return value;
}
