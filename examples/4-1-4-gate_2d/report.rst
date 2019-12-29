Problem Description
===================

The domain is a unit square :math:`x \in [0.0, 1.0], y \in [0.0, 1.0]`. The boudary conditions are

.. math::
   \phi = 1 &, \quad \text{if} \quad x = 0.0 \quad \text{and} \quad y = [0.0, 0.3]\\
   \frac{d \phi}{d x} = 0 &, \quad \text{if} \quad x = 1.0\\
   \phi = 0 &, \quad \text{if} \quad y = 0.0\\
   \frac{d \phi}{d y} = 0 &, \quad \text{if} \quad y = 1.0\\

The velocity is :math:`\mathbf{u}=(1, 1)`. 

.. figure:: _static/{{folder_name}}/illustration.png
   :alt: the gate profile
   :align: center 

   Illustration of the problem.

The :math:`y=x` and :math:`y=x+0.3` is a step which jumps between 1 and 0.

Results
===================

First Order Upwind
-------------------
Initial condition of the domain is :math:`\phi = 0.0`. The iteration stops when :math:`||\phi^{n+1} - \phi^{n}|| < 10^{-4}`. CFL number is 0.5.


