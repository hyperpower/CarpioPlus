Introduction
===================

A Bézier curve is is a parametric curve used in computer graphics and related fields. Details can be found at Wiki_. A few Bézier curves have been implemented.

Linear Bézier curves
=====================
Given distinct points :math:`\mathbf{P}_0` and :math:`\mathbf{P}_1`, a linear Bézier curve is simply a straight line between those two points. The curve is given by

.. math::
   \mathbf{B}(t) &= \mathbf{P}_0 + t(\mathbf{P}_1 - \mathbf{P}_0)\\
                 &= (1-t)\mathbf{P}_0 + t\mathbf{P}_1
   :label: eq_bezier1

where, :math:`0 \le t \le 0`

.. figure:: _static/{{folder_name}}/b1.png
   :alt: linear bezier curves
   :align: center 

   An example of linear bezier curves.

Quadratic Bézier curves
========================
A quadratic Bézier curve is the path traced by the function :math:`\mathbf{B}(t)`, given points :math:`\mathbf{P}_0`, :math:`\mathbf{P}_1`, and :math:`\mathbf{P}_2`,

.. math::
   \mathbf{B}(t) = (1-t)[(1-t)\mathbf{P}_0 + t\mathbf{P}_1] + t[(1-t)\mathbf{P}_1 + t\mathbf{P}_2]
   :label: eq_bezier2

where, :math:`0 \le t \le 0`

.. figure:: _static/{{folder_name}}/b2.png
   :alt: Quadratic bezier curves
   :align: center 

   An example of Quadratic bezier curves.

Cubic Bézier curves
========================
Cubic Bézier curve can be defined as an affine combination of two quadratic Bézier curves:

Given points :math:`\mathbf{P}_0`, :math:`\mathbf{P}_1`, :math:`\mathbf{P}_2` and :math:`\mathbf{P}_3`

.. math::
   \mathbf{B}(t) = (1-t) \mathbf{B}_{\mathbf{P}_0,\mathbf{P}_1,\mathbf{P}_2} + t \mathbf{B}_{\mathbf{P}_1,\mathbf{P}_2,\mathbf{P}_3}
   :label: eq_bezier3

where, :math:`0 \le t \le 0`

.. figure:: _static/{{folder_name}}/b3.png
   :alt: Cubic Bezier curves
   :align: center 

   An example of Cubic bezier curves.

High Order Bézier curves
========================
 
A **recursive definition** for the Bézier curve of degree n expresses it as a point-to-point linear combination (linear interpolation) of a pair of corresponding points in two Bézier curves of degree n − 1.

Let :math:`\mathbf {B}_{\mathbf {P} _{0}\mathbf {P} _{1}\ldots \mathbf {P} _{n}}` denote the Bézier curve determined by any selection of points :math:`\mathbf{P}_0`, :math:`\mathbf{P}_1`, :math:`\ldots` :math:`\mathbf{P}_n`. Then to start,

.. math::
   \mathbf{B}(t) = (1-t) \mathbf{B}_{\mathbf {P} _{0}\mathbf {P} _{1}\ldots \mathbf {P} _{n-1}} + t \mathbf{B}_{\mathbf {P} _{1}\mathbf {P} _{2}\ldots \mathbf {P} _{n}}
   :label: eq_bezierh

.. figure:: _static/{{folder_name}}/bh.png
   :alt: High order Bezier curves
   :align: center 

   An example of High order bezier curves.


.. _Wiki: https://en.wikipedia.org/wiki/B%C3%A9zier_curve

