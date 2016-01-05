"""Define the :class:`~geographiclib.polygonarea.PolygonArea` class

The constructor initializes a empty polygon.

  * :meth:`~geographiclib.polygonarea.PolygonArea.Clear` reset the
    polygon

  * :meth:`~geographiclib.polygonarea.PolygonArea.AddPoint` add a vertex
    to the polygon

  * :meth:`~geographiclib.polygonarea.PolygonArea.AddEdge` add an edge
    to the polygon

  * :meth:`~geographiclib.polygonarea.PolygonArea.Compute` compute the
    properties of the polygon

  * :meth:`~geographiclib.polygonarea.PolygonArea.TestPoint` compute the
    properties of the polygon with a tentative additional vertex

  * :meth:`~geographiclib.polygonarea.PolygonArea.TestEdge` compute the
    properties of the polygon with a tentative additional edge

  * :meth:`~geographiclib.polygonarea.PolygonArea.CurrentPoint` return
    the current vertex

"""
# polygonarea.py
#
# This is a rather literal translation of the GeographicLib::PolygonArea class
# to python.  See the documentation for the C++ class for more information at
#
#    http://geographiclib.sourceforge.net/html/annotated.html
#
# The algorithms are derived in
#
#    Charles F. F. Karney,
#    Algorithms for geodesics, J. Geodesy 87, 43-55 (2013),
#    https://dx.doi.org/10.1007/s00190-012-0578-z
#    Addenda: http://geographiclib.sourceforge.net/geod-addenda.html
#
# Copyright (c) Charles Karney (2011-2016) <charles@karney.com> and licensed
# under the MIT/X11 License.  For more information, see
# http://geographiclib.sourceforge.net/
######################################################################

import math
from geographiclib.geomath import Math
from geographiclib.accumulator import Accumulator

