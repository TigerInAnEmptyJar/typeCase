#pragma once

#include "planeShape.h"
#include "shapeparameter.h"
#include "volumeShape.h"

#include <boost/signals2.hpp>
#include <boost/uuid/uuid.hpp>

#include <functional>
#include <map>
#include <memory>

enum class ShapeType
{
  PlanarShape,
  VolumeShape,
  Undefined
};

/*!
 * \brief A factory for shape classes.
 *
 * This class is the factory for all shapes. It collects factory functions for the installed
 * shapes. This way the factory only knows about the basic kinds of shapes: base_shape,
 * planeShape and volumeShape, but is unaware of the actual implementation of these.
 * If you want to publish a shape to the factory (and thus to typeCase) use the addShapeToFactory
 * method to add factory functors.
 * Intention is here to move the generation of series of shapes and envelopes from the shapes to
 * this factory.
 *
 * This class is non-copyable. If you want to have your own factory, create one; but keep in mind:
 * there is no factory tree implemented.
 */
class ShapeFactory
{
public:
  ShapeFactory() = default;
  ~ShapeFactory() = default;
  ShapeFactory(ShapeFactory const& other) = delete;
  ShapeFactory& operator=(ShapeFactory const& other) = delete;

  //! \name Use management
  //! \{
  /*!
   * \brief get a list of defined shapes
   * \return a list of all defined shapes
   */
  std::vector<shape_parameter> definedShapes() const;

  /*!
   * \brief get a shapeParameter of a defined shape
   * \param id the id of the shape to return the parameter
   * \return a shapeParameter for the specified id.
   */
  shape_parameter shapeParameter(boost::uuids::uuid id) const;

  /*!
   * \brief get the type of a defined shape
   * \param id the id of the shape to query
   * \return the ShapeType of the queried shape.
   */
  ShapeType shapeType(boost::uuids::uuid id) const;

  /*!
   * \brief is a shape defined by this factory
   * Determines whether the creation of shapes with the provided id is supported by the factory.
   * \param id the id of the shape to check.
   * \return true if the factory provides the creation of a shape of this id.
   */
  bool isShapeDefined(boost::uuids::uuid const& id) const;
  //! \}

  //! \name Generation
  //! \{
  /*!
   * \brief volumeShape generation
   * This method creates a volume shape. If the provided description isn't registered for volumes
   * but for planes or else, a nullptr is returned.
   * \param shape the description of the shape to generate.
   * \return a shared-pointer to the newly created volume.
   */
  std::shared_ptr<volumeShape> createVolume(shape_parameter const& shape) const;

  /*!
   * \brief series generation of volumes
   * This method creates a volume shape. If the provided description isn't registered for volumes
   * but for planes or else, a nullptr is returned.
   * If num is greater than 0 it will create the next (or num-th) shape in the series of shapes.
   * \param first_element the description of the first element in the series.
   * \param num the ordinal of the shape to create.
   * \return a shared-pointer to the newly created volume.
   */
  std::shared_ptr<volumeShape> createNext(shape_parameter const& first_element, size_t num) const;

  /*!
   * \brief envelope generation of volumes
   * This method creates a volume shape. If the provided description isn't registered for volumes
   * but for planes or else, a nullptr is returned.
   * If num is greater than 1 it will create an envelope of num of the provided shapes.
   * \param first_element the description of the first element in the series.
   * \param num the numer of shapes the envelope shall contain.
   * \return a shared-pointer to the newly created volume.
   */
  std::shared_ptr<volumeShape> createEnvelope(shape_parameter const& first_element,
                                              size_t num) const;

  /*!
   * \brief planeShape generation
   * This method creates a planar shape. If the provided description isn't registered for planes
   * but for volumes or else, a nullptr is returned.
   * \param shape the description of the shape to generate.
   * \return a shared-pointer to the newly created planar shape.
   */
  std::shared_ptr<planeShape> createPlane(shape_parameter const& shape) const;

  /*!
   * \brief any shape generation
   * This method generates a shape object. It doesn't distinguish between planes or volumes, so it
   * will always return a shape, if the type of shape had been registered.
   * \param shape the description of the shape to generate.
   * \return a shared-pointer to the newly created shape.
   */
  std::shared_ptr<base_shape> createShape(shape_parameter const& shape) const;
  //! \}

  //! \name Management
  //! \{
  /*!
   * This type defines the functor type to create a shape. The values for the shape are taken
   * from the one parameter.
   */
  using ShapeCreation_t = std::function<std::shared_ptr<base_shape>(shape_parameter const&)>;

  /*!
   * This defines the functor for series creation. An assumption taken here is that the provided
   * shape_parameter is the first shape of the series. The second parameter defines the number of
   * the shape in the series that is to be generated.
   * Note: the return value here is a shape_parameter, not a shape.
   */
  using SeriesCreation_t = std::function<shape_parameter(shape_parameter const&, size_t)>;

  /*!
   * This defines the functor for envelope creation. An assumption taken here is that the provided
   * shape_parameter is the first shape of the series. The second parameter defines the number of
   * shapes that are in the series. The generated object describes the envelope of the series of
   * this amount of shapes.
   * Note: the return value here is a shape_parameter, not a shape.
   * Note: the returned shape can be of a different type than the original shape.
   */
  using EnvelopeCreation_t = std::function<shape_parameter(shape_parameter const&, size_t)>;

  /*!
   * \brief add a shape to the factory
   * This method adds the generation of a shape to the factory.
   * \param shape the parameter of the shape to generate. This the only property of this parameter
   * that needs to have a valid value is the uuid.
   * \param shapeType the type of the shape (planar, volume, other).
   * \param creationFunction the functor to create the shape.
   * \param seriesCreation the functor for series creation.
   * \param envelopeCreation the functor for envelope creation.
   * \return true if registering the shape was successful.
   */
  bool addShapeToFactory(shape_parameter const& shape, ShapeType shapeType,
                         ShapeCreation_t creationFunction, SeriesCreation_t seriesCreation,
                         EnvelopeCreation_t envelopeCreation);

  /*!
   * \brief remove a shape from the factory
   * This method removes a geneation of a shape from the factory.
   * \param id the id of the shape to remove.
   * \return true if unregistering was successful.
   */
  bool removeShapeFromFactory(boost::uuids::uuid const& id);
  //! \}
  //!

  //! \name Signalling
  //! \{
  /*!
   * Functor type for notifications for added shapes signal. The parameter is the id of the added
   * shape.
   */
  using ShapeAddedSignal_t = std::function<void(boost::uuids::uuid)>;

  /*!
   * \brief subscribe to Shape Added signal
   * Use this method to subscribe to the shape-added signal.
   * \param subscriber the functor to be called in case a shape has been added.
   * \return the connection.
   */
  boost::signals2::connection subscribeShapeAdded(ShapeAddedSignal_t subscriber);

  /*!
   * Functor type for notifications for remove shapes signal. The parameter is the id of the
   * removed shape.
   */
  using ShapeRemovedSignal_t = std::function<void(boost::uuids::uuid)>;

  /*!
   * \brief subscribe to Shape Removed signal.
   * Use this method to subscribe to the shape-removed signal.
   * \param subscriber the functor to be called in case a shape has been removed.
   * \return the connection.
   */
  boost::signals2::connection subscribeShapeRemoved(ShapeRemovedSignal_t subscriber);
  //! \}

  /*!
   * \brief get the singleton instance
   * \return the one instance of shape-factory.
   */
  static ShapeFactory& getInstance();

private:
  struct Configuration
  {
    ShapeCreation_t creation;
    SeriesCreation_t series;
    EnvelopeCreation_t envelope;
    shape_parameter parameter;
    ShapeType shape_type;
  };
  std::map<boost::uuids::uuid, Configuration> installed_shapes;

  boost::signals2::signal<void(boost::uuids::uuid)> shapeAdded;
  boost::signals2::signal<void(boost::uuids::uuid)> shapeRemoved;
};

class FactoryShapeProvider
{
public:
  virtual ~FactoryShapeProvider() = default;
  virtual void addToFactory(ShapeFactory& factory) const = 0;
  virtual void removeFromFactory(ShapeFactory& factory) const = 0;

  static std::vector<std::shared_ptr<FactoryShapeProvider>> getProviders()
  {
    return installedProviders;
  }

protected:
  static std::vector<std::shared_ptr<FactoryShapeProvider>> installedProviders;
};
