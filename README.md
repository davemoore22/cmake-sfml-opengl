# cmake-sfml-opengl
A minimal CMake/SFML/OpenGL Example in VSCode

Uses the rewrite of the SFML OpenGL example code originally at https://www.sfml-dev.org/tutorials/2.5/window-opengl.php by Tim Stoddard (https://github.com/gamepopper) at https://gist.github.com/gamepopper/1931ca297f3decdee90e785f12762192.

A very quick and dirty rewrite that needs serious optimisation.

Note that when using Cmake Tools in VSCode, it doesn't seem to set the working directory on the run kit, only on the debug kit so it can't find the resource files copied across into the build directory. Also works from the command line though as well.
