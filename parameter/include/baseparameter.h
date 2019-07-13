#pragma once

#include "geometry.h"

#include <boost/uuid/uuid.hpp>

#include <iostream>
#include <string>
#include <variant>
#include <vector>

class ParameterValue
{
public:
  enum class ValueType
  {
    BOOLEAN,
    INT,
    FLOAT,
    DOUBLE,
    POINT3D,
    VECTOR3D,
    STRING,
    VECTOR_INT,
    VECTOR_FLOAT,
    VECTOR_DOUBLE,
    VECTOR_STRING,
    UNDEFINED,
  };

  ParameterValue() = default;
  ParameterValue(ParameterValue const&) = default;
  ParameterValue(ParameterValue&&) = default;
  ParameterValue& operator=(ParameterValue const&) = default;
  ParameterValue& operator=(ParameterValue&&) = default;
  bool operator==(ParameterValue const& other);

  template <typename T>
  ParameterValue(T value) : _value(value)
  {
  }
  template <typename T>
  ParameterValue& operator=(T value)
  {
    _value = value;
    return *this;
  }

  template <typename T>
  bool contains()
  {
    return std::holds_alternative<T>(_value);
  }

  template <typename T>
  T const& value()
  {
    return std::get<T>(_value);
  }

  ValueType valueType() const;

  bool isDefined() const;

private:
  using Type =
      std::variant<bool, int, float, double, point3D, vector3D, std::string, std::vector<int>,
                   std::vector<float>, std::vector<double>, std::vector<std::string>>;
  Type _value;
};

/*!
 *
 */
template <class X>
class single_parameter
{
private:
  std::string name; //!
  X data;

public:
  /*!
   * \brief single_parameter
   * Default constructor.
   */
  single_parameter() = default;

  /*!
   * \brief single_parameter
   *  Constructor. Sets the name of the parameter and its data.
   * \param Name
   * \param Data
   */
  single_parameter(const std::string& Name, X Data);

  /*!
   * \brief single_parameter
   *  Copy operator.
   * \param sp
   */
  single_parameter(const single_parameter& sp) = default;

  /*!
   * \brief Destructor
   */
  ~single_parameter() = default;

  /*!
   * \brief operator =
   *  Copy operator.
   * \param s
   * \return
   */
  single_parameter& operator=(const single_parameter& s) = default;

  /*!
   * \brief operator ==
   *  Comparison operator. Returns true if the two single_parameter have the same name.
   * \param s
   * \return
   */
  bool operator==(const single_parameter& s) const { return s.data == data && s.name == name; }

  /*!
   * \brief operator ==
   *  Comparison operator. Returns true if the single_parameters has this name.
   * \param s
   * \return
   */
  bool operator==(const std::string& s) const;

  /*!
   * \brief getName
   *  Returns the parameters name.
   * \return
   */
  std::string getName() const;

  /*!
   * \brief setName
   *  Sets the parameters name.
   * \param n
   */
  void setName(const std::string& n);

  /*!
   * \brief getData
   * Returns the parameters value.
   * \return
   */
  X getData() const;

  /*!
   * \brief setData
   * Sets the values for the parameter.
   * \param d
   */
  void setData(X d);
};

/*!
 * \brief The base_parameter class
 */
class base_parameter
{
public:
  /*!
   * \brief base_parameter
   *  Default constructor.
   */
  base_parameter() = default;
  virtual ~base_parameter() = default;

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
  bool operator==(base_parameter p);

private:
  std::string _name;
  std::string _description;
  boost::uuids::uuid _id;
};
