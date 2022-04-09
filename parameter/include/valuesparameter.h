#pragma once

#include "baseparameter.h"

#include <memory>
#include <variant>
#include <vector>

template <typename...>
struct is_one_of
{
  static constexpr bool value = false;
};

template <typename T, typename S, typename... Ts>
struct is_one_of<T, S, Ts...>
{
  static constexpr bool value = std::is_same<T, S>::value || is_one_of<T, Ts...>::value;
};

class algorithm_parameter;

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
    UNDEFINED = -1,
    BOOLEAN = 0,
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
    ALGORITHM,
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
  bool operator==(ParameterValue const& other) const;

  /*!
   * \brief operator ==
   * Comparison operator.
   * \param other the value to compare to.
   * \return true if the values are equal.
   */
  template <typename T>
  bool operator==(T const& other) const
  {
    if (!std::holds_alternative<T>(_value)) {
      return false;
    }
    return std::get<T>(_value) == other;
  }

  /*!
   * \brief Constructor.
   * Assigns type and value.
   * \param value the value this object holds.
   */
  template <typename T, typename = typename std::enable_if<is_one_of<
                            T, bool, int, float, double, point3D, vector3D, std::string,
                            std::vector<int>, std::vector<float>, std::vector<double>,
                            std::vector<std::string>, std::shared_ptr<algorithm_parameter>>::value>>
  ParameterValue(T value) : _value(value)
  {
  }

  /*!
   * \brief Constructor.
   * Specialization for strings.
   * \param value the value this object holds.
   */
  ParameterValue(char const* value) : _value(std::string{value}) {}

  /*!
   * \brief Assignment operator.
   * This makes sure that the type is conserved in direct assignment.
   * \param value the new value of the object.
   * \return this
   */
  template <typename T, typename = typename std::enable_if<is_one_of<
                            T, bool, int, float, double, point3D, vector3D, std::string,
                            std::vector<int>, std::vector<float>, std::vector<double>,
                            std::vector<std::string>, std::shared_ptr<algorithm_parameter>>::value>>
  ParameterValue& operator=(T value)
  {
    if (std::holds_alternative<T>(_value)) {
      _value = value;
    }
    return *this;
  }

  /*!
   * \brief Assignment operator.
   * This makes sure that the type is conserved in direct assignment.
   * Overload for string assignment.
   * \param value the new value of the object.
   * \return this
   */
  ParameterValue& operator=(char const* value)
  {
    if (std::holds_alternative<std::string>(_value)) {
      _value = std::string{value};
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
  T const& value() const
  {
    return std::get<T>(_value);
  }

  /*!
   * \brief Get the held value.
   * \return the value the object holds.
   */
  template <typename T>
  T& value()
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
  using Type = std::variant<bool, int, float, double, point3D, vector3D, std::string,
                            std::vector<int>, std::vector<float>, std::vector<double>,
                            std::vector<std::string>, std::shared_ptr<algorithm_parameter>>;
  Type _value;
};

std::ostream& operator<<(std::ostream& o, ParameterValue const& v);

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
  bool operator==(values_parameter const&) const;

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
