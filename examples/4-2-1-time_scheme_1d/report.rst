Problem Description
===================

This example solves Laplace equation

.. math::
   
   \frac{d \phi}{d t} = \frac{\partial }{\partial x} ( \beta \frac{\partial \phi}{\partial x})

where, :math:`\beta = 1` in present case.


Time Scheme
===================

There are three typical time scheme for discreting Poisson equation. 

- Explicit Scheme
  
.. math::
   
   \frac{\phi^{n+1} - \phi^n}{d t} = \frac{\partial }{\partial x} ( \beta \frac{\partial \phi^n}{\partial x})

- Implicit Scheme

.. math::
   
   \frac{\phi^{n+1} - \phi^n}{d t} = \frac{\partial }{\partial x} ( \beta \frac{\partial \phi^{n+1}}{\partial x})

- Crank-Nicolson

.. math::
   
   \frac{\phi^{n+1} - \phi^n}{d t} = \frac{1}{2} \frac{\partial }{\partial x} ( \beta \frac{\partial \phi^{n}}{\partial x}) + \frac{1}{2} \frac{\partial }{\partial x} ( \beta \frac{\partial \phi^{n+1}}{\partial x})

Above three schemes can be written in a general form

.. math::
   
   \frac{\phi^{n+1} - \phi^n}{d t} = (1 - f) \frac{\partial }{\partial x} ( \beta \frac{\partial \phi^{n}}{\partial x}) + f \frac{\partial }{\partial x} ( \beta \frac{\partial \phi^{n+1}}{\partial x})

When, :math:`f=0`, 1 and 0.5, the schemes are explicit, implicit and C-N, respectively. if :math:`0.5 \le f \le 1`, the equation is stable. if :math:`0 \le f < 0.5`, the eqution is stable with conditon. The condition is 

.. math::

  \frac{\beta \Delta t}{\Delta x^2} \le \frac{1}{2(1-2f)}

where, :math:`F_o = \frac{\beta \Delta t}{\Delta x^2}` is mesh Fourier number.

Test 
================

We set a 1D domain, :math:`x \in [0,1]`, discrete by n = 20. 

.. math::
   
   \phi|_{t = 0} &= 0, \\
   \phi|_{x = 0} &= 1, \\
   \phi|_{x = 1} &= 0


.. plot::

   import matplotlib.pyplot as plt
   import numpy as np

   x = np.linspace(0, 0.5, 100)
   y = []
   for v in x:
      y.append(0.01 / (2 * (1-2*v)))

   plt.plot(x, y)
   plt.ylabel("$\Delta t $")
   plt.xlabel("$f \in [0, 0.5)$")
   plt.text(0.03, 0.4, "if $\Delta x = 1/10$,\n then $\Delta t$ must below the line", fontsize=12)
   plt.grid()
   plt.show()

As shown in above figure, the explicit scheme should choose tiny time step to ensure obtaining the stable result.

Here, we choose :math:`F_o = 0.6`. Explicit scheme will have ossilate results.

.. figure:: _static/{{folder_name}}/explicit.gif
   :alt: explicit result
   :align: center 

   One dimension result of explicit solver.

Same configuration was set for implicit solver and CN solver. The results are better.

.. figure:: _static/{{folder_name}}/implicit.gif
   :alt: implicit result
   :align: center 

   One dimension result of implicit solver.

.. figure:: _static/{{folder_name}}/CN.gif
   :alt: CN result
   :align: center 

   One dimension result of CN solver.


