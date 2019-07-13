#pragma once

#include "geometry.h"

#include <boost/uuid/uuid.hpp>

#include <iostream>
#include <string>
#include <variant>
#include <vector>

/*!
 * \brief The ParameterValue class
 * This class holds a value for a parameter. This value can be a number of types, including
 * bool, int, float, string, etc.
 */
class ParameterValue
{
public:
  /*!
   * \brief The ValueType enum
   * This enum defines the types the ParameterValue can hold.
   */
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

  /*!
   * \brief operator ==
   * Comparison operator.
   * \param other the value to compare to.
   * \return true if the values are equal.
   */
  bool operator==(ParameterValue const& other);

  /*!
   * \brief Constructor.
   * Assigns type and value.
   * \param value the value this object holds.
   */
  template <typename T>
  ParameterValue(T value) : _value(value)
  {
  }

  /*!
   * \brief Assignment operator.
   * This makes sure that the type is conserved in direct assignment.
   * \param value the new value of the object.
   * \return this
   */
  template <typename T>
  ParameterValue& operator=(T value)
  {
    if (std::holds_alternative<T>(_value)) {
      _value = value;
    }
    return *this;
  }

  /*!
   * \brief Determines whether a certain type is held by this object.
   * \return true if the specified type is in this object.
   */
  template <typename T>
  bool contains()
  {
    return std::holds_alternative<T>(_value);
  }

  /*!
   * \brief Get the held value.
   * \return the value the object holds.
   */
  template <typename T>
  T const& value()
  {
    return std::get<T>(_value);
  }

  /*!
   * \brief valueType
   * \return the value type of this object.
   */
  ValueType valueType() const;

  /*!
   * \brief isDefined
   * \return true if this object has been defined with a value.
   */
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
  bool operator==(base_parameter p);

private:
  std::string _name;
  std::string _description;
  boost::uuids::uuid _id;
};

/*!
 * \brief The values_parameter class
 * This class holds a list of named parameters of any kind. This will be useful for algorithms as
 * well as shapes.
 */
class values_parameter : public base_parameter
{
public:
  using base_parameter::base_parameter;
  values_parameter() = default;
  values_parameter(values_parameter const&) = default;
  values_parameter(values_parameter&&) = default;
  values_parameter& operator=(values_parameter const&) = default;
  values_parameter& operator=(values_parameter&&) = default;

  /*!
   * \brief numberOfValues
   * \return the number of values stored for this object.
   */
  size_t numberOfValues() const;

  /*!
   * \brief valueName
   * Get the name of a specific value.
   * \param index the index of the wanted value.
   * \return the name of the specified value.
   */
  std::string valueName(size_t index) const;

  /*!
   * \brief value
   * Get the stored value of a specific value.
   * \param index the index of the wanted value.
   * \return the value at the specified index.
   */
  ParameterValue const& value(size_t index) const;

  /*!
   * \brief value
   * Get the stored value of a specific value.
   * \param index the index of the wanted value.
   * \return the value at the specified index.
   */
  ParameterValue& value(size_t index);

  /*!
   * \brief value
   * Get the stored value of a specific value.
   * Note: make sure before that the template parameter is of the correct type, else an exception
   * will be thrown.
   * \param index the index of the wanted value.
   * \return the value at the specified index.
   */
  template <typename T>
  T const& value(size_t index) const
  {
    return _values[index].second.value<T>();
  }

  /*!
   * \brief contains
   * Query whether the value in question contains a specified type.
   * \param index the index of the value in question.
   * \return true if the specified value is of the named type.
   */
  template <typename T>
  bool contains(size_t index) const
  {
    return _values[index].second.contains<T>();
  }

  /*!
   * \brief addValue
   * Adds a value to the value list.
   * \param name the name of the new value,
   * \param value the new value.
   */
  void addValue(std::string const& name, ParameterValue const& value);

  /*!
   * \brief removeValue
   * Removes a specific value.
   * \param index the index of the value to remove.
   */
  void removeValue(size_t index);

  /*!
   * \brief clearValues
   * Clear all values.
   */
  void clearValues();

private:
  std::vector<std::pair<std::string, ParameterValue>> _values;
};
