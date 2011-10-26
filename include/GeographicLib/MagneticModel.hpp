/**
 * \file MagneticModel.hpp
 * \brief Header for GeographicLib::MagneticModel class
 *
 * Copyright (c) Charles Karney (2011) <charles@karney.com> and licensed under
 * the MIT/X11 License.  For more information, see
 * http://geographiclib.sourceforge.net/
 **********************************************************************/

#if !defined(GEOGRAPHICLIB_MAGNETICMODEL_HPP)
#define GEOGRAPHICLIB_MAGNETICMODEL_HPP "$Id$"

#include <string>
#include <sstream>
#include <vector>
#include <GeographicLib/Constants.hpp>
#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/SphericalHarmonic.hpp>
#include <GeographicLib/SphericalHarmonic1.hpp>

#if defined(_MSC_VER)
// Squelch warnings about dll vs vector
#pragma warning (push)
#pragma warning (disable: 4251)
#endif

namespace GeographicLib {

  class MagneticCircle;

  /**
   * \brief Model of the earth's magnetic field
   *
   * Evaluate the earth's magnetic field according to a model.  At present only
   * internal magnetic fields are handling.  These are due to currents and
   * magnetized rocks in the earth; these vary slowly (over many years).
   * Excluded are the effects of currents in the ionosphere and magnetosphere
   * which have daily and annual variations.
   *
   * See
   * - http://geomag.org/models/index.html
   * - http://ngdc.noaa.gov/geomag/EMM/emm.shtml
   * - http://ngdc.noaa.gov/geomag/WMM/DoDWMM.shtml
   **********************************************************************/

  class GEOGRAPHIC_EXPORT MagneticModel {
  private:
    typedef Math::real real;
    std::string _name, _dir, _description, _date, _filename;
    real _t0, _tmin, _tmax, _a, _hmin, _hmax;
    int _N, _M, _N1, _M1;
    SphericalHarmonic::normalization _norm;
    Geocentric _earth;
    std::vector<real> _G, _H, _G1, _H1;
    void Field(real lat, real lon, real h, real t, bool diffp,
               real& Bx, real& By, real& Bz,
               real& Bxt, real& Byt, real& Bzt) const;
    SphericalHarmonic1 _harma;
    SphericalHarmonic _harmb;
    void ReadMetadata(const std::string& name);
    static bool ParseLine(const std::string& line,
                          std::string& key, std::string& val);
  public:

    /** \name Setting up the magnetic model
     **********************************************************************/
    ///@{
    /**
     * Construct a magnetic model.
     *
     * @param[in] name the name of the model.
     * @param[in] path (optional) directory for data file.
     * @param[in] earth (optional) Geocentric object for converting
     *   coordinates; default Geocentric::WGS84.
     *
     * A filename is formed by appending ".wwm" (World Magnetic Model) to
     * the name.  If \e path is specified (and is non-empty), then the file is
     * loaded from directory, \e path.  Otherwise the path is given by the
     * MAGNETIC_PATH environment variable.  If that is undefined, a
     * compile-time default path is used
     * (/usr/local/share/GeographicLib/magnetic on non-Windows systems and
     * C:/Documents and Settings/All Users/Application
     * Data/GeographicLib/geoids on Windows systems).  This may throw an
     * exception because the file does not exist, is unreadable, or is corrupt.
     *
     * This file contains the metadata which specifies the properties of the
     * model, it particular the degree and order of the spherical harmonic sums
     * used to approximate the magnetic potential and its time dependency.  The
     * coefficients for the spherical harmonic sums are obtained from a file
     * obtained by appending ".cof" to metadata file (so the filename ends in
     * ".wwm.cof").
     *
     * The model is not tied to a particular ellipsoidal model of the earth.
     * The final earth argument to the constructor specify an ellipsoid to
     * allow geodetic coordinates to the transformed into the spherical
     * coordinates used in the spherical harmonic sum.
     **********************************************************************/
    MagneticModel(const std::string& name, const std::string& path = "",
                  const Geocentric& earth = Geocentric::WGS84);
    ///@}

    /** \name Compute the magnetic fieldkgeoid heights
     **********************************************************************/
    ///@{
    /**
     * Evaluate the components of the magnetic field.
     *
     * @param[in] lat latitude of the point (degrees).
     * @param[in] lon longitude of the point (degrees).
     * @param[in] h the height of the point above the ellipsoid (meters).
     * @param[in] t the time (years).
     * @param[out] Bx the easterly component of the magnetic field (nanotesla).
     * @param[out] By the northerly component of the magnetic field (nanotesla).
     * @param[out] Bz the vertical (up) component of the magnetic field
     *   (nanotesla).
     **********************************************************************/
    void operator()(real lat, real lon, real h, real t,
                    real& Bx, real& By, real& Bz) const {
      real dummy;
      Field(lat, lon, h, t, false, Bx, By, Bz, dummy, dummy, dummy);
    }

    /**
     * Evaluate the components of the magnetic field and their time derivatives
     *
     * @param[in] lat latitude of the point (degrees).
     * @param[in] lon longitude of the point (degrees).
     * @param[in] h the height of the point above the ellipsoid (meters).
     * @param[in] t the time (years).
     * @param[out] Bx the easterly component of the magnetic field (nanotesla).
     * @param[out] By the northerly component of the magnetic field (nanotesla).
     * @param[out] Bz the vertical (up) component of the magnetic field
     *   (nanotesla).
     * @param[out] Bxt the rate of change of \e Bx (nT/yr).
     * @param[out] Byt the rate of change of \e By (nT/yr).
     * @param[out] Bzt the rate of change of \e Bz (nT/yr).
     **********************************************************************/
    void operator()(real lat, real lon, real h, real t,
                    real& Bx, real& By, real& Bz,
                    real& Bxt, real& Byt, real& Bzt) const {
      Field(lat, lon, h, t, true, Bx, By, Bz, Bxt, Byt, Bzt);
    }

