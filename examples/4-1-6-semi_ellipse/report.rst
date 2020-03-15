Problem Description
===================

The domain is a unit square :math:`x \in [0.0, 1.0], y \in [0.0, 1.0]`. The boudary conditions are

.. math::
   \phi = \sqrt{ 1 - (6y)^2} &, \quad \text{if} \quad x = 0.0 \quad \text{and} \quad y = [0.0, 1/6]\\
   \frac{d \phi}{d x} = 0 &, \quad \text{if} \quad x = 1.0\\
   \phi = \sqrt{ 1 - (6x)^2} &, \quad \text{if} \quad y = 0.0 \quad \text{and} \quad x = [0.0, 1/6]\\
   \frac{d \phi}{d y} = 0 &, \quad \text{if} \quad y = 1.0\\

The velocity is :math:`\mathbf{u}=(1, 1)`. 

.. figure:: _static/{{folder_name}}/illustration.png
   :alt: the semi-ellipse profile
   :align: center 

   Illustration of the problem.

The semi-ellipse profile will be on the center of the domain.

Results
===================

Scheme Comparsion
-------------------
Initial condition of the domain is :math:`\phi = 0.0`. The iteration stops when 3 norms are all less than :math:`10^{-5}`. CFL number is 0.4. 

The final profile at section (x = 0.7) is shown as follows.

.. figure:: _static/{{folder_name}}/compare_section.png
   :alt: the semi-ellipse profile
   :align: center 

   Comparsion of schemes.


