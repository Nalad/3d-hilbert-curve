# 3D Hilbert Curve

Generate a 3D Hilbert curve, transform coordinates to screen space using MVP (Model View Projection), and render it by PostScript. Written in pure C (std99) with self-made OOP. 

### Examples
![01](https://github.com/Nalad/3d-hilbert-curve/blob/master/examples/01.png)
![02](https://github.com/Nalad/3d-hilbert-curve/blob/master/examples/02.png)
![03](https://github.com/Nalad/3d-hilbert-curve/blob/master/examples/03.png)

### Requirements

- Any C modern compiler (GCC/Clang)

### Usage

    usage: Hilbert3D depth pictureWidth cubeWidth distance xOrigin yOrigin zOrigin xAngle yAngle

    arguments:
		depth - order of the curve
		pictureWidth - self explaining
		cubeWidth - self explaining
		distance - distance from the camera
		xOrigin, yOrigin, zOrigin - translation
		xAngle, yAngle - rotation
