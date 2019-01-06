#ifndef __BASEPARAM
#define __BASEPARAM

#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <class X>
bool takeItemFromVector(vector<X>&, X&);

template <class X>
bool moveItemToPosVector(vector<X>&, X&, int);

/*!
 *
 */
template <class X>
class single_parameter
{
private:
  string name; //!
  X data;

public:
  /*!
   * \brief single_parameter
   * Default constructor.
   */
  single_parameter();

  /*!
   * \brief single_parameter
   *  Constructor. Sets the name of the parameter and its data.
   * \param Name
   * \param Data
   */
  single_parameter(const string& Name, X Data);

  /*!
   * \brief single_parameter
   *  Copy operator.
   * \param sp
   */
  single_parameter(const single_parameter& sp);

  /*!
   * \brief Destructor
   */
  ~single_parameter();

  /*!
   * \brief operator =
   *  Copy operator.
   * \param s
   * \return
   */
  single_parameter& operator=(const single_parameter& s);

  /*!
   * \brief operator ==
   *  Comparison operator. Returns true if the two single_parameter have the same name.
   * \param s
   * \return
   */
  bool operator==(const single_parameter& s) const;

  /*!
   * \brief operator ==
   *  Comparison operator. Returns true if the single_parameters has this name.
   * \param s
   * \return
   */
  bool operator==(const string& s) const;

  /*!
   * \brief getName
   *  Returns the parameters name.
   * \return
   */
  string getName() const;

  /*!
   * \brief setName
   *  Sets the parameters name.
   * \param n
   */
  void setName(const string& n);

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
private:
  string name;                //!
  vector<string> description; //!
public:
  /*!
   * \brief base_parameter
   *  Default constructor.
   */
  base_parameter();

  /*!
   * \brief ~base_parameter
   */
  virtual ~base_parameter();

  /*!
   * \brief setName
   *  Set the name.
   * \param n
   */
  void setName(const string& n);

  /*!
   * \brief setDescription
   *  Set the description.
   * \param d
   */
  void setDescription(vector<string> d);

  /*!
   * \brief setDescription
   *  Set the lineth description line to d.
   * \param d
   * \param line
   */
  void setDescription(const string& d, int line);

  /*!
   * \brief getName
   *  Returns the anem of the parameter.
   * \return
   */
  string getName() const;

  /*!
   * \brief getDescription
   *  Returns the lineth description line of the parameter.
   * \param line
   * \return
   */
  string getDescription(int line) const;

  /*!
   * \brief getDescription
   *  Returns the description of the parameter.
   * \return
   */
  vector<string> getDescription() const;

  /*!
   * \brief operator <
   *  Comparison operator. Returns true if the name of the parameter is smaller than one of p.
   * \param p
   * \return
   */
  bool operator<(base_parameter p);

  /*!
   * \brief operator >
   *  Comparison operator. Returns true if the name of the parameter is larger than the one of p.
   * \param p
   * \return
   */
  bool operator>(base_parameter p);

  /*!
   * \brief operator ==
   *  Comparison operator. Returns true if the name of the parameter is equal to the one of p.
   * \param p
   * \return
   */
  bool operator==(base_parameter p);

  /*!
   * \brief operator <=
   *  Comparison operator. Returns true if the name of the parameter is smaller or equal to the one
   * of p.
   * \param p
   * \return
   */
  bool operator<=(base_parameter p);

  /*!
   * \brief operator >=
   *  Comparison operator. Returns true if the name of the parameter is larger or equal to the one
   * of p.
   * \param p
   * \return
   */
  bool operator>=(base_parameter p);
};

#endif
