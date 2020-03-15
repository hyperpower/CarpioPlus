Problem Description
===================

The 1D gate profile has been initialized as:

.. math::
   x &\in [0.25, 0.75]   , \phi = 1,  \\
   x &\notin [0.25, 0.75], \phi = 0
   :label: eq_initial

The velocity is :math:`u=1` m/s. After 1 seconds (100 steps with :math:`\Delta t = 0.01`), the final profile should be 

.. math::
   x \in [1.25, 1.75]&, \phi = 1,  \\
     else            &, \phi = 0
   :label: eq_final 

.. figure:: _static/{{folder_name}}/illustration.png
   :alt: the gate profile
   :align: center 

   Illustration of the problem.

Results
===================

First Order Upwind
-------------------
The first schem is First Order Upwind (FOU).

.. figure:: _static/{{folder_name}}/FOU.gif
   :alt: the gate profile
   :align: center 

   First Order Upwind.

.. figure:: _static/{{folder_name}}/FOU_000100.png
   :alt: the gate profile
   :align: center 

   First Order Upwind (Last Step).

QUICK 
-------------------
The second scheme is QUICK. Its accuracy is better than FOU.

.. figure:: _static/{{folder_name}}/QUICK.gif
   :alt: the gate profile
   :align: center 

   QUICK.

.. figure:: _static/{{folder_name}}/QUICK_000100.png
   :alt: the gate profile
   :align: center 

   QUICK (Last Step).

Normal Compare
---------------------
Here, we plot Norm1, Norm2 and Norminf changing with steps.

.. figure:: _static/{{folder_name}}/FOU_norm.png
   :alt: the gate profile
   :align: center 

   FOU Norm.

.. figure:: _static/{{folder_name}}/QUICK_norm.png
   :alt: the gate profile
   :align: center 

   QUICK Norm.




