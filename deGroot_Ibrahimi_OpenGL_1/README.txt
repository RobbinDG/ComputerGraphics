List of Modifications:
 - We defined the vertices and triangulations of both shapes (setupCube(), setupPyr())
 - We defined translation matrices for both shapes, and defined a projection matrix using the given values.
 - We defined a function to setup the VAO and VBO for an arbitrary set of triangles.
 - We defined openGL uniforms for the transformation and projection matrices, and use them in the shader to transform and project all the vertices.
 - We defined how rotation and scaling should work according to the specification.
 - We linked the Qt UI elements to their respective functions
 - We loaded and rendered a sphere object from file.

User instructions
 - The rotating buttons at the top rotate all elements around their axes in the x, y and z direction. You can reset the rotation with the "reset rotation" button.
 - Similarly, one may scale all objects by using the scale slider. This also can be reset with the appropriate button.