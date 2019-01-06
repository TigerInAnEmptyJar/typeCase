#ifndef _EVENTPAINTER_
#define _EVENTPAINTER_
#include "container.h"
#include "shapes.h"
#include <QtCore/QObject>
#include <TCanvas.h>
#include <TObject.h>
#include <vector>
using namespace std;

/*!
 * \brief The EventPainter class
 */
class EventPainter : public QObject
{
  Q_OBJECT
private:
  TEvent* Event;
  TPad* canvas;
  point2D upperLeft;
  point2D lowerRight;
  vector<TObject*> drawings;
  vector<TBase*> drawnObjects;
  vector<int> detectors;
  vector<int> detectorColors;
  vector<int> pixels;
  vector<int> pixelColors;
  vector<int> pixelMarker;
  vector<int> trackColors;
  vector<int> particleColors;
  bool detSorted;
  void paintHits(/* float zoom */);
  void paintPixels(/* float zoom */);
  void paintTracks(/* float zoom */);
  bool trackLike;
  bool particleLike;

public:
  /*!
   * \brief EventPainter
   * Default constructor.
   */
  EventPainter();

  /*!
   * \brief EventPainter
   * Constructor. Sets the painters properties. e is a pointer to the event structure that shall be
   * painted, c a pointer to the canvas or pad where the event pattern should be drawn onto.
   * \param e
   * \param c
   */
  EventPainter(TEvent* e, TPad* c);

  /*!
   * \brief EventPainter
   * Constructor. Sets the painters properties. e is a pointer to the event structure that shall be
   * painted, c a pointer to the canvas or pad where the event pattern should be drawn onto.
   * \param e
   * \param c
   * \param left
   * \param right
   * \param up
   * \param down
   */
  EventPainter(TEvent* e, TPad* c, float left, float right, float up, float down);

  /*!
   * \brief getDrawnObjects
   * Returns a vector of TObjects that have been drawn to the pad close to the position x-y. This
   * contains hits, pixels and tracks.
   * \param x
   * \param y
   * \return
   */
  vector<TBase*> getDrawnObjects(double x, double y);

  /*!
   * \brief Destructor
   */
  virtual ~EventPainter() = default;
public slots:

  /*!
   * \brief setTrackLike
   * Sets the color mode for the drawn detector elements. If sort is true (default), then the
   * detector elements will be drawn in the track's color instead of the detectors color.
   * \param like
   */
  void setTrackLike(bool like = true);

  /*!
   * \brief setEvent
   * Set the event pointer.
   * \param e
   */
  void setEvent(TEvent* e);

  /*!
   * \brief setCanvas
   * Set the canvas or pad the event should be drawn to.
   * \param c
   */
  void setCanvas(TPad* c);

  /*!
   * \brief setSize
   * Set the upper left and lower right corners of the canvas/pad in pad coordinates. Obsolete.
   * \param left
   * \param right
   * \param up
   * \param down
   */
  void setSize(float left, float right, float up, float down);

  /*!
   * \brief setSize
   * Set the upper left and lower right corners of the canvas/pad in pad coordinates. Obsolete.
   * \param upperLeftIn
   * \param lowerRightIn
   */
  void setSize(point2D upperLeftIn, point2D lowerRightIn);

  /*!
   * \brief rePaint
   * Paints the event e with this zoom to the current canvas/pad. First hits, then pixels and tracks
   * are drawn. In the end, there are event statistics added. The hits and pixels are drawn using
   * the Draw()-method from baseShape. The tracks are drawn using markers.
   * \param e
   */
  void rePaint(TEvent* e);

  /*!
   * \brief rePaint
   * Paints the event with this zoom to the current canvas. First hits, then pixels and tracks are
   * drawn. In the end, there are event statistics added. The hits and pixels are drawn using the
   * Draw()-method from baseShape. The tracks are drawn using markers.
   */
  void rePaint();

  /*!
   * \brief addDetector
   * Adds a detector with ID detNum and color to the list of detectors.
   * \param detNum
   * \param color
   */
  void addDetector(int detNum, int color = 1);

  /*!
   * \brief addPixel
   * Adds a pixel with ID pixNum, color and marker-type to the list of pixels.
   * \param pixNum
   * \param color
   * \param marker
   */
  void addPixel(int pixNum, int color = 1, int marker = 1);

  /*!
   * \brief addTrackColor
   * Adds a track color.
   * \param color
   */
  void addTrackColor(int color);

  /*!
   * \brief addParticleColor
   * Add a particle-color. In particle-mode a particle-track with ID is drawn in color.
   * \param ID
   * \param color
   */
  void addParticleColor(int ID, int color);

  /*!
   * \brief clearDetectors
   * Sets the number of detectors to draw to zero.
   */
  void clearDetectors();

  /*!
   * \brief clearPixels
   * Sets the number pf pixels to draw to zero.
   */
  void clearPixels();

  /*!
   * \brief clearTrackColor
   * Sets the number of tracks to draw to zero.
   */
  void clearTrackColor();

  /*!
   * \brief clearParticleColor
   * Remove the detector at index detNum from the list of drawn detectors.
   */
  void clearParticleColor();

  /*!
   * \brief removeDetector
   * Remove the detector at index detNum from the list of drawn detectors.
   * \param detNum
   */
  void removeDetector(int detNum);

  /*!
   * \brief removePixel
   * Remove the pixel from index pixNum from the list of drawn pixels.
   * \param pixNum
   */
  void removePixel(int pixNum);

  /*!
   * \brief setDetector
   * Set the color for the detNumth detector.
   * \param detNum
   * \param color
   */
  void setDetector(int detNum, int color);

  /*!
   * \brief setPixel
   * Sets the color for the pixNumth pixel.
   * \param pixNum
   * \param color
   */
  void setPixel(int pixNum, int color);

  /*!
   * \brief setPixel
   * Set the color and marker for the pixNumth pixel.
   * \param pixNum
   * \param color
   * \param marker
   */
  void setPixel(int pixNum, int color, int marker);

  /*!
   * \brief setTrackSorted
   * Sets the color mode for the drawn detector elements. If sort is true (default), then the
   * detector elements will be drawn in the track's color instead of the detectors color.
   * \param sort
   */
  void setTrackSorted(bool sort = true);

  /*!
   * \brief setPaintMode
   * Set the projection paint mode to parallel, central or fisheye.
   * \param mode
   */
  void setPaintMode(int mode);

  /*!
   * \brief setParticleMode
   * Sets the particle mode. Particles of a certain ID are drawn in a specific color.
   * \param value
   */
  void setParticleMode(bool value = true);
};

#endif
