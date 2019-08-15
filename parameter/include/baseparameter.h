#pragma once

#include "geometry.h"

#include <boost/uuid/uuid.hpp>

#include <iostream>
#include <string>

/*!
 * \brief The base_parameter class
 */
class base_parameter
{
public:
  base_parameter() = default;
  base_parameter(base_parameter const&) = default;
  base_parameter(base_parameter&&) = default;
  virtual ~base_parameter() = default;
  base_parameter& operator=(base_parameter const&) = default;
  base_parameter& operator=(base_parameter&&) = default;

  /*!
   * \brief setName
   *  Set the name.
   * \param n
   */
  void setName(std::string const& n);

  /*!
   * \brief setDescription
   *  Set the description.
   * \param d
   */
  void setDescription(std::string const& d);

  /*!
   * \brief getName
   *  Returns the name of the parameter.
   * \return
   */
  std::string getName() const;

  /*!
   * \brief getDescription
   *  Returns the description of the parameter.
   * \return
   */
  std::string getDescription() const;

  /*!
   * \brief setId
   * Sets the id of the parameter.
   * \param id the new id of the parameter.
   */
  void setId(boost::uuids::uuid const& id);

  /*!
   * \brief id
   * Returns the unique id of the parameter.
   * \return the id of the parameter.
   */
  boost::uuids::uuid id() const;

  /*!
   * \brief operator ==
   *  Comparison operator. Returns true if the name of the parameter is equal to the one of p.
   * \param p
   * \return
   */
  bool operator==(base_parameter const& p) const;

private:
  std::string _name;
  std::string _description;
  boost::uuids::uuid _id;
};
