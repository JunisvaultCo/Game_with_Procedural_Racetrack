# OpenGL_Game_with_Procedural_Racetrack

Written by Iulia-Silvia Plăiașu

Uses the https://github.com/UPB-Graphics/gfx-framework framework. It is an implementation for the second homework
from the Elements of Computer Graphics class, but with a racetrack which was not in the assignment.
(Assignment: https://www.youtube.com/watch?v=eyZwa6lDm14 )

Example run: 
https://www.youtube.com/watch?v=3YMzCBdll7g

# Controls:

A, D - rotate left and right

W - move forwards

S - move backwards

Q, E - rotate camera around car

O - generate a new track

Z, X - zoom out minimap.


# Run (and compile) with

mkdir build

cd build

cmake .. && cmake --build . && .\bin\Debug\GFXFramework

Dependencies that need to be installed depend on the GFXFramework


# Procedural Racetrack Generation overall logic:

First, generate a random list of points. Connect each point with its neighbors in the list (the neighbors of the first are second and last).

Second, create the "outer" shell of the current polygon. The point is to create a polygon of minimum area that doesn't intersect itself at all (when that's possible. There are cases where some points will still overlap regardless, unless you distort the polygon, but these will work fine).

Third, add new points at the middle of each edge. Now you can easily use Bezier curves to smoothen out the corners and give the track the curved look!

Finally, the current track lacks width. So use vector operations to create two sets of points, an inner set and an outer set. Connect each two corresponding edges from each set by making two triangles. 

Obviously, several prunings are used to ensure a decent aspect of the track, preferably done as soon as possible. The results are not ideal, but decent in practice.
