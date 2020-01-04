Problem Description
===================

The domain is a unit square :math:`x \in [0.0, 1.0], y \in [0.0, 1.0]`. The boudary conditions are

.. math::
   \phi =\sin^2(\frac{10}{3} \pi y) &, \quad \text{if} \quad x = 0.0 \quad \text{and} \quad y = [0.0, 0.3]\\
   \frac{d \phi}{d x} = 0 &, \quad \text{if} \quad x = 1.0\\
   \phi = 0 &, \quad \text{if} \quad y = 0.0\\
   \frac{d \phi}{d y} = 0 &, \quad \text{if} \quad y = 1.0\\

The velocity is :math:`\mathbf{u}=(1, 1)`. 

.. figure:: _static/{{folder_name}}/illustration.png
   :alt: the gate profile
   :align: center 

   Illustration of the problem.


Results
===================

This example tests three GRP (Generalized polynomial-ratio) schemes.

.. figure:: _static/{{folder_name}}/GPR0_section.png
   :alt: the gate profile
   :align: center 

   GPRO.

.. figure:: _static/{{folder_name}}/GPR12_section.png
   :alt: the gate profile
   :align: center 

   GPR12.

.. figure:: _static/{{folder_name}}/GPR13_section.png
   :alt: the gate profile
   :align: center 

   GPR13.



