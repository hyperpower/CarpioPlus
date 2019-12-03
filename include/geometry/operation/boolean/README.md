# Overview 

Files in this folder define all of the functions for Boolean operation.

The Boolean operations include:

- Intersection
- Union
- Subtract

Following tables show the functions which have been implemented

## Table of Intersection
|          |        Segment          |  Box         | Line         |
|----------|:-----------------------:|--------------|--------------|
| Segment  | :ballot_box_with_check: |:construction:|:construction:|
| Box      |                         |:construction:|:construction:|
| Line     |                         |              |:construction:|
| Polygon  |                         |              |              |

## Table of Union
|          |             Segment          |  Box         | Line     |
|----------|------------------------------|--------------|----------|
| Segment  |:negative_squared_cross_mark: |              |          |
| Box      |                              |:construction:|          |
| Line     |                              |              |          |

## Table of Subtract

# File Structure
| file | content |
|------|---------|
|[line_box.hpp](line_box.hpp)| A line cuts a box, or a box cuts a line |

# Segment vs. Segment

All of related functions can be found in '[segment_segment.hpp](segment_segment.hpp)'. 

The basic idea of finding the intersection point of two segment in 2D is simple. A **class IntersectionPairSS_** has been created for saving four points from two segments and intersection point.



## Usage
