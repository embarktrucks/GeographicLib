#
# Makefile.am
#
# Copyright (C) 2009, Francesco P. Lovergine <frankie@debian.org>

AM_CPPFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include \
           -I$(top_builddir)/man -I$(top_srcdir)/man -Wall -Wextra

LDADD = $(top_builddir)/src/libGeographic.la
DEPS = $(top_builddir)/src/libGeographic.la

bin_PROGRAMS = CartConvert \
	ConicProj \
	GeoConvert \
	GeodSolve \
	GeodesicProj \
	GeoidEval \
	Gravity \
	MagneticField \
	Planimeter \
	RhumbSolve \
	TransverseMercatorProj

CartConvert_SOURCES = CartConvert.cpp \
	../man/CartConvert.usage \
	../include/geographic_lib/Config.h \
	../include/geographic_lib/Constants.hpp \
	../include/geographic_lib/DMS.hpp \
	../include/geographic_lib/Geocentric.hpp \
	../include/geographic_lib/LocalCartesian.hpp \
	../include/geographic_lib/Math.hpp \
	../include/geographic_lib/Utility.hpp
ConicProj_SOURCES = ConicProj.cpp \
	../man/ConicProj.usage \
	../include/geographic_lib/Config.h \
	../include/geographic_lib/AlbersEqualArea.hpp \
	../include/geographic_lib/Constants.hpp \
	../include/geographic_lib/DMS.hpp \
	../include/geographic_lib/LambertConformalConic.hpp \
	../include/geographic_lib/Math.hpp \
	../include/geographic_lib/Utility.hpp
GeoConvert_SOURCES = GeoConvert.cpp \
	../man/GeoConvert.usage \
	../include/geographic_lib/Config.h \
	../include/geographic_lib/Constants.hpp \
	../include/geographic_lib/DMS.hpp \
	../include/geographic_lib/GeoCoords.hpp \
	../include/geographic_lib/Math.hpp \
	../include/geographic_lib/UTMUPS.hpp \
	../include/geographic_lib/Utility.hpp
GeodSolve_SOURCES = GeodSolve.cpp \
	../man/GeodSolve.usage \
	../include/geographic_lib/Config.h \
	../include/geographic_lib/Constants.hpp \
	../include/geographic_lib/DMS.hpp \
	../include/geographic_lib/Geodesic.hpp \
	../include/geographic_lib/GeodesicExact.hpp \
	../include/geographic_lib/GeodesicLine.hpp \
	../include/geographic_lib/GeodesicLineExact.hpp \
	../include/geographic_lib/Math.hpp \
	../include/geographic_lib/Utility.hpp
GeodesicProj_SOURCES = GeodesicProj.cpp \
	../man/GeodesicProj.usage \
	../include/geographic_lib/Config.h \
	../include/geographic_lib/AzimuthalEquidistant.hpp \
	../include/geographic_lib/CassiniSoldner.hpp \
	../include/geographic_lib/Constants.hpp \
	../include/geographic_lib/DMS.hpp \
	../include/geographic_lib/Geodesic.hpp \
	../include/geographic_lib/GeodesicLine.hpp \
	../include/geographic_lib/Gnomonic.hpp \
	../include/geographic_lib/Math.hpp \
	../include/geographic_lib/Utility.hpp
GeoidEval_SOURCES = GeoidEval.cpp \
	../man/GeoidEval.usage \
	../include/geographic_lib/Config.h \
	../include/geographic_lib/Constants.hpp \
	../include/geographic_lib/DMS.hpp \
	../include/geographic_lib/GeoCoords.hpp \
	../include/geographic_lib/Geoid.hpp \
	../include/geographic_lib/Math.hpp \
	../include/geographic_lib/UTMUPS.hpp \
	../include/geographic_lib/Utility.hpp
Gravity_SOURCES = Gravity.cpp \
	../man/Gravity.usage \
	../include/geographic_lib/Config.h \
	../include/geographic_lib/CircularEngine.hpp \
	../include/geographic_lib/Constants.hpp \
	../include/geographic_lib/DMS.hpp \
	../include/geographic_lib/Geocentric.hpp \
	../include/geographic_lib/GravityCircle.hpp \
	../include/geographic_lib/GravityModel.hpp \
	../include/geographic_lib/Math.hpp \
	../include/geographic_lib/NormalGravity.hpp \
	../include/geographic_lib/SphericalEngine.hpp \
	../include/geographic_lib/SphericalHarmonic.hpp \
	../include/geographic_lib/SphericalHarmonic1.hpp \
	../include/geographic_lib/Utility.hpp
MagneticField_SOURCES = MagneticField.cpp \
	../man/MagneticField.usage \
	../include/geographic_lib/Config.h \
	../include/geographic_lib/CircularEngine.hpp \
	../include/geographic_lib/Constants.hpp \
	../include/geographic_lib/DMS.hpp \
	../include/geographic_lib/Geocentric.hpp \
	../include/geographic_lib/MagneticCircle.hpp \
	../include/geographic_lib/MagneticModel.hpp \
	../include/geographic_lib/Math.hpp \
	../include/geographic_lib/SphericalEngine.hpp \
	../include/geographic_lib/SphericalHarmonic.hpp \
	../include/geographic_lib/Utility.hpp
Planimeter_SOURCES = Planimeter.cpp \
	../man/Planimeter.usage \
	../include/geographic_lib/Config.h \
	../include/geographic_lib/Accumulator.hpp \
	../include/geographic_lib/Constants.hpp \
	../include/geographic_lib/DMS.hpp \
	../include/geographic_lib/Ellipsoid.hpp \
	../include/geographic_lib/GeoCoords.hpp \
	../include/geographic_lib/Geodesic.hpp \
	../include/geographic_lib/Math.hpp \
	../include/geographic_lib/PolygonArea.hpp \
	../include/geographic_lib/UTMUPS.hpp \
	../include/geographic_lib/Utility.hpp
RhumbSolve_SOURCES = RhumbSolve.cpp \
	../man/RhumbSolve.usage \
	../include/geographic_lib/Config.h \
	../include/geographic_lib/Constants.hpp \
	../include/geographic_lib/DMS.hpp \
	../include/geographic_lib/Ellipsoid.hpp \
	../include/geographic_lib/Math.hpp \
	../include/geographic_lib/Utility.hpp
TransverseMercatorProj_SOURCES = TransverseMercatorProj.cpp \
	../man/TransverseMercatorProj.usage \
	../include/geographic_lib/Config.h \
	../include/geographic_lib/Constants.hpp \
	../include/geographic_lib/DMS.hpp \
	../include/geographic_lib/EllipticFunction.hpp \
	../include/geographic_lib/Math.hpp \
	../include/geographic_lib/TransverseMercator.hpp \
	../include/geographic_lib/TransverseMercatorExact.hpp \
	../include/geographic_lib/Utility.hpp

sbin_SCRIPTS = geographiclib-get-geoids \
	geographiclib-get-gravity \
	geographiclib-get-magnetic

geographiclib_data = $(datadir)/geographic_lib
geographiclib-get-geoids: geographiclib-get-geoids.sh
	sed -e "s%@GEOGRAPHICLIB_DATA@%$(geographiclib_data)%" $< > $@
	chmod +x $@
geographiclib-get-gravity: geographiclib-get-gravity.sh
	sed -e "s%@GEOGRAPHICLIB_DATA@%$(geographiclib_data)%" $< > $@
	chmod +x $@
geographiclib-get-magnetic: geographiclib-get-magnetic.sh
	sed -e "s%@GEOGRAPHICLIB_DATA@%$(geographiclib_data)%" $< > $@
	chmod +x $@

CLEANFILES = $(sbin_SCRIPTS)
EXTRA_DIST = Makefile.mk CMakeLists.txt tests.cmake \
	geographiclib-get-geoids.sh geographiclib-get-gravity.sh \
	geographiclib-get-magnetic.sh
