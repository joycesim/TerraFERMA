
#include <dolfin.h>
#include "petscsnes.h"
#include "BucketPETScBase.h"

using namespace buckettools;

//*******************************************************************|************************************************************//
// define the petsc snes callback function that assembles the residual function
//*******************************************************************|************************************************************//
PetscErrorCode buckettools::FormFunction(SNES snes, Vec x, Vec f, 
                                                          void* ctx)
{
  SNESCtx *snesctx = (SNESCtx *)ctx;                                 // cast the snes context
  Function_ptr iteratedfunction = (*snesctx).iteratedfunction;       // collect the iterated system bucket function
  std::vector<BoundaryCondition_ptr>& bcs = (*snesctx).bcs;          // get the vector of bcs
  Vec_ptr px(&x, dolfin::NoDeleter());                               // convert the iterated snes vector
  Vec_ptr pf(&f, dolfin::NoDeleter());                               // convert the rhs snes vector
  dolfin::PETScVector rhs(pf), iteratedvec(px);
  bool reset_tensor = false;                                         // never reset the tensor

  dolfin::log(dolfin::INFO, "In FormFunction");

  (*iteratedfunction).vector() = iteratedvec;                        // update the iterated system bucket function

  (*(*snesctx).bucket).update_nonlinear();                           // update nonlinear coefficients

  dolfin::assemble(rhs, (*(*snesctx).linear), reset_tensor);         // assemble the rhs from the context linear form
  for(uint i = 0; i < bcs.size(); ++i)                               // loop over the bcs
  {
    (*bcs[i]).apply(rhs, iteratedvec);                               // FIXME: will break symmetry?
  }
  
  PetscFunctionReturn(0);
}

//*******************************************************************|************************************************************//
// define the petsc snes callback function that assembles the jacobian function
//*******************************************************************|************************************************************//
PetscErrorCode buckettools::FormJacobian(SNES snes, Vec x, Mat *A, 
                                         Mat *B, MatStructure* flag, 
                                         void* ctx)
{
  SNESCtx *snesctx = (SNESCtx *)ctx;                                 // cast the snes context
  Function_ptr iteratedfunction = (*snesctx).iteratedfunction;       // collect the iterated system bucket function
  std::vector<BoundaryCondition_ptr>& bcs = (*snesctx).bcs;          // get the vector of bcs
  Vec_ptr px(&x, dolfin::NoDeleter());                               // convert the iterated snes vector
  dolfin::PETScVector iteratedvec(px);
  Mat_ptr pA(A, dolfin::NoDeleter());                                // convert the snes matrix
  Mat_ptr pB(B, dolfin::NoDeleter());                                // convert the snes matrix pc
  dolfin::PETScMatrix matrix(pA), matrixpc(pB);
  bool reset_tensor = false;                                         // never reset the tensor

  dolfin::log(dolfin::INFO, "In FormJacobian");

  (*iteratedfunction).vector() = iteratedvec;                        // update the iterated system bucket function

  (*(*snesctx).bucket).update_nonlinear();                           // update nonlinear coefficients

  dolfin::assemble(matrix, (*(*snesctx).bilinear), reset_tensor);    // assemble the matrix from the context bilinear form
  for(uint i = 0; i < bcs.size(); ++i)                               // loop over the bcs
  {
    (*bcs[i]).apply(matrix);                                         // FIXME: will break symmetry?
  }
  if ((*snesctx).ident_zeros)
  {
    matrix.ident_zeros();
  }

  if ((*snesctx).bilinearpc)                                         // do we have a different bilinear pc form associated?
  {
    dolfin::assemble(matrixpc, (*(*snesctx).bilinearpc),             // assemble the matrix pc from the context bilinear pc form
                                                      reset_tensor);
    for(uint i = 0; i < bcs.size(); ++i)                             // loop over the bcs
    {
      (*bcs[i]).apply(matrixpc);                                     // FIXME: will break symmetry
    }
    if ((*snesctx).ident_zeros_pc)
    {
      matrixpc.ident_zeros();
    }
  }

  *flag = SAME_NONZERO_PATTERN;                                      // both matrices are assumed to have the same sparsity

  PetscFunctionReturn(0);
}


