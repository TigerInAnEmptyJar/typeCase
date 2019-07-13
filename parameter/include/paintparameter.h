#ifndef __PAINTPARAM
#define __PAINTPARAM
#include "baseparameter.h"
using namespace std;

/*!
 * \brief The paint_parameter class
 */
class paint_parameter : public base_parameter
{
private:
  int ID;
  int color;
  int marker;
  bool painted;

public:
  /*!
   * \brief paint_parameter
   *  Constructor. Sets the name, the ID, the color of the object to draw. It also sets weather it
   * shall be painted and if a marker is used, which one it shall use.
   * \param nameIn
   * \param IDIn
   * \param colorIn
   * \param toPaint
   * \param markerIn
   */
  paint_parameter(const string& nameIn = string(), int IDIn = 0, int colorIn = 1,
                  bool toPaint = false, int markerIn = 20);

  /*!
   * \brief Destructor
   */
  ~paint_parameter();

  /*!
   * \brief paint_parameter
   *  Copy constructor.
   * \param p
   */
  paint_parameter(const paint_parameter& p);

  /*!
   * \brief operator =
   *  Copy operator.
   * \param p
   * \return
   */
  paint_parameter& operator=(const paint_parameter& p);

  /*!
   * \brief getID
   *  Return the objects ID.
   * \return
   */
  int getID() const;

  /*!
   * \brief getMarker
   *  Returns the objects marker (if a marker is drawn). See the root- markers for detail.
   * \return
   */
  int getMarker() const;

  /*!
   * \brief getColor
   *  Returns the color of the object. See the root- colors for detail.
   * \return
   */
  int getColor() const;

  /*!
   * \brief isToPaint
   *  Returns true, if the object is drawn.
   * \return
   */
  bool isToPaint() const;

  /*!
   * \brief setID
   *  Sets the objects ID.
   * \param IDIn
   */
  void setID(int IDIn);

  /*!
   * \brief setColor
   *  Sets the color for the object to draw. See the root- colors for detail.
   * \param colorIn
   */
  void setColor(int colorIn);

  /*!
   * \brief setMarker
   *  Set the marker property. See the root-markers for detail.
   * \param markerIn
   */
  void setMarker(int markerIn);

  /*!
   * \brief setToPaint
   *  Set the paint switch to true.
   * \param to
   */
  void setToPaint(bool to = true);
};

/*!
 * \brief operator <<
 * \param o
 * \param p
 * \return
 */
ostream& operator<<(ostream& o, const paint_parameter& p);

/*!
 * \brief operator >>
 * \param i
 * \param p
 * \return
 */
istream& operator>>(istream& i, paint_parameter& p);

#endif
