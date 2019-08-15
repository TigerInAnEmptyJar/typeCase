#pragma once
#include "baseparameter.h"
#include "geometry.h"

#include <boost/uuid/uuid.hpp>
#include <vector>
using namespace std;

/*!
 * \brief The shape_parameter class
 */
class shape_parameter : public values_parameter
{
public:
  using values_parameter::values_parameter;
  shape_parameter() = default;
  shape_parameter(shape_parameter const&) = default;
  shape_parameter(shape_parameter&&) = default;
  ~shape_parameter() = default;

  shape_parameter& operator=(shape_parameter const&) = default;
  shape_parameter& operator=(shape_parameter&&) = default;

  /*!
   * \brief operator ==
   * Comparison operator.
   * \param other the parameter to compare to.
   * \return true if both parameter are equal.
   */
  bool operator==(shape_parameter const& other) const;

  /*!
   * \brief completeWrite
   *  Retruns wether the values of the parameters should be written in addition to their names.
   * \return
   */
  bool completeWrite() const;

  /*!
   * \brief setCompleteWrite
   *  Complete-write is when in addition to the names of the parameters, the values are written to
   * ostream with the "operator<<". This property is set here.
   * \param value
   */
  void setCompleteWrite(bool value = true);

private:
  bool _completeWrite;
};

/*!
 * \brief operator <<
 * \param o
 * \param sh
 * \return
 */
ostream& operator<<(ostream& o, const shape_parameter& sh);
