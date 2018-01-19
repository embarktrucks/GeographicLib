/**
 * \file Accumulator.cpp
 * \brief Implementation for geographic_lib::Accumulator class
 *
 * Copyright (c) Charles Karney (2013) <charles@karney.com> and licensed under
 * the MIT/X11 License.  For more information, see
 * https://geographiclib.sourceforge.io/
 **********************************************************************/

#include <geographic_lib/Accumulator.hpp>

namespace geographic_lib {

  /// \cond SKIP

  // Need to instantiate Accumulator to get the code into the shared library
  // (without this, NETGeographic complains about not finding the == and !=
  // operators).
  template class GEOGRAPHICLIB_EXPORT Accumulator<Math::real>;

  /// \endcond

} // namespace geographic_lib
