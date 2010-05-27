/***************************************************************************
 *                GLATEventCube.cpp  -  LAT event cube class               *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2009-2010 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
/**
 * @file GLATEventCube.cpp
 * @brief GLATEventCube class implementation.
 * @author J. Knodlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <iostream>
#include "GException.hpp"
#include "GLATEventCube.hpp"
#include "GFitsImageFlt.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_LOAD_CNTMAP                 "GLATEventCube::load_cntmap(GFitsHDU*)"
#define G_POINTER                               "GLATEventCube::pointer(int)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                  GLATEventCube constructors/destructors                 =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Constructor
 ***************************************************************************/
GLATEventCube::GLATEventCube() : GEventCube()
{
    // Initialise class members for clean destruction
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] cube Event cube from which the instance should be built.
 ***************************************************************************/
GLATEventCube::GLATEventCube(const GLATEventCube& cube) : GEventCube(cube)
{
    // Initialise class members for clean destruction
    init_members();

    // Copy members
    copy_members(cube);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GLATEventCube::~GLATEventCube()
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                        GLATEventCube operators                          =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] cube Event cube to be assigned.
 ***************************************************************************/
GLATEventCube& GLATEventCube::operator= (const GLATEventCube& cube)
{
    // Execute only if object is not identical
    if (this != &cube) {

        // Copy base class members
        this->GEventCube::operator=(cube);

        // Free members
        free_members();

        // Initialise private members for clean destruction
        init_members();

        // Copy members
        copy_members(cube);

    } // endif: object was not identical

    // Return this object
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                      GLATEventCube public methods                       =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Load LAT counts map from FITS file.
 *
 * @param[in] filename Counts map FITS filename to be loaded.
 *
 * It is assumed that the counts map resides in the primary extension of the
 * FITS file and that the energy boundaries reside in the extension named
 * EBOUNDS.
 ***************************************************************************/
void GLATEventCube::load(const std::string& filename)
{
    // Free and initialise base class members
    this->GEvents::free_members();
    this->GEvents::init_members();

    // Free and initialise base class members
    this->GEventCube::free_members();
    this->GEventCube::init_members();

    // Free and initialise class members
    free_members();
    init_members();

    // Allocate FITS file
    GFits file;

    // Open counts map FITS file
    file.open(filename);

    // Get HDUs
    GFitsHDU* hdu_cntmap  = file.hdu("Primary");
    GFitsHDU* hdu_ebounds = file.hdu("EBOUNDS");

    // Load counts map
    load_cntmap(hdu_cntmap);

    // Load energy boundaries
    load_ebds(hdu_ebounds);

    // Close FITS file
    file.close();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Get pointer to element
 *
 * @param[in] index Event index for which pointer will be returned.
 *
 * TODO: Implement conversion routine from event cube index to sky direction
 ***************************************************************************/
GLATEventBin* GLATEventCube::pointer(int index)
{
    #if defined(G_RANGE_CHECK)
    if (index < 0 || index >= m_elements)
        throw GException::out_of_range(G_POINTER, index, 0, m_elements-1);
    #endif

    // Get energy bin
    int ieng = index / m_pixels;

    // Setup bin attributes
    m_bin.m_counts = &(m_counts[index]);
    m_bin.m_time   = &m_time;
    //m_bin.m_dir    = ;
    m_bin.m_energy = &(m_energies[ieng]);

    // Return pointer
    return (GLATEventBin*)&(m_bin);
}


/***********************************************************************//**
 * @brief Return number of events in cube
 ***************************************************************************/
int GLATEventCube::number(void) const
{
    // Initialise result
    double number = 0.0;

    // Sum event cube
    if (m_elements > 0 && m_counts != NULL) {
        for (int i = 0; i < m_elements; ++i)
            number += m_counts[i];
    }

    // Return
    return int(number+0.5);
}


/*==========================================================================
 =                                                                         =
 =                      GLATEventCube private methods                      =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GLATEventCube::init_members(void)
{
    // Initialise members
    m_bin      = GLATEventBin();
    m_nx       = 0;
    m_ny       = 0;
    m_pixels   = 0;
    m_ebins    = 0;
    m_counts   = NULL;
    m_energies = NULL;
    m_time.met(0.0);
    m_dirs     = NULL;
    m_ebds     = GEbounds();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] cube GLATEventCube members which should be copied.
 ***************************************************************************/
void GLATEventCube::copy_members(const GLATEventCube& cube)
{
    // Copy LAT specific attributes
    m_bin    = cube.m_bin;
    m_nx     = cube.m_nx;
    m_ny     = cube.m_ny;
    m_pixels = cube.m_pixels;
    m_ebins  = cube.m_ebins;
    m_time   = cube.m_time;
    m_ebds   = cube.m_ebds;

    // Copy cube
    if (m_elements > 0 && cube.m_counts != NULL) {

        // Allocate memory
        m_counts = new double[m_elements];

        // Copy counts
        for (int i = 0; i < m_elements; ++i)
            m_counts[i] = cube.m_counts[i];

    }

    // Copy bin energies
    if (m_ebins > 0 && cube.m_energies != NULL) {

        // Allocate memory
        m_energies = new GEnergy[m_ebins];

        // Copy sky directions
        for (int i = 0; i < m_ebins; ++i)
            m_energies[i] = cube.m_energies[i];

    }

    // Copy sky directions
    if (m_pixels > 0 && cube.m_dirs != NULL) {

        // Allocate memory
        m_dirs = new GSkyDir[m_pixels];

        // Copy sky directions
        for (int i = 0; i < m_pixels; ++i)
            m_dirs[i] = cube.m_dirs[i];

    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GLATEventCube::free_members(void)
{
    // Free memory
    if (m_counts   != NULL) delete [] m_counts;
    if (m_energies != NULL) delete [] m_energies;
    if (m_dirs     != NULL) delete [] m_dirs;

    // Signal free pointers
    m_counts   = NULL;
    m_energies = NULL;
    m_dirs     = NULL;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone class
***************************************************************************/
GLATEventCube* GLATEventCube::clone(void) const
{
    return new GLATEventCube(*this);
}


/***********************************************************************//**
 * @brief Load LAT counts map from HDU.
 *
 * @param[in] hdu Pointer to FITS HDU from which events are loaded.
 *
 * @todo Set sky directions
 *
 * Assumes that 'm_counts' and 'm_dirs' are free.
 ***************************************************************************/
void GLATEventCube::load_cntmap(GFitsHDU* hdu)
{
    // Main loop
    do {

        // Fall through if HDU is invalid
        if (hdu == NULL)
            continue;

        // Get counts map dimension. Fall through if zero.
        m_dim = hdu->card("NAXIS")->integer();
        if (m_dim <= 0) {
            m_dim = 0;
            continue;
        }

        // Get axis dimensions
        m_naxis = new int[m_dim];
        if (m_naxis == NULL)
            throw GException::mem_alloc(G_LOAD_CNTMAP, m_dim);
        char keyword[10];
        for (int i = 0; i < m_dim; ++i) {
            sprintf(keyword, "NAXIS%d", i+1);
            m_naxis[i] = hdu->card(std::string(keyword))->integer();
        }

        // Set number of pixels
        m_nx     = m_naxis[0];
        m_ny     = m_naxis[1];
        m_pixels = m_nx*m_ny;

        // Compute number of elements
        m_elements = 1;
        for (int i = 0; i < m_dim; ++i)
            m_elements *= m_naxis[i];

        // Fall through if the number of elements is zero
        if (m_elements <= 0) {
            m_elements = 0;
            continue;
        }

        // Get image pointer
        GFitsImageFlt* ptr = (GFitsImageFlt*)hdu->data();

        // Allocate event cube bins
        m_counts = new double[m_elements];
        if (m_counts == NULL)
            throw GException::mem_alloc(G_LOAD_CNTMAP, m_elements);

        // Copy pixels into counts map
        for (int i = 0; i < m_elements; ++i)
            m_counts[i] = (ptr->pixels())[i];

        // Allocate sky directions
        m_dirs = new GSkyDir[m_pixels];
        if (m_dirs == NULL)
            throw GException::mem_alloc(G_LOAD_CNTMAP, m_pixels);

        // Set sky direction
        //TODO TODO TODO

    } while (0); // end of main loop

    // Return
    return;
}


/***********************************************************************//**
 * @brief Load energy boundaries from HDU.
 *
 * @param[in] hdu Pointer to FITS HDU from which energy boundaries are loaded.
 *
 * Assumes that the energy array 'm_energies' is free.
 ***************************************************************************/
void GLATEventCube::load_ebds(GFitsHDU* hdu)
{
    // Main loop
    do {

        // Fall through if HDU is invalid
        if (hdu == NULL)
            continue;

        // Load energy boundaries
        m_ebds.load(hdu);

        // Fall through if there are no energy boundaries
        if (m_ebds.elements() < 1)
            continue;

        // Set number of energy bins
        m_ebins = m_ebds.elements();

        // Setup bin energies
        m_energies = new GEnergy[m_ebins];

        // Set log mean energies
        for (int i = 0; i < m_ebins; ++i)
            m_energies[i].MeV(m_ebds.elogmean(i));

    } while (0); // end of main loop

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                         GLATEventCube friends                           =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Put LAT event cube in output stream
 *
 * @param[in] os Output stream into which the event cube will be dumped
 * @param[in] cube Event cube to be dumped
 ***************************************************************************/
std::ostream& operator<< (std::ostream& os, const GLATEventCube& cube)
{
    // Put LAT event list in output stream
    os << "=== GLATEventCube ===" << std::endl;
    os << " Number of elements ........: " << cube.size() << std::endl;
    os << " Number of pixels ..........: " << cube.pixels() << std::endl;
    os << " Number of energy bins .....: " << cube.ebins() << std::endl;
    os << " Number of events ..........: " << cube.number() << std::endl;
    os << cube.m_ebds;

    // Return output stream
    return os;
}


/*==========================================================================
 =                                                                         =
 =                  Other functions used by GLATEventCube                  =
 =                                                                         =
 ==========================================================================*/
