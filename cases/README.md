# Cases for Carpio Project

This folder include all the test cases for Carpio project.

## Table of Content

1. Geometry
   1. Basic Geometry Objects
      1. :ballot_box_with_check: Point Location 
      2. :ballot_box_with_check: Segments intersection
      3. Segment vs box
      4. Triangle intersection
      5. :ballot_box_with_check: Point in polygon
      6. Polygon collision detection
      7. Polygon boolean
   2. Triangle Surface
      1. Build triangle surface
      2. Delauney Triangulation
2. Algebra
   1. Array
      1. Array operations
   2. Matrix
      1. Matrix operations
3. Domain
   1. Structure
   2. Octree
3. Equation
   1. Convection
      1. One Step 1D
      2. Gate 1D
      3. One Step 2D
      4. Gate 2D
   2. Poisson
   3. Navier-Stokes

## Prerequisites

Compliation of the cases depends on multiple projects. They are CMake, Python 3 and Multiple packages of Python.


## Usage

Each case can work independently. Follwing instruction shows how to these cases.

If you want to run case "1-1-1-point_location" (project_name), you can chang your working directory to this folder.

```
cd path_to_project
```

And run the 'run.py'

```
python3 run.py
```

The case will automatically run for you. The figures will put in fig folder, and the data will put in 'data' folder.

## To do list

- delete report.py in 1-1-2

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details