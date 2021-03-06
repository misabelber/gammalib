.. _issues:

Known issues
============

Below you will find a list of known issues.

- :ref:`Many (but not all) unit tests fail <issue_test_fail>`
- :ref:`Python module does not work <issue_python>`
- :ref:`Mac OS X <issue_macosx>`
- :ref:`Solaris <issue_solaris>`
- :ref:`OpenSolaris <issue_opensolaris>`

.. _issue_test_fail:

.. topic:: Many (but not all) unit tests fail

   Occasionally it may happen that the cfitsio library is not found when
   configuring GammaLib. The library will compile successfully without
   cfitsio, but in that case FITS I/O will not be supported. Consequently,
   many unit tests will fail. If you are sure that cfitsio is installed,
   but the path where the library and the path where the ``fitsio.h`` header
   reside are non-standard, you may add the paths explicitly during
   configuration using:

   .. code-block:: bash

      $ ./configure LDFLAGS='-L/path/to/cftsio/library' CPPFLAGS='-I/path/to/fitsio.h/header'

   The same logic applies for finding the readline and ncurses libraries,
   although these libraries are not manadatory for getting the full
   GammaLib functionalities.

   Alternatively, cfitsio can be found when compiling GammaLib but not 
   when using the shared library.  To solve the issue, locate the 
   directory where the shared ``libcfitsio`` library resides and then type

   .. code-block:: bash

      $ export LD_LIBRARY_PATH=/directory/to/lib:$LD_LIBRARY_PATH

   on Unix based systems or

   .. code-block:: bash

      $ export DYLD_LIBRARY_PATH=/directory/to/lib:$DYLD_LIBRARY_PATH
   
   on Mac OS X (``/directory/to/lib`` should be replaced by the correct
   library path on your system).

.. _issue_python:

.. topic:: Python module does not work

   GammaLib includes a Python module that is built from so called wrapper 
   files that are autogenerated using the `swig <http://www.swig.org/>`_
   tool. These wrapper files are shipped with a GammaLib release, but if
   you use the code from git you need `swig <http://www.swig.org/>`_
   to generate the wrapper files during the build step. In any case,
   to compile the Python module GammaLib needs the ``Python.h`` header file
   which may not necessarily be installed on your system. Check the output
   of ``./configure`` to examine the configuration that GammaLib has
   detected. You may see the following::

    * Python                       (yes)
    * Python.h                     (yes)
    * Python wrappers              (yes)
    * swig                         (yes)

   Recall, if the wrappers exist you do not need `swig <http://www.swig.org/>`_,
   but if the wrappers don't exist you need `swig <http://www.swig.org/>`_.
   If the ``Python.h`` header file does not exist then install the Python
   development package.

.. _issue_macosx:

.. topic:: Mac OS X

   Dependent on the Mac OS X version you are using, not everything that
   is needed to install GammaLib may be available (e.g. automake, libtool, 
   etc...). The easiest way to get the needed software is using a package 
   management system such as `MacPorts <https://www.macports.org/>`_ or 
   `Homebrew <http://brew.sh/>`_. On a fresh El Capitan install you need 
   for example the following from `Homebrew <http://brew.sh/>`_:

   .. code-block:: bash

     $ brew install automake
     $ brew install libtool
     $ brew install cfitsio
     $ brew install swig

   `swig <http://www.swig.org/>`_ is only necessary if you installed the 
   code from git. Dependening on your system, you also may need to install
   the Python development package.

.. _issue_solaris:

.. topic:: Solaris

   Although GammaLib builds on Solaris using the Sun compiler, there are
   problems with global symbols in shared libraries and exception catching,
   which prevents the FITS interface to work correctly. GammaLib has
   however been built and tested successfully using the GNU compiler, and
   this is the only build method that is currently supported. Problems have
   also been encountered when compiling cfitsio versions more recent than
   3.250. The problems have been reported to the cfitsio developer team,
   and are likely to be solved in the future. For the time being, it is
   recommended to use cfitsio version 3.250 on Solaris.

.. _issue_opensolaris:

.. topic:: OpenSolaris

   On OpenSolaris, the same problems concerning the SunStudio compiler
   occur as for Solaris, and also here, the GNU compiler is the recommended
   tool to build GammaLib. Also here, cfitsio version 3.250 is the
   recommended library as more recent version feature relocation
   problems. GammaLib has been tested using gcc 4.3.2 on OpenSolaris
   2009.06. Make sure to create the following symbolic links if they do
   not yet exist on your system:

   .. code-block:: bash

      $ ln -s /usr/bin/gcc4.3.2 /usr/bin/gcc
      $ ln -s /usr/bin/g++4.3.2 /usr/bin/g++

   They avoid excess warnings during compilation.
