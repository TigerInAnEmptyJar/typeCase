#ifndef __SHAPEPARAM
#define __SHAPEPARAM
#include "baseparameter.h"
#include "geometry.h"

#include <boost/uuid/uuid.hpp>
#include <vector>
using namespace std;

/*!
 * \brief The shape_parameter class
 */
class shape_parameter : public base_parameter
{
private:
  vector<single_parameter<point3D>> points;   //!
  vector<single_parameter<vector3D>> vectors; //!
  vector<single_parameter<int>> integers;     //!
  vector<single_parameter<float>> floats;     //!
  vector<single_parameter<string>> strings_;  //!
  bool fcompleteWrite;                        //!
  boost::uuids::uuid id;

public:
  /*!
   * \brief shape_parameter
   *  Default constructor.
   */
  shape_parameter();

  /*!
   * \brief shape_parameter
   *  Copy constructor.
   * \param sp
   */
  shape_parameter(const shape_parameter& sp);

  /*!
   * \brief Destructor
   */
  ~shape_parameter();

  /*!
   * \brief get the id of the shape
   * \return the id of the shape.
   */
  boost::uuids::uuid getId() const;

  /*!
   * \brief set the id of the shape
   * \param newId the new id of the shape
   */
  void setId(boost::uuids::uuid newId);

  /*!
   * Template method. Returns the ith parameters value stored for the specified type. This can be
   * int, float, point3D, vector3D or QString.
   */
  template <class T>
  T getParam(int i) const;

  /*!
   * Template method. Returns the ith parameters name stored for the specified type. This can be
   * int, float, point3D, vector3D or QString.
   */
  template <class T>
  string getParamName(int i) const;

  /*!
   * Template method. Sets the ith parameters value of the specified type. This can be int, float,
   * point3D, vector3D or QString.
   */
  template <class T>
  void setParam(int i, const T& p);

  /*!
   * Template method. Sets the ith parameters name stored for the specified type. This can be int,
   * float, point3D, vector3D or QString.
   */
  template <class T>
  void setParamName(int i, string n);

  /*!
   * Template method. Adds a parameter to the list of parameters stored for the specified type. This
   * can be int, float, point3D, vector3D or QString.
   */
  template <class T>
  void addParam(T p, string n);

  /*!
   * Template method. Returns the number of parameters stored for the specified type. This can be
   * int, float, point3D, vector3D or QString.
   */
  template <class T>
  int NumberOfParams() const;

  /*!
   * \brief clearProperties
   *  Empties all parameters.
   */
  void clearProperties();

  /*!
   * \brief operator =
   *  Copy operator.
   * \param sh
   */
  void operator=(const shape_parameter& sh);

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
};

/*!
 * \brief operator >>
 * \param i
 * \param sh
 * \return
 */
istream& operator>>(istream& i, shape_parameter& sh);

/*!
 * \brief operator <<
 * \param o
 * \param sh
 * \return
 */
ostream& operator<<(ostream& o, const shape_parameter& sh);

/*!
 *
 */
template <>
bool takeItemFromVector<shape_parameter>(vector<shape_parameter>& vect, shape_parameter& item);

/*!
 *
 */
template <>
bool moveItemToPosVector<shape_parameter>(vector<shape_parameter>& vect, shape_parameter& item,
                                          int pos);
#endif
