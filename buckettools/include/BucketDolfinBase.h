
#ifndef __BUCKETDOLFIN_BASE_H
#define __BUCKETDOLFIN_BASE_H

#include <dolfin.h>

namespace buckettools
{

  //*****************************************************************|************************************************************//
  // A collection of classes and subroutines used in the bucket by dolfin
  //*****************************************************************|************************************************************//

  //*****************************************************************|************************************************************//
  // Side class:
  //
  // An overloaded dolfin subdomain class to describe the side of an internally generated dolfin mesh 
  //*****************************************************************|************************************************************//
  class Side : public dolfin::SubDomain
  {

  //*****************************************************************|***********************************************************//
  // Publicly available functions
  //*****************************************************************|***********************************************************//

  public:                                                            // accesible to everyone

    //***************************************************************|***********************************************************//
    // Constructors and destructors
    //***************************************************************|***********************************************************//

    Side(const uint &component, const double &side);                 // optional constructor

    ~Side();                                                         // default destructor

    //***************************************************************|***********************************************************//
    // Overloaded base class functions
    //***************************************************************|***********************************************************//
    
    bool inside(const dolfin::Array<double>& x, bool on_boundary)    // return a boolean, true if point x is inside the subdomain, false otherwise
                                                              const; 

  //*****************************************************************|***********************************************************//
  // Private functions
  //*****************************************************************|***********************************************************//

  private:                                                           // only accessible in this class

    //***************************************************************|***********************************************************//
    // Base data
    //***************************************************************|***********************************************************//

    uint component_;                                                 // component of n-dimensional space

    double side_;                                                    // location of the side in that n-dimensional space

  };

  
  //*****************************************************************|************************************************************//
  // Assembler class:
  //
  // A dummy class to hold some auxilliary assembly subroutines
  //*****************************************************************|************************************************************//
  class Assembler
  {

  //*****************************************************************|***********************************************************//
  // Publicly available functions
  //*****************************************************************|***********************************************************//

  public:                                                            // accesible to everyone

    static void add_zeros_diagonal(dolfin::GenericTensor& A);               // add zeros to the diagonal of a tensor

  };
}

#endif

