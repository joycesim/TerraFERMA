
#ifndef __DETECTORS_FILE_H
#define __DETECTORS_FILE_H

#include <cstdio>
#include <fstream>
#include <string>
#include <dolfin.h>
#include "DiagnosticsFile.h"

namespace buckettools
{

  //*****************************************************************|************************************************************//
  // predeclarations: a circular dependency between the DetectorsFile class and the Bucket class requires a lot of predeclarations.
  //*****************************************************************|************************************************************//
  class Bucket;
  class SystemBucket;
  typedef boost::shared_ptr< SystemBucket > SystemBucket_ptr;
  class FunctionBucket;
  typedef boost::shared_ptr< FunctionBucket > FunctionBucket_ptr;
  typedef std::map< std::string, FunctionBucket_ptr >::const_iterator FunctionBucket_const_it;
  class GenericDetectors;
  typedef boost::shared_ptr< GenericDetectors > GenericDetectors_ptr;
  typedef std::map< std::string, GenericDetectors_ptr >::const_iterator GenericDetectors_const_it;
  typedef boost::shared_ptr< dolfin::Mesh > Mesh_ptr;

  //*****************************************************************|************************************************************//
  // DetectorsFile class:
  //
  // A derived class from the base statfile class intended for the output of detectors data to file every dump period.
  //*****************************************************************|************************************************************//
  class DetectorsFile : public DiagnosticsFile
  {

  //*****************************************************************|***********************************************************//
  // Publicly available functions
  //*****************************************************************|***********************************************************//

  public:
    
    //***************************************************************|***********************************************************//
    // Constructors and destructors
    //***************************************************************|***********************************************************//
    
    DetectorsFile(const std::string name);                           // specific constructor
    
    ~DetectorsFile();                                                // default destructor
    
    //***************************************************************|***********************************************************//
    // Header writing functions
    //***************************************************************|***********************************************************//

    void write_header(const Bucket &bucket);
    
    //***************************************************************|***********************************************************//
    // Data writing functions
    //***************************************************************|***********************************************************//

    void write_data();
    
  //*****************************************************************|***********************************************************//
  // Private functions
  //*****************************************************************|***********************************************************//

  private:
    
    //***************************************************************|***********************************************************//
    // Header writing functions (continued)
    //***************************************************************|***********************************************************//

    void header_bucket_(uint &column);
    
    void header_detector_(GenericDetectors_const_it d_begin,
                          GenericDetectors_const_it d_end,
                          uint &column);
    
    void header_func_(FunctionBucket_const_it f_begin,
                      FunctionBucket_const_it f_end,
                      GenericDetectors_const_it d_begin,
                      GenericDetectors_const_it d_end,
                      uint &column);
    
    //***************************************************************|***********************************************************//
    // Data writing functions (continued)
    //***************************************************************|***********************************************************//

    void data_bucket_();
    
    void data_detector_(GenericDetectors_const_it d_begin,
                        GenericDetectors_const_it d_end);
    
    void data_func_(FunctionBucket_const_it f_begin,
                    FunctionBucket_const_it f_end,
                    GenericDetectors_const_it d_begin,
                    GenericDetectors_const_it d_end,
                    Mesh_ptr mesh);
    

  };
  
  typedef boost::shared_ptr< DetectorsFile > DetectorsFile_ptr;          // define a boost shared ptr type for the class

}
#endif
