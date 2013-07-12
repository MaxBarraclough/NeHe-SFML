-  Update credits at top of lessons

-  Indentation of lines between glBegin(); and glEnd();

-  Add links to NeHe's web-pages explaining the lessons' content,
  e.g. lesson 5 -> http://nehe.gamedev.net/tutorial/3d_shapes/10035/

-  Should we be using floats, in the form of 1.0f, rather than doubles, if they eventually end up as GLfloat or GLdouble ?

-  Port the rest of the lessons. (Currently we have only the first 12.)

-  Fix repetitive vsync configuration adjustment, see http://en.sfml-dev.org/forums/index.php?topic=4481.msg29255#msg29255

-  Does Lesson 9 need a depth-buffer? Or Lesson 11?

-  Is everything ok regarding Z-buffer, or whichever hidden-region-elimination (TODO: what are these called again?) algo we're going with? Things look funny... maybe this is actually all fine? Lesson 7 certainly doesn't look right... is that intentional?
  Also Lesson 5

-  Cruft-removal: delete any stray commented-out code

-  Formatting:
    +  no more tabs, just spaces. Or the other way round.
    +  spaces between operators: "a + b / c" rather than "a+b/c"

-  Go through anywhere in the code where the string "TODO" appears

-  Port the non-lesson cpp files

-  Add a COPYING file (or copying.txt), and reintroduce the requirement for it in CMakeLists.txt

-  Not sure how to do this regarding resource files, but we could do a more 'proper' installation procedure.
  See http://www.cmake.org/cmake/help/cmake_tutorial.html :: "Installing and Testing (Step 3)"
  This would probably mean changes to source: we couldn't just open "data/world.txt". Presumably CPack has a way of handling this stuff somehow.

