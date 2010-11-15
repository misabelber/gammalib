/***************************************************************************
 *               GFitsHeaderCard.hpp  - FITS header card class             *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2008-2010 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GFitsHeaderCard.hpp
 * @brief GFitsHeaderCard class definition.
 * @author J. Knodlseder
 */

#ifndef GFITSHEADERCARD_HPP
#define GFITSHEADERCARD_HPP

/* __ Includes ___________________________________________________________ */


/***********************************************************************//**
 * @class GFitsHeaderCard
 *
 * @brief Implements FITS header card interface
 *
 * This class implements FITS header card. Each card consists of a
 * keyname (string), a value (string, floating pointer, integer or logical)
 * and a comment (string). COMMENT or HISTORY cards do not have any value.
 ***************************************************************************/
class GFitsHeaderCard {

    // Friend classes
    friend class GFitsHeader;

    // I/O friends
    friend std::ostream& operator<< (std::ostream& os, const GFitsHeaderCard& card);

public:
    // Constructors & Destructors
    GFitsHeaderCard(void);
    GFitsHeaderCard(const std::string& keyname, const std::string& value,
                    const std::string& comment);
    GFitsHeaderCard(const std::string& keyname, const double& value,
                    const std::string& comment);
    GFitsHeaderCard(const std::string& keyname, const int& value,
                    const std::string& comment);
    GFitsHeaderCard(const GFitsHeaderCard& card);
    virtual ~GFitsHeaderCard(void);

    // Operators
    GFitsHeaderCard& operator= (const GFitsHeaderCard& card);

    // Methods to set card properties
    void         keyname(const std::string& keyname);
    void         value(const std::string& value);
    void         value(const double& value);
    void         value(const int& value);
    void         unit(const std::string& unit);
    void         comment(const std::string& comment);

    // Methods to get card properties
    std::string  keyname(void) const;
    std::string  value(void) const;
    int          value_type(void) const;
    int          decimals(void) const;
    std::string  unit(void) const;
    std::string  comment(void) const;
    std::string  string(void);
    double       real(void);
    int          integer(void);

private:
    // Private methods
    void init_members(void);
    void copy_members(const GFitsHeaderCard& card);
    void free_members(void);
    int  get_value_type(const std::string& value);
    void read(void* vptr, int keynum);
    void read(void* fptr, const std::string& keyname);
    void write(void* fptr);

    // Private data area
    std::string m_keyname;         //!< Name of the card
    std::string m_value;           //!< Value of the card as read from file
    int         m_value_type;      //!< Type of the card value
    int         m_value_decimals;  //!< Decimals of value (for float)
    std::string m_unit;            //!< Unit of the card value
    std::string m_comment;         //!< Card comment
    int         m_comment_write;   //!< Signals that comment should be written
};

#endif /* GFITSHEADERCARD_HPP */
