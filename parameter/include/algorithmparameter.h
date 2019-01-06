#ifndef __ALGPARAM
#define __ALGPARAM
#include "baseparameter.h"
#include "geometry.h"
using namespace std;

/*!
 * \brief The algorithm_parameter class
 */
class algorithm_parameter : public base_parameter
{
private:
  bool inUse;
  int category;
  int level;
  int ID;
  vector<single_parameter<int>> integers;     //!
  vector<single_parameter<float>> floats;     //!
  vector<single_parameter<point3D>> points;   //!
  vector<single_parameter<bool>> switches;    //!
  vector<single_parameter<string>> stringes;  //!
  vector<single_parameter<vector3D>> vectors; //!
  //    vector<single_parameter<QBitArray> > bitmaps; //!
  vector<single_parameter<vector<int>>> intVectors;       //!
  vector<single_parameter<vector<float>>> floatVectors;   //!
  vector<single_parameter<vector<string>>> stringVectors; //!
  vector<single_parameter<algorithm_parameter>> algos;

public:
  /*!
   * \brief algorithm_parameter
   * Default constructor.
   */
  algorithm_parameter();

  /*!
   * \brief algorithm_parameter
   *  Copy constructor.
   * \param ap
   */
  algorithm_parameter(const algorithm_parameter& ap);

  /*!
   * \brief algorithm_parameter
   *  Constructor. Sets the name and the ID of the algorithm.
   * \param id
   */
  algorithm_parameter(string, int id);

  /*!
   * \brief algorithm_parameter
   *  Constructor. Sets the algorithms name, level and category and its ID.
   * \param id
   */
  algorithm_parameter(string, int, int, int id = -1);

  /*!
   * \brief algorithm_parameter
   *  Constructor. Sets the constructors name, weather it is used, its level and category and the ID
   * (-1 by default).
   * \param id
   */
  algorithm_parameter(string, bool, int, int, int id = -1);

  /*!
   * \brief ~algorithm_parameter
   * Destructor
   */
  virtual ~algorithm_parameter();

  /*!
   * \brief setCategory
   *  Sets the category of the parameter in display with the analysis program typeCase.
   * \param cat
   */
  virtual void setCategory(int cat);

  /*!
   * \brief getCategory
   *  Returns the category of the parameter in display with the analysis program typeCase.
   * \return
   */
  virtual int getCategory() const;

  /*!
   * \brief setLevel
   * Sets the level of the parameter in display with the analysis program typeCase.
   * \param lev
   */
  virtual void setLevel(int lev);

  /*!
   * \brief getLevel
   *  Returns the level of the parameter in display with the analysis program typeCase.
   * \return
   */
  virtual int getLevel() const;

  /*!
   * \brief setUse
   *  Sets, if the algorithm described by this parameter should be used in analysis.
   * \param in
   */
  virtual void setUse(bool in);

  /*!
   * \brief IsUsed
   *  Returns true, if the algorithm described by this parameter should be used in analysis.
   * \return
   */
  virtual bool IsUsed() const;

  /*!
   * \brief getID
   *  Returns the ID of the algorithm.
   * \return
   */
  virtual int getID() const;

  /*!
   * \brief setID
   *  Sets the ID of the algorithm.
   * \param id
   */
  virtual void setID(int id);

  /*!
   * Template method. Changes the parameter with name nameof the specified type (bool, int,
   * float,QString, point3D, vector3D, vector, vector, vector, algorithm_parameter). It gets a new
   * value.
   */
  template <class X>
  void changeParam(string name, X data);

  /*!
   * Template method. Changes the posth parameter of the specified type (bool, int, float,QString,
   * point3D, vector3D, vector, vector, vector, algorithm_parameter). It gets a new name and a new
   * value. This only works for the template class QBitArray.
   */
  template <class X>
  void changeParam(string name, const bool data[], int num);

