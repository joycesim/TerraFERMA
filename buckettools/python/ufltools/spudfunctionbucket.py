import sys
import libspud
import ufltools.functionbucket

class SpudFunctionBucket(ufltools.functionbucket.FunctionBucket):
  """A class that stores all the information necessary to write the ufl for a function (field or coefficient) 
     plus all the information necessary to populate that class using libspud.
     Note that the class has limited ufl production because much of this is system dependent."""

  def fill(self, optionpath, system, index):
    """Fill a function class with data describing that function using libspud, the given optionpath and the system its based on."""
    self.name       = libspud.get_option(optionpath+"/name")
    self.symbol     = libspud.get_option(optionpath+"/ufl_symbol")
    self.system     = system
    self.index      = index
    self.type       = libspud.get_option(optionpath+"/type/name")

    self.rank   = libspud.get_option(optionpath+"/type/rank/name")
    self.family   = None
    self.degree = None
    self.functional = None
    if self.type != "Constant":
      self.family = libspud.get_option(optionpath+"/type/rank/element/family")
      self.degree = libspud.get_option(optionpath+"/type/rank/element/degree")
    else:
      if libspud.have_option(optionpath+"/type/rank/value/functional"):
        functional_optionpath = optionpath+"/type/rank/value/functional"
        functional = ufltools.spud.SpudFunctionalBucket()
        functional.fill(functional_optionpath, self)
        self.functional = functional
    
    self.size     = None
    self.shape    = None
    self.symmetry = None
    if self.rank == "Vector":
      if libspud.have_option(optionpath+"/type/rank/element/size"):
        self.size = libspud.get_option(optionpath+"/type/rank/element/size")
    elif self.rank == "Tensor":
      if libspud.have_option(optionpath+"/type/rank/element/shape"):
        self.shape = libspud.get_option(optionpath+"/type/rank/element/shape")
      if libspud.have_option(optionpath+"/type/rank/element/symmetry"):
        self.symmetry = True

    self.functionals = []
    
    for k in range(libspud.option_count(optionpath+"/diagnostics/include_in_statistics/functional")):
      functional_optionpath = optionpath+"/diagnostics/include_in_statistics/functional["+`k`+"]"
      functional = ufltools.spud.SpudFunctionalBucket()
      # get all the information about this functional from the options dictionary
      functional.fill(functional_optionpath, self)
      # let the field know about this functional
      self.functionals.append(functional)
      # done with this functional
      del functional

