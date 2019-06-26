=================
Convection Scheme
=================

Numericla simulation of convection-dominated flow can be described as

.. math::
   \frac{\partial \phi}{\partial t} + \frac{\partial (\mathbf{u} \phi)}{\partial \mathbf{x}} = 0
   :label: eq_con

where :math:`\phi = \phi(\mathbf{x}, t)` denotes the dependent varibable and :math:`\mathbf{u}` is the advection velocity. The Eq. :eq:`eq_con` 

In the past decades, several different series of shemes have been developed.

.. toctree::

   tvd
   eno