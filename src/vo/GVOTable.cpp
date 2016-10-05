/***************************************************************************
 *                     GVOTable.cpp - VO Table composition class           *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2015 by Thierry Louge                               *
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
 * @file GVOTable.cpp
 * @brief Implements IVOA standard Recommendation 2013-09-20 VOTable1.3
 * @author Thierry Louge
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <cstdlib>         // std::getenv() function
#include <cstdio>          // std::fopen(), etc. functions
#include <cstring>         // std::memset() function
#include <csignal>         // signal() function
#include <cerrno>          // errno
#include <unistd.h>        // close() function
#include <netdb.h>         // getaddrinfo() function
#include <netinet/in.h>    // sockaddr_in, INADDR_ANY, htons
#include <fstream>
#include <sys/shm.h>
#include <sys/socket.h>    // socket(), connect() functions
#include <sys/wait.h>      // waitpid() function
#include <arpa/inet.h>     // inet_addr() function
#include "GVOTable.hpp"
#include "GException.hpp"
#include "GXml.hpp"

/* __ Method name definitions ____________________________________________ */

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */
//#define G_CONSOLE_DUMP
#define G_SHOW_MESSAGE


/*==========================================================================
 =                                                                         =
 =                         Constructors/destructors                        =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 ***************************************************************************/
GVOTable::GVOTable(void)
{
    // Initialise members
    init_members();

    // Return
    return;
}
/***********************************************************************//**
 * @brief Constructor for generic xml case
 ***************************************************************************/
GVOTable::GVOTable(const std::string& filename)
{
    // Initialise members
    init_members();
    //Set up the preliminary structure
    GXml xmlhandler = GXml(filename);
    m_sharedtablename = std::tmpnam(NULL);
    xmlhandler.save(m_sharedtablename+".xml");
    //m_tablexml = GXml(filename);
    //m_tablexml->save(m_sharedtablename+".xml");
    //close_votable();
    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] VOTable
 ***************************************************************************/
GVOTable::GVOTable(const GVOTable& votable)
{
    // Initialise members
    init_members();
    
    // Copy members
    copy_members(votable);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GVOTable::~GVOTable(void)
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
 * @param[in] VO table.
 * @return VO table.
 ***************************************************************************/
GVOTable& GVOTable::operator=(const GVOTable& votable)
{
    // Execute only if object is not identical
    if (this != &votable) {

        // Free members
        free_members();

        // Initialise members
        init_members();

        // Copy members
        copy_members(votable);

    } // endif: object was not identical

    // Return
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                             Public methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Clear object.
 *
 * Reset object to a clean initial state.
 ***************************************************************************/
void GVOTable::clear(void)
{
    // Free members
    free_members();

    // Initialise members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone object
 ***************************************************************************/
GVOTable* GVOTable::clone(void) const
{
    // Clone client
    return new GVOTable(*this);
}


/***********************************************************************//**
 * @brief Print VO Table information
 *
 * @param[in] chatter Chattiness (defaults to NORMAL).
 * @return String containing VO table information
 ***************************************************************************/
std::string GVOTable::print(const GChatter& chatter) const
{
    // Initialise result string
    std::string result;

    // Continue only if chatter is not silent
    if (chatter != SILENT) {

        // Append header
        result.append("=== GVOTable ===");
	result.append(m_header);
	result.append(m_fields);    
    	result.append(m_data);
    	result.append(m_footer);

    } // endif: chatter was not silent

    // Return result
    return result;
}
/***********************************************************************//**
 * @brief Opens the VO table, set the header
 *
 ***************************************************************************/
void GVOTable::open_votable(void)
{
    m_header.append("<?xml version=\"1.0\"?>\n");
    // m_header.append("<VOTABLE version=\"1.3\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n");
    //m_header.append("xmlns=\"http://www.ivoa.net/xml/VOTable/v1.3\"\n");
    //STC is recommended space time coordinates datamodel for galaxies
    m_header.append("xmlns:stc=\"http://www.ivoa.net/xml/STC/v1.30\">\n");
    //m_header.append("\t<RESOURCE name=\"Gammalib\">\n");
    //m_header.append("\t\t<TABLE name=\"GammalibTable\">\n");
    //m_header.append("\t\t\t<DESCRIPTION>VOTable coming from gammalib</DESCRIPTION>\n");
    // Return
    return;
}
/***********************************************************************//**
 * @brief Set the footer, closes the VO table
 *
 ***************************************************************************/
void GVOTable::close_votable(void)
{
    m_footer.append("\t\t</TABLE>\n");
    m_footer.append("\t</RESOURCE>\n");
    m_footer.append("</VOTABLE>\n");

    std::string table = m_header + m_wcs + m_pixels + m_fields + m_data + m_footer;	
    m_tablexml = new GXml(table);
    m_tablexml->save("VOTableExemple.xml");
    // Return
    return;
}
/***********************************************************************//**
 * @brief Fills <FIELD> in the VO table
 *
 * @param[in] FIELD characteristics, like: name="_RAJ2000" ucd="pos.eq.ra" 
 * ref="J2000" datatype="double" width="10" precision="6" unit="deg"
 ***************************************************************************/
void GVOTable::fill_fields(const std::string& name, const std::string& ucd,
	const std::string& id, const std::string& datatype, 
	const std::string& width,const std::string& precision,
	const std::string& unit,const std::string& description)
{
    m_fields.append("\t\t\t<FIELD  name=\""+name+"\" ucd=\""+ucd+"\" ID=\""+id+"\" datatype=\""+datatype+"\" width=\""+width+"\" precision=\""+precision+"\" unit=\""+unit+"\">\n");
    m_fields.append("\t\t\t\t<DESCRIPTION>"+description+"</DESCRIPTION>\n");
    m_fields.append("\t\t\t</FIELD>\n");
    // Return
    return;
}
/***********************************************************************//**
 * @brief Fills TABLEDATA of the VO table
 *
 ***************************************************************************/
void GVOTable::fill_tabledata(const std::string& data)
{
    m_data.append("<TD>"+data+"</TD>");
    // Return
    return;
}

/***********************************************************************//**
 * @brief inits TABLEDATA part of the VO table
 *
 ***************************************************************************/
void GVOTable::init_tabledata(void)
{
    m_data.append("\t\t\t<DATA>\n");
    m_data.append("\t\t\t\t<TABLEDATA>\n");
    m_data.append("\t\t\t\t\t<TR>");
    // Return
    return;
}
/***********************************************************************//**
 * @brief closes TABLEDATA part of the VO table
 *
 ***************************************************************************/
void GVOTable::close_tabledata(void)
{
    m_data.append("\t\t\t\t\t</TR>");
    m_data.append("\t\t\t\t</TABLEDATA>\n");
    m_data.append("\t\t\t</DATA>\n");
    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                             Private methods                             =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GVOTable::init_members(void)
{
    // Initialise members
    m_header = "";
    m_fields = "";
    m_data = "";
    m_footer = "";
    m_pixels = "";
    m_wcs = "";
    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] hub VO hub.
 ***************************************************************************/
void GVOTable::copy_members(const GVOTable& hub)
{
    // Copy members

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GVOTable::free_members(void)
{ 
    
    // Return
    return;
}