class PolygonArea(object):
  """Area of a geodesic polygon"""

  def _transit(lon1, lon2):
    """Count crossings of prime meridian for AddPoint."""
    # Return 1 or -1 if crossing prime meridian in east or west direction.
    # Otherwise return zero.
    # Compute lon12 the same way as Geodesic::Inverse.
    lon1 = Math.AngNormalize(lon1)
    lon2 = Math.AngNormalize(lon2)
    lon12, _ = Math.AngDiff(lon1, lon2)
    cross = (1 if lon1 < 0 and lon2 >= 0 and lon12 > 0
             else (-1 if lon2 < 0 and lon1 >= 0 and lon12 < 0 else 0))
    return cross
  _transit = staticmethod(_transit)

  def _transitdirect(lon1, lon2):
    """Count crossings of prime meridian for AddEdge."""
    # We want to compute exactly
    #   int(floor(lon2 / 360)) - int(floor(lon1 / 360))
    # Since we only need the parity of the result we can use std::remquo but
    # this is buggy with g++ 4.8.3 and requires C++11.  So instead we do
    lon1 = math.fmod(lon1, 720.0); lon2 = math.fmod(lon2, 720.0)
    return ( (0 if ((lon2 >= 0 and lon2 < 360) or lon2 < -360) else 1) -
             (0 if ((lon1 >= 0 and lon1 < 360) or lon1 < -360) else 1) )
  _transitdirect = staticmethod(_transitdirect)

  def __init__(self, earth, polyline = False):
    """Construct a PolygonArea object

    :param earth: a :class:`~geographiclib.geodesic.Geodesic` object
    :param polyline: if true, treat object as a polyline instead of a polygon

    Initially the polygon has no vertices.
    """

    from geographiclib.geodesic import Geodesic
    self._earth = earth
    self._area0 = 4 * math.pi * earth._c2
    self._polyline = polyline
    self._mask = (Geodesic.LATITUDE | Geodesic.LONGITUDE |
                  Geodesic.DISTANCE |
                  (Geodesic.EMPTY if self._polyline else
                   Geodesic.AREA | Geodesic.LONG_UNROLL))
    if not self._polyline: self._areasum = Accumulator()
    self._perimetersum = Accumulator()
    self.Clear()

  def Clear(self):
    """Reset to empty polygon."""
    self._num = 0
    self._crossings = 0
    if not self._polyline: self._areasum.Set(0)
    self._perimetersum.Set(0)
    self._lat0 = self._lon0 = self._lat1 = self._lon1 = Math.nan

  def AddPoint(self, lat, lon):
    """Add the next vertex to the polygon

    :param lat: the latitude of the point in degrees
    :param lon: the longitude of the point in degrees

    This adds an edge from the current vertex to the new vertex.
    """

    if self._num == 0:
      self._lat0 = self._lat1 = lat
      self._lon0 = self._lon1 = lon
    else:
      _, s12, _, _, _, _, _, S12 = self._earth._GenInverse(
        self._lat1, self._lon1, lat, lon, self._mask)
      self._perimetersum.Add(s12)
      if not self._polyline:
        self._areasum.Add(S12)
        self._crossings += PolygonArea._transit(self._lon1, lon)
      self._lat1 = lat
      self._lon1 = lon
    self._num += 1

  def AddEdge(self, azi, s):
    """Add the next edge to the polygon

    :param azi: the azimuth at the current the point in degrees
    :param s: the length of the edge in meters

    This specifies the new vertex in terms of the edge from the current
    vertex.

    """

    if self._num != 0:
      _, lat, lon, _, _, _, _, _, S12 = self._earth._GenDirect(
        self._lat1, self._lon1, azi, False, s, self._mask)
      self._perimetersum.Add(s)
      if not self._polyline:
        self._areasum.Add(S12)
        self._crossings += PolygonArea._transitdirect(self._lon1, lon)
      self._lat1 = lat
      self._lon1 = lon
      self._num += 1

  # return number, perimeter, area
  def Compute(self, reverse = False, sign = True):
    """Compute the properties of the polygon

    :param reverse: if true then clockwise (instead of
      counter-clockwise) traversal counts as a positive area
    :param sign: if true then return a signed result for the area if the
      polygon is traversed in the "wrong" direction instead of returning
      the area for the rest of the earth
    :return: a tuple of number, perimeter (meters), area (meters^2)

    If the object is a polygon (and not a polygon), the perimeter
    includes the length of a final edge connecting the current point to
    the initial point.  If the object is a polyline, then area is nan.

    More points can be added to the polygon after this call.

    """
    if self._polyline: area = Math.nan
    if self._num < 2:
      perimeter = 0
      if not self._polyline: area = 0
      return self._num, perimeter, area

    if self._polyline:
      perimeter = self._perimetersum.Sum()
      return self._num, perimeter, area

    _, s12, _, _, _, _, _, S12 = self._earth._GenInverse(
      self._lat1, self._lon1, self._lat0, self._lon0, self._mask)
    perimeter = self._perimetersum.Sum(s12)
    tempsum = Accumulator(self._areasum)
    tempsum.Add(S12)
    crossings = self._crossings + PolygonArea._transit(self._lon1, self._lon0)
    if crossings & 1:
      tempsum.Add( (1 if tempsum.Sum() < 0 else -1) * self._area0/2 )
    # area is with the clockwise sense.  If !reverse convert to
    # counter-clockwise convention.
    if not reverse: tempsum.Negate()
    # If sign put area in (-area0/2, area0/2], else put area in [0, area0)
    if sign:
      if tempsum.Sum() > self._area0/2:
        tempsum.Add( -self._area0 )
      elif tempsum.Sum() <= -self._area0/2:
        tempsum.Add(  self._area0 )
    else:
      if tempsum.Sum() >= self._area0:
        tempsum.Add( -self._area0 )
      elif tempsum.Sum() < 0:
        tempsum.Add(  self._area0 )

    area = 0 + tempsum.Sum()
    return self._num, perimeter, area

  # return number, perimeter, area
  def TestPoint(self, lat, lon, reverse = False, sign = True):
    """Compute the properties for a tentative additional vertex

    :param lat: the latitude of the point in degrees
    :param lon: the longitude of the point in degrees
    :param reverse: if true then clockwise (instead of
      counter-clockwise) traversal counts as a positive area
    :param sign: if true then return a signed result for the area if the
      polygon is traversed in the "wrong" direction instead of returning
      the area for the rest of the earth
    :return: a tuple of number, perimeter (meters), area (meters^2)

    """
    if self._polyline: area = Math.nan
    if self._num == 0:
      perimeter = 0
      if not self._polyline: area = 0
      return 1, perimeter, area

    perimeter = self._perimetersum.Sum()
    tempsum = 0 if self._polyline else self._areasum.Sum()
    crossings = self._crossings; num = self._num + 1
    for i in ([0] if self._polyline else [0, 1]):
      _, s12, _, _, _, _, _, S12 = self._earth._GenInverse(
        self._lat1 if i == 0 else lat, self._lon1 if i == 0 else lon,
        self._lat0 if i != 0 else lat, self._lon0 if i != 0 else lon,
        self._mask)
      perimeter += s12
      if not self._polyline:
        tempsum += S12
        crossings += PolygonArea._transit(self._lon1 if i == 0 else lon,
                                         self._lon0 if i != 0 else lon)

    if self._polyline:
      return num, perimeter, area

    if crossings & 1:
      tempsum += (1 if tempsum < 0 else -1) * self._area0/2
    # area is with the clockwise sense.  If !reverse convert to
    # counter-clockwise convention.
    if not reverse: tempsum *= -1
    # If sign put area in (-area0/2, area0/2], else put area in [0, area0)
    if sign:
      if tempsum > self._area0/2:
        tempsum -= self._area0
      elif tempsum <= -self._area0/2:
        tempsum += self._area0
    else:
      if tempsum >= self._area0:
        tempsum -= self._area0
      elif tempsum < 0:
        tempsum += self._area0

    area = 0 + tempsum
    return num, perimeter, area

  # return num, perimeter, area
  def TestEdge(self, azi, s, reverse = False, sign = True):
    """Compute the properties for a tentative additional edge

    :param azi: the azimuth at the current the point in degrees
    :param s: the length of the edge in meters
    :param reverse: if true then clockwise (instead of
      counter-clockwise) traversal counts as a positive area
    :param sign: if true then return a signed result for the area if the
      polygon is traversed in the "wrong" direction instead of returning
      the area for the rest of the earth
    :return: a tuple of number, perimeter (meters), area (meters^2)

    """

    if self._num == 0:               # we don't have a starting point!
      return 0, Math.nan, Math.nan
    num = self._num + 1
    perimeter = self._perimetersum.Sum() + s
    if self._polyline:
      return num, perimeter, Math.nan

    tempsum =  self._areasum.Sum()
    crossings = self._crossings
    _, lat, lon, _, _, _, _, _, S12 = self._earth._GenDirect(
      self._lat1, self._lon1, azi, False, s, self._mask)
    tempsum += S12
    crossings += PolygonArea._transitdirect(self._lon1, lon)
    _, s12, _, _, _, _, _, S12 = self._earth._GenInverse(
      lat, lon, self._lat0, self._lon0, self._mask)
    perimeter += s12
    tempsum += S12
    crossings += PolygonArea._transit(lon, self._lon0)

    if crossings & 1:
      tempsum += (1 if tempsum < 0 else -1) * self._area0/2
    # area is with the clockwise sense.  If !reverse convert to
    # counter-clockwise convention.
    if not reverse: tempsum *= -1
    # If sign put area in (-area0/2, area0/2], else put area in [0, area0)
    if sign:
      if tempsum > self._area0/2:
        tempsum -= self._area0
      elif tempsum <= -self._area0/2:
        tempsum += self._area0
    else:
      if tempsum >= self._area0:
        tempsum -= self._area0
      elif tempsum < 0:
        tempsum += self._area0

    area = 0 + tempsum
    return num, perimeter, area

  def CurrentPoint(self):
    """Return the current vertex

    :return: the current vertex as a tuple of latitude, longitude

    """

    return self._lat1, self._lon1