    /**
     * Create a MagneticCircle object to allow the magnetic field at many
     * points with constant \e lat, \e h, and \e t and varying \e lon to be
     * computed efficienty.
     *
     * @param[in] lat latitude of the point (degrees).
     * @param[in] h the height of the point above the ellipsoid (meters).
     * @param[in] t the time (years).
     * @return a MagneticCircle object whose MagneticCircle::operator()(real
     *   lon) member function computes the field at a particular \e lon.
     *
     * If the field at several points on a circle of latitude need to be
     * calculated then instead of
     \code
  SphericalModel m(...);     // Create a magnetic model
  double lat = 33, lon0 = 44, dlon = 0.01, h = 1000, t = 2012;
  for (int i = 0; i <= 100; ++i) {
    real
      lon = lon0 + i * dlon, Bx, By, Bz;
    m(lat, lon, h, t, Bx, By, Bz);
    std::cout << lon << " " << Bx << " " << By << " " << Bz << "\n";
  }
     \endcode
     * use a MagneticCircle as in
     \code
  SphericalModel m(...);     // Create a magnetic model
  double lat = 33, lon0 = 44, dlon = 0.01, h = 1000, t = 2012;
  MagneticCircle c(m.Circle(lat, h, t)); // the MagneticCircle object
  for (int i = 0; i <= 100; ++i) {
    real
      lon = lon0 + i * dlon, Bx, By, Bz;
    c(lon, Bx, By, Bz);
    std::cout << lon << " " << Bx << " " << By << " " << Bz << "\n";
  }
     \endcode
     * For high-degree models, this will be substantially faster.
     **********************************************************************/
    MagneticCircle Circle(real lat, real h, real t) const;
    ///@}

    /** \name Inspector functions
     **********************************************************************/
    ///@{
    /**
     * @return geoid description, if available, in the data file; if
     *   absent, return "NONE".
     **********************************************************************/
    const std::string& Description() const throw() { return _description; }

    /**
     * @return date of the data file; if absent, return "UNKNOWN".
     **********************************************************************/
    const std::string& DateTime() const throw() { return _date; }

    /**
     * @return full file name used to load the geoid data.
     **********************************************************************/
    const std::string& GeoidFile() const throw() { return _filename; }

    /**
     * @return "name" used to load the geoid data (from the first argument of
     *   the constructor).
     **********************************************************************/
    const std::string& MagneticModelName() const throw() { return _name; }

    /**
     * @return directory used to load the geoid data.
     **********************************************************************/
    const std::string& MagneticModelDirectory() const throw() { return _dir; }

    /**
     * @return the minimum height (in meters) for this this MagneticModel
     *   should be used.
     *
     * Because the model will typically provide useful results
     * slightly outside the range of allowed heights, no check of \e t
     * argument is made by MagneticModel::operator()() or
     * MagnetgicModel::Circle.
     **********************************************************************/
    Math::real MinHeight() const throw() { return _hmin; }

    /**
     * @return the maximum height (in meters) for this this MagneticModel
     *   should be used.
     *
     * Because the model will typically provide useful results
     * slightly outside the range of allowed heights, no check of \e t
     * argument is made by MagneticModel::operator()() or
     * MagnetgicModel::Circle.
     **********************************************************************/
    Math::real MaxHeight() const throw() { return _hmax; }

    /**
     * @return the minimum time (in years) for this this MagneticModel
     *   should be used.
     *
     * Because the model will typically provide useful results
     * slightly outside the range of allowed times, no check of \e t
     * argument is made by MagneticModel::operator()() or
     * MagnetgicModel::Circle.
     **********************************************************************/
    Math::real MinTime() const throw() { return _tmin; }

    /**
     * @return the maximum time (in years) for this this MagneticModel
     *   should be used.
     *
     * Because the model will typically provide useful results
     * slightly outside the range of allowed times, no check of \e t
     * argument is made by MagneticModel::operator()() or
     * MagnetgicModel::Circle.
     **********************************************************************/
    Math::real MaxTime() const throw() { return _tmax; }

    /**
     * @return \e a the equatorial radius of the ellipsoid (meters).  This is
     *   the value of \e a inherited from the Geocentric object used in the
     *   constructor.
     **********************************************************************/
    Math::real MajorRadius() const throw() { return _earth.MajorRadius(); }

    /**
     * @return \e f the flattening of the ellipsoid.  This is the value
     *   inherited from the Geocentric object used in the constructor.
     **********************************************************************/
    Math::real Flattening() const throw() { return _earth.Flattening(); }
    ///@}

    /**
     * @return the default path for magnetic model data files.
     *
     * This is the value of the environment variable MAGNETIC_PATH, if set,
     * otherwise, it is a compile-time default.
     **********************************************************************/

    static std::string DefaultMagneticPath();

    /**
     * @return the default name for the magnetic model.
     *
     * This is the value of the environment variable MAGNETIC_NAME, if set,
     * otherwise, it is "wmm2010-12".  The MagneticModel class does not use
     * this function; it is just provided as a convenience for a calling
     * program when constructing a MagneticModel object.
     **********************************************************************/
    static std::string DefaultMagneticName();
  };

} // namespace GeographicLib

#if defined(_MSC_VER)
#pragma warning (pop)
#endif

#endif  // GEOGRAPHICLIB_MAGNETICMODEL_HPP