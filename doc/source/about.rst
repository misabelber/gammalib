.. _about:

About
=====

GammaLib is a versatile toolbox for the high-level analysis
of astronomical gamma-ray data. It consists of a C++ library and a
Python module that exposes the full functionality of the library
to Python. The library provides an abstract data analysis framework
that is independent of any specific gamma-ray telescope. Instrument
specific aspects, such as handling of data formats and response
functions, are implemented as isolated and well defined modules sharing
an identical abstract interface. This enables a joint multi-instrument
analysis of data, allowing for consistent broad-band spectral fitting
or imaging. So far, GammaLib supports analysis of data obtained with
COMPTEL, Fermi/LAT, and Cherenkov telescopes (CTA, H.E.S.S., MAGIC,
VERITAS).

Except for HEASARC's cfitsio library that is used to implement
the FITS interface, GammaLib does not rely on any third-party 
software. All functionalities are implemented natively.

GammaLib is developed by a team of enthousiastic gamma-ray astronomers with
support from engineers. We regularily organise
`coding sprints <https://cta-redmine.irap.omp.eu/projects/ctools/wiki/Coding_sprints>`_
where key developers but also newcomers meet to discuss the developments 
and next steps, and advance with the coding of the software.


Acknowledging or citing GammaLib
--------------------------------

If you use GammaLib for work/research presented in a publication we ask you
to include the formal reference

   J. Knoedlseder, M. Mayer, C. Deil, J.-B. Cayrou, E. Owen, N. Kelley-Hoskins,
   C.-C. Lu, R. Buehler, F. Forest, T. Louge, H. Siejkowski, K. Kosack,
   L. Gerard, A. Schulz, P. Martin, D. Sanchez, S. Ohm, T. Hassan, and
   S. Brau-Nogue, 2016, A&A, in press (`arXiv:1606.00393 <http://arxiv.org/abs/1606.00393>`_)

in your paper and that you add the following acknowledgment:

   *This research made use of GammaLib, a community-developed toolbox for
   the high-level analysis of astronomical gamma-ray data.*

If you wish, you can also include a link to http://cta.irap.omp.eu/gammalib/
(if the journal allows this) in addition to the above text, as well as the
identifier of the code in the Astrophysics Source Code Library (ASCL):
`ascl:1110.007 <http://ascl.net/1110.007>`_.

If you are giving a presentation or talk featuring work/research that makes
use of GammaLib, we suggest using this logo on your title slide:

.. figure:: gammalib-logo.jpg
   :width: 150px
   :align: center

And to see who published an article using GammaLib you may check the `following link <http://cdsads.u-strasbg.fr/cgi-bin/nph-ref_query?bibcode=2016A%26A...593A...1K&amp;refs=CITATIONS&amp;db_key=AST>`_.


License
-------

GammaLib is free software distributed under the GNU GPL license version 3.
