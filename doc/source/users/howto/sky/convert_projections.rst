.. _sec_sky_convert_projections:

How to convert sky map projections?
===================================

The following code illustrates how to convert a map in HealPix projection
into a map in cartesian projection. Map conversion is performed using the
+= operator that adds the bilinearly interpolated intensity values from one
map to another. The example code applies to any kind of map projections.

Python:

.. code-block:: python

    >>> import gammalib
    >>> healpix = gammalib.GSkyMap("healpix.fits")
    >>> map = gammalib.GSkyMap("CAR","GAL",0.0,0.0,0.5,0.5,100,100)
    >>> map += healpix
    >>> map.save("carmap.fits")

C++:

.. code-block:: cpp

    #include "GammaLib.hpp"
    GSkyMap healpix("healpix.fits");
    GSkyMap map("CAR","GAL",0.0,0.0,0.5,0.5,100,100);
    map += healpix;
    map.save("carmap.fits");
    
