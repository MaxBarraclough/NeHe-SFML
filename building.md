                         Building the Tutorials
==========================================================================

CMake is used to build the tutorials.
Whatever your platform, you will need [CMake](http://www.cmake.org/)
and [SFML 2.0](http://www.sfml-dev.org/).

There is an [official SFML tutorial](https://github.com/LaurentGomila/SFML/wiki/Tutorial%3A-Build-your-SFML-project-with-CMake)
on the topic of building your CMake-based SFML code.

This document aims to be more immediately useful to someone
going through the tutorial.


                           Ubuntu Specifics
==========================================================================

Four shell-scripts are included. Other than nuke-cmake.sh they are all
useful for running CMake to configure a build on Unix-like systems.

ubuntu-cmake-run.sh assumes SFML 2 has been installed from source,
to the default places.

Example build on Ubuntu:
>  ./ubuntu-cmake-run.sh && make


                            Windows Specifics
==========================================================================

Using CMake GUI and Visual Studio 2010 64 bit
--------------------------------------------------------------------------

(Adjust the specifics to your machine and Visual Studio version.)

*  Open cmake-gui (also known simply as "CMake")

*  For the "Where is the source code:" field, select

              c:\wherever\NeHe-SFML2

*  For "Where to build the binaries", create and select a subfolder of
   NeHe-SFML2, called, say, "win-build".
   (It will still work if you just choose NeHe-SFML2, but it's messy.)

*  Using the "Add Entry" button, add two entries of type PATH:

              CMAKE_MODULE_PATH    c:/wherever/SFML-2.0/cmake/Modules
              SFML_ROOT            c:/wherever/SFML-2.0/

   (Don't worry about forward-slash vs backslash - CMake uses forward-slashes,
   but it will cope fine with backslashey input.)

   Optionally: Configure an entry named

              CMAKE_INSTALL_PREFIX

*  Click the Configure button

*  Select "Visual Studio 10 Win64"

*  Click Finish

*  When that's done, click Generate.
   (When this finishes, the Visual Studio 2010 project will be ready.)

*  In Visual Studio, open

       c:\wherever\NeHe-SFML2\win-build\ALL_BUILD.vcxproj

*  Build as usual

*  Before you can run the output .exe files, you will need to add
   c:\wherever\SFML-2.0\bin to your 'PATH' environmental variable.

*  When you have done this, you should be able to run the exe for
   the first lesson (which is a blank screen).

   You will find it in

              c:\wherever\NeHe-SFML2\win-build\Debug\nehe_lesson_01.exe

*  Other lessons require the files in the "data" folder, which is expected to exist at
   the same level as the exe file. Either move the exes to c:\wherever\NeHe-SFML2
   (such that they live at the same level as the "data" folder),
   or move the 'data' folder to Debug.
   If you're feeling fancy, you can instead create a symlink using "mklink /D ...."

*  With that done, you should be able to freely run the lessons' exes.


                            Mac Specifics
==========================================================================

Unlike in Ubuntu, in OS X there is no standard SFML installation:
mac-cmake-run-with-clang.sh and mac-cmake-run.sh will need adjusting
depending on where SFML 2 resides on your system.

                        Mac 'Framework' Issues
--------------------------------------------------------------------------

SFML 2 for Mac has some framework dependencies, apparently caused by
Apple no longer shipping OS X with a bundled X server.

Annoyingly, this is not apparent at CMake time, or even at make time:
it will only cause a problem at run-time.

(OS X's X server lives on as XQuartz, which is not included in OS X.
SFML for Mac does not use X11, but has dependencies on things which
generally won't be present on a Mac unless XQuartz is installed.)

If you have XQuartz installed, you should in theory already have the
required frameworks.
I found that Snow Leopard does *not*  appear to have the frameworks,
despite including an X server.

If you do not have the frameworks installed, you can either install XQuartz
(http://xquartz.macosforge.org/) or get just the specific frameworks:

>    git clone https://github.com/LaurentGomila/SFML

then look in SFML/extlibs/libs-osx/Frameworks
Move both the directories contained therein to /Library/Frameworks

Discussion of this issue can be found at:

https://github.com/LaurentGomila/SFML/commit/54bc8644844aaea2209c495f57b240cd371810c6

https://github.com/LaurentGomila/SFML/issues/241

https://github.com/LaurentGomila/SFML/issues/342

