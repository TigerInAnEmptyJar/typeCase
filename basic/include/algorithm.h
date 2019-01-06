#ifndef ALGORITHM
#define ALGORITHM
#include <QtCore/QObject>
#include <string>
#include <vector>
class algorithm_parameter;
class TH1;
class TTree;
using namespace std;

/*!
 * \brief The AAlgorithm class
 * Basic algorithm class. Any derived classes of AAlgorithm can be inserted into the
 * typeCase-analyser.
 */
class AAlgorithm : public QObject
{
private:
  string name; //!
public:
  /*!
   * \brief AAlgorithm
   * Constructor. Sets the name of the algorithm.
   * \param n Name of the algorithm.
   */
  AAlgorithm(string n);

  /*!
   * \brief AAlgorithm
   * Copy-constructor.
   * \param a algorithm to copy.
   */
  AAlgorithm(const AAlgorithm& a);

  /*!
   * \brief Destructor
   */
  ~AAlgorithm();

  /*!
   * \brief getName
   * Returns the name of the algorithm.
   * \return
   */
  string getName() const;

  /*!
   * \brief process
   * Base functionality is defined here. In the analysis class, this method will be called
   * once for every event, so plug in the functionality you want to provide with your
   * algorithm here.
   * \param ptr ignored.
   * \return nullptr
   */
  virtual void* process(void* ptr);

  /*!
   * \brief histogramNames
   * Returns a vector with the names of the histograms defined for the algorithm.
   * \return list of histogram names available for this algorithm.
   */
  virtual vector<string> histogramNames();

  /*!
   * \brief histogram
   * Returns a pointer to the histogram defined for this algorithm with name histoName.
   * If there is no such histogram, NULL is returned. For the AAlgorithm class, NULL
   * is always returned, it may be otherwise for derived classes.
   * \param histoName name of the histogram.
   * \return nullptr
   */
  virtual TH1* histogram(string histoName);

  /*!
   * \brief treeNames
   * Returns a vector with the names of the trees defined for the algorithm.
   * \return
   */
  virtual vector<string> treeNames();

  /*!
   * \brief tree
   * Returns the pointer to a root-tree of name treename. If no such tree is found,
   * NULL is returned. For AAlgorithm, always NULL is returned, it may be otherwise
   * in derived classes.
   * \param treename
   * \return
   */
  virtual TTree* tree(string treename);

  /*!
   * \brief getDescription
   * Returns an algorithm_parameter containing a description and the necessary parameters
   * for the algorithm to run. The content of these parameters is not necessarily
   * sensible.
   * \return
   */
  static algorithm_parameter getDescription();
};

/*!
 * \brief toString
 * \param str
 * \return
 */
string toString(const QString& str);
#endif
