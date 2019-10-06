#pragma once
#include "geometry.h"
#include "valuesparameter.h"

/*!
 * \brief The algorithm_parameter class
 */
class algorithm_parameter : public values_parameter
{
public:
  enum class Category
  {
    INPUT,
    OUTPUT,
    CALIBRATION,
    PIXEL,
    TRACKING,
    POST_TRACKING,
    HISTOGRAMMING,
    FITTING,
    ELSE,
  };

public:
  algorithm_parameter() = default;
  algorithm_parameter(const algorithm_parameter& ap) = default;
  algorithm_parameter(algorithm_parameter&& ap) = default;
  algorithm_parameter& operator=(const algorithm_parameter& p) = default;
  algorithm_parameter& operator=(algorithm_parameter&& p) = default;

  /*!
   * \brief algorithm_parameter
   *  Constructor. Sets the name and the ID of the algorithm.
   * \param id
   */
  algorithm_parameter(std::string, int id);

  /*!
   * \brief algorithm_parameter
   *  Constructor. Sets the algorithms name, level and category and its ID.
   * \param id
   */
  algorithm_parameter(std::string, Category, int, int id = -1);

  /*!
   * \brief algorithm_parameter
   *  Constructor. Sets the constructors name, weather it is used, its level and category and the ID
   * (-1 by default).
   * \param id
   */
  algorithm_parameter(std::string, bool, Category, int, int id = -1);

  /*!
   * \brief operator ==
   *  Comparison operator. Returns true if the algorithms ID is equal the one of p. Needed for the
   * sorting algorithm of the root-TObjArray-class.
   * \param p
   * \return
   */
  bool operator==(const algorithm_parameter& p) const;

  /*!
   * \brief setCategory
   *  Sets the category of the parameter in display with the analysis program typeCase.
   * \param cat
   */
  virtual void setCategory(Category cat);

  /*!
   * \brief getCategory
   *  Returns the category of the parameter in display with the analysis program typeCase.
   * \return
   */
  virtual Category getCategory() const;

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

private:
  Category category{Category::ELSE};
  int level{-1};
  int ID{-1};
  bool inUse{false};
};

/*!
 * \brief to_string
 * \param category the category to convert.
 * \return a string representation of the named category.
 */
std::string to_string(algorithm_parameter::Category category);

/*!
 * \brief operator <<
 * \param o
 * \param a
 * \return
 */
ostream& operator<<(ostream& o, const algorithm_parameter& a);
