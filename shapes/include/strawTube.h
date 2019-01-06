#include "cylinder.h"
class shape_parameter;
using namespace std;

/*!
 * \brief The strawTube class
 *  This class implements a straw-tube as it is used in the COSY-TOF- and PANDA-experiment. It is
 * derived of the shape cylinder giving it the properties of the base class. Additionally it has an
 * iso-chrone-radius. This is the distance a particle passed the counter's central wire.
 */
class strawTube : public cylinder
{
private:
  vector3D stackingDirection;
  vector3D shift;
  float isoChrone;
  int halvedAt;
  int nHalvedElements;

public:
  /*!
   * \brief strawTube
   *  Default constructor.
   */
  strawTube();

  /*!
   * \brief strawTube
   *  Constructor. Creates a straw tube. centerIn is the center of the tube, line the direction from
   * the center to one end of the tube, rad is the radius of the tube. It has an additional
   * radius-variable the iso-chrone-radius. To stack the tubes a stack-direction is necessary and,
   * if they are stacked in more than one layer, the shift from one layer to the next has to be
   * given. hav is the number of elements halved due to beam-tube and halvAt is the element-number
   * of the first halved element.
   * \param centerIn
   * \param lineDirection
   * \param rad
   * \param stackDirection
   * \param shiftDirection
   * \param isoChroneR
   * \param halv
   * \param halvAt
   */
  strawTube(point3D centerIn, vector3D lineDirection, float rad, vector3D stackDirection,
            vector3D shiftDirection, float isoChroneR, int halv, int halvAt);

  /*!
   * \brief strawTube
   *  Constructor. Creates a straw-tube with the properties from the description.
   * \param description
   */
  strawTube(const shape_parameter& description);

  /*!
   * \brief strawTube
   *  Copy constructor.
   * \param tube
   */
  strawTube(const strawTube& tube);

  /*!
   * \brief ~strawTube
   */
  virtual ~strawTube();

  /*!
   * \brief getIsoChroneRadius
   *  Returns the Iso-Chrone-Radius.
   * \return
   */
  virtual float getIsoChroneRadius() const;

  /*!
   * \brief getStackingDirection
   *  Returns the stacking direction of the strawTube-stack.
   * \return
   */
  virtual vector3D getStackingDirection() const;

  /*!
   * \brief getShift
   *  Returns the shift-vector from the first layer to the second one.
   * \return
   */
  virtual vector3D getShift() const;

  /*!
   * \brief getNumberOfHalvedElements
   *  Return the number of elements that are halved due to beam-tube.
   * \return
   */
  virtual int getNumberOfHalvedElements() const;

  /*!
   * \brief getHalvedAt
   *  Returns the element number of the element that is first halved due to beam-tube.
   * \return
   */
  virtual int getHalvedAt() const;

  /*!
   * \brief setIsoChroneRadius
   *  Sets the Iso-Chrone-radius.
   * \param value
   */
  virtual void setIsoChroneRadius(float value);

  /*!
   * \brief setStackingDirection
   *  Sets the direction in which the strawTubes shall be stacked.
   * \param value
   */
  virtual void setStackingDirection(vector3D value);

  /*!
   * \brief setShift
   *  Sets the shift-vector, from the front layer to the second layer.
   * \param value
   */
  virtual void setShift(vector3D value);

  /*!
   * \brief setNumberOfHalvedElements
   *  Sets the number of elements in the stack that are halved due to beam-tube.
   * \param value
   */
  virtual void setNumberOfHalvedElements(int value);

  /*!
   * \brief setHalvedAt
   *  Sets the number of the element that is first halved due to beam-tube.
   * \param value
   */
  virtual void setHalvedAt(int value);

  /*!
   * \brief getFlightPathInShape
   *  Returns the length of the path a line has in the shape.
   * \param line
   * \return
   */
  virtual float getFlightPathInShape(const sLine3D& line);

  /*!
   * \brief getClone
   *  Returns a clone of the shape. Take care that you free the memory after use.
   * \return
   */
  virtual volumeShape* getClone();

  /*!
   * \brief operator =
   * Copy operator. Works only if s is a strawTube or a cylinder.
   * \param s
   */
  virtual void operator=(const volumeShape& s);

  /*!
   * \brief getNext
   *  Assuming a certain stackType, the shape of the elements times farther in the stack is
   * generated and returned.
   * \param times
   * \param stackType
   * \return
   */
  virtual volumeShape* getNext(int times, int stackType);

  /*!
   * \brief getEnvelope
   *  Returns the enveloping shape of the stack of times strawTubes, stacked in type stackType.
   * \param times
   * \param stackType
   * \return
   */
  virtual volumeShape* getEnvelope(int times, int stackType);

  /*!
   * \brief suspect
   *  For a stack of strawTubes with a certain stackType, the hit of the line with the overall shape
   * of the stack is calculated computing the element number of the element that was most likely
   * hit.
   * \param line
   * \param stackType
   * \return
   */
  virtual int suspect(const sLine3D& line, int stackType);

  /*!
   * \brief Draw
   *  Projects and draws the quadrangle using the root-Draw functions to a root-TCanvas. The
   * projection plane is plane, the eye-point is eye. You can provide line color, fill color and
   * fill style. Supplying a suitable pointer for the bounding-box returns the size of the shape on
   * the canvas.
   * \param eye
   * \param plane
   * \param boundingBox
   * \param lColor
   * \param fColor
   * \param fStyle
   */
  virtual void Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                    int fColor = 8, int fStyle = 1001) const;

  /*!
   * \brief Draw
   *  Projects and draws the quadrangle using the root-Draw functions to a root-TCanvas. The
   * projection plane is plane, the eye-point is eye. You can provide line color, fill color and
   * fill style. Supplying a suitable pointer for the bounding-box returns the size of the shape on
   * the canvas. Ident can hold a pointer to the TPolyLine that was drawn to canvas.
   * \param eye
   * \param plane
   * \param boundingBox
   * \param ident
   * \param lColor
   * \param fColor
   * \param fStyle
   */
  virtual void Draw(const point3D& eye = point3D(0, 0, 0),
                    const plane3D& plane = plane3D(point3D(0, 0, 1), vector3D(0, 0, 1)),
                    vector4D* boundingBox = NULL, TObject** ident = NULL, int lColor = 1,
                    int fColor = 8, int fStyle = 1001) const;

  /*!
   * \brief description
   *  Returns a description of this strawTube.
   * \return
   */
  virtual shape_parameter description() const;

  /*!
   * \brief getDescription
   *  Static method. Returns a default description of a strawTube with all necessary variables
   * declared.
   * \return
   */
  static shape_parameter getDescription();
};