  /*!
   * Template method. Changes the posth parameter of the specified type (bool, int, float,QString,
   * point3D, vector3D, vector, vector, vector, algorithm_parameter). It gets a new name and a new
   * value.
   */
  template <class X>
  void changeParam(string name, X data, int pos);

  /*!
   * Template method. Removes the number of parameters of the specified type (bool, int,
   * float,QString, point3D, vector3D, vector, vector, vector, algorithm_parameter).
   */
  template <class X>
  int getNumberOfParam() const;

  /*!
   * Template method. Returns the ith parameter of the specified type (bool, int, float,QString,
   * point3D, vector3D, vector, vector, vector, algorithm_parameter) from the list of parameters.
   * Warning: no consistency check, breaks if i is larger than the number of parameters stored for
   * this type.
   */
  template <class X>
  single_parameter<X> getParam(int i) const;

  /*!
   * Returns the first parameter with name nme.
   */
  template <class X>
  single_parameter<X> getParam(const string& nme) const;

  /*!
   * Template method. Adds a parameter of the specified type (bool, int, float,QString, point3D,
   * vector3D, vector, vector, vector, algorithm_parameter) to the list of parameters.
   */
  template <class X>
  void addParam(single_parameter<X> data);

  /*!
   * Template method. Removes the last parameter of the specified type (bool, int, float,QString,
   * point3D, vector3D, vector, vector, vector, algorithm_parameter) from the list of parameters.
   */
  template <class X>
  void popParam();

  /*!
   * \brief removeParam
   *  Searches a parameter of that name in the parameter lists, starting with bool, int, float,
   * point3D, vector3D, QString, vector, vector, vector, algorithm_parameter. If a parameter of that
   * name is found, it is removed and it quits the method.
   * \param paramName
   */
  void removeParam(const string& paramName);

  /*!
   * \brief operator <
   *  Comparison operator. Returns true if the algorithms ID is smaller the one of p. Needed for the
   * sorting algorithm of the root-TObjArray-class.
   * \param p
   * \return
   */
  bool operator<(algorithm_parameter p);

  /*!
   * \brief operator >
   *  Comparison operator. Returns true if the algorithms ID is larger the one of p. Needed for the
   * sorting algorithm of the root-TObjArray-class.
   * \param p
   * \return
   */
  bool operator>(algorithm_parameter p);

  /*!
   * \brief operator ==
   *  Comparison operator. Returns true if the algorithms ID is equal the one of p. Needed for the
   * sorting algorithm of the root-TObjArray-class.
   * \param p
   * \return
   */
  bool operator==(const algorithm_parameter& p);

  /*!
   * \brief operator <=
   *  Comparison operator. Returns true if the algorithms ID is smaller or equal the one of p.
   * Needed for the sorting algorithm of the root-TObjArray-class.
   * \param p
   * \return
   */
  bool operator<=(algorithm_parameter p);

  /*!
   * \brief operator >=
   *  Comparison operator. Returns true if the algorithms ID is larger or equal the one of p. Needed
   * for the sorting algorithm of the root-TObjArray-class.
   * \param p
   * \return
   */
  bool operator>=(algorithm_parameter p);

  /*!
   * \brief operator =
   * Copy operator.
   * \param p
   */
  void operator=(const algorithm_parameter& p);

  /*!
   * \brief toString
   * Returns a string representation of the algorithms parameter, name description, etc. Contains
line breaks.
pattern = 1: cathegory, level and used status are shown in one line,
pattern = 2: cathegory, level and used status are shown with description,
pattern = 3: only ID is shown with description,
else: only ID is shown.
In all modes the parameters are shown.
   * \param pattern
   * \return
   */
  string toString(int pattern = 1) const;
};

/*!
 * \brief operator >>
 * \param i
 * \param a
 * \return
 */
istream& operator>>(istream& i, algorithm_parameter& a);

/*!
 * \brief operator <<
 * \param o
 * \param a
 * \return
 */
ostream& operator<<(ostream& o, const algorithm_parameter& a);
#endif
