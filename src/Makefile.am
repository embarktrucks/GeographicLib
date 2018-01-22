#
# Makefile.am
#
# Copyright (C) 2009, Francesco P. Lovergine <frankie@debian.org>

AM_CPPFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include -Wall -Wextra

lib_LTLIBRARIES = libGeographic.la

libGeographic_la_LDFLAGS = \
		-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)
libGeographic_la_SOURCES = Accumulator.cpp \
		AlbersEqualArea.cpp \
		AzimuthalEquidistant.cpp \
		CassiniSoldner.cpp \
		CircularEngine.cpp \
		DMS.cpp \
		Ellipsoid.cpp \
		EllipticFunction.cpp \
		GARS.cpp \
		GeoCoords.cpp \
		Geocentric.cpp \
		Geodesic.cpp \
		GeodesicExact.cpp \
		GeodesicExactC4.cpp \
		GeodesicLine.cpp \
		GeodesicLineExact.cpp \
		Geohash.cpp \
		Geoid.cpp \
		Georef.cpp \
		Gnomonic.cpp \
		GravityCircle.cpp \
		GravityModel.cpp \
		LambertConformalConic.cpp \
		LocalCartesian.cpp \
		MGRS.cpp \
		MagneticCircle.cpp \
		MagneticModel.cpp \
		Math.cpp \
		NormalGravity.cpp \
		OSGB.cpp \
		PolarStereographic.cpp \
		PolygonArea.cpp \
		Rhumb.cpp \
		SphericalEngine.cpp \
		TransverseMercator.cpp \
		TransverseMercatorExact.cpp \
		UTMUPS.cpp \
		Utility.cpp \
		../include/geographic_lib/Accumulator.hpp \
		../include/geographic_lib/AlbersEqualArea.hpp \
		../include/geographic_lib/AzimuthalEquidistant.hpp \
		../include/geographic_lib/CassiniSoldner.hpp \
		../include/geographic_lib/CircularEngine.hpp \
		../include/geographic_lib/Constants.hpp \
		../include/geographic_lib/DMS.hpp \
		../include/geographic_lib/Ellipsoid.hpp \
		../include/geographic_lib/EllipticFunction.hpp \
		../include/geographic_lib/GARS.hpp \
		../include/geographic_lib/GeoCoords.hpp \
		../include/geographic_lib/Geocentric.hpp \
		../include/geographic_lib/Geodesic.hpp \
		../include/geographic_lib/GeodesicExact.hpp \
		../include/geographic_lib/GeodesicLine.hpp \
		../include/geographic_lib/GeodesicLineExact.hpp \
		../include/geographic_lib/Geohash.hpp \
		../include/geographic_lib/Geoid.hpp \
		../include/geographic_lib/Georef.hpp \
		../include/geographic_lib/Gnomonic.hpp \
		../include/geographic_lib/GravityCircle.hpp \
		../include/geographic_lib/GravityModel.hpp \
		../include/geographic_lib/LambertConformalConic.hpp \
		../include/geographic_lib/LocalCartesian.hpp \
		../include/geographic_lib/MGRS.hpp \
		../include/geographic_lib/MagneticCircle.hpp \
		../include/geographic_lib/MagneticModel.hpp \
		../include/geographic_lib/Math.hpp \
		../include/geographic_lib/NearestNeighbor.hpp \
		../include/geographic_lib/NormalGravity.hpp \
		../include/geographic_lib/OSGB.hpp \
		../include/geographic_lib/PolarStereographic.hpp \
		../include/geographic_lib/PolygonArea.hpp \
		../include/geographic_lib/Rhumb.hpp \
		../include/geographic_lib/SphericalEngine.hpp \
		../include/geographic_lib/SphericalHarmonic.hpp \
		../include/geographic_lib/SphericalHarmonic1.hpp \
		../include/geographic_lib/SphericalHarmonic2.hpp \
		../include/geographic_lib/TransverseMercator.hpp \
		../include/geographic_lib/TransverseMercatorExact.hpp \
		../include/geographic_lib/UTMUPS.hpp \
		../include/geographic_lib/Utility.hpp \
		../include/geographic_lib/Config.h

../include/geographic_lib/Config.h: ../include/geographic_lib/Config-ac.h
	( egrep '\bVERSION\b|\bGEOGRAPHICLIB_|\bHAVE_LONG_DOUBLE\b' $< | \
	  sed -e 's/ VERSION / GEOGRAPHICLIB_VERSION_STRING /' \
	      -e 's/ HAVE_LONG_DOUBLE / GEOGRAPHICLIB_HAVE_LONG_DOUBLE /'; \
	  grep WORDS_BIGENDIAN $< | tail -1 | \
	  sed -e 's/ WORDS_BIGENDIAN / GEOGRAPHICLIB_WORDS_BIGENDIAN /' ) > $@

$(libGeographic_la_OBJECTS): ../include/geographic_lib/Config.h

geographiclib_data=$(datadir)/geographic_lib

DEFS=-DGEOGRAPHICLIB_DATA=\"$(geographiclib_data)\" @DEFS@

EXTRA_DIST = Makefile.mk CMakeLists.txt
