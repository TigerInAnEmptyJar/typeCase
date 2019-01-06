#ifndef TBASE
#define TBASE
#include <string>
using namespace std;

/*!
 * \brief The TBase class
 * TBase was introduced to get rid of the root overhead but to maintain some basic
 * features, like distinguishability of the different types while being able to store
 * them in the same array.
 */
class TBase
{
protected:
  string fname;

public:
  /*!
   * \brief TBase
   * Constructor. Sets the name of the type.
   * \param nme
   */
  TBase(const string& nme);

  /*!
   * \brief Destructor
   */
  ~TBase();

  /*!
   * \brief name
   * \return Returns the name of the type.
   */
  string name() const;
};
#endif
