.. tvd_schemes:

=================
TVD schemes
=================

Introduction
=============
The TVD schemes originally developed by Harten

Semi-discrete steady-state TVD schemes
=======================================
The section describes the method for obtaining the solution of the **steady-state** of the advection equation. Essentially, Eq. :eq:`eq_con` is equivalent to 

.. math::
   \frac{\partial (\mathbf{u} \phi)}{\partial \mathbf{x}} = 0
   :label: eq_steady

Here, we discuss the Eq. :eq:`eq_con` in one-dimension. Therefore, the eqution becomes

.. math::
   \frac{d \phi}{d t} + \frac{d (u \phi)}{d \mathbf{x}} = 0
   :label: eq_con1

Discretization method on the X direction was shown in following figure.

.. plot:: py_matplot/convection_1d.py
   :align: center 

The variables are

.. table:: Summary of Variables.
   :widths: auto
   :align: center 

   ================= ========================================
     Sign             Note   
   ================= ========================================
    :math:`\phi`      Scalar on center of the control Volume
    :math:`\Delta x`  Size of the control volume
   ================= ========================================

The finite volume method is used to obtain the flux-conseravtive form over a control volume :math:`C_i = [x_{i-1/2}, x_{i-1/2}]`

.. math::
   \frac{d \phi_i}{d t} + \frac{ u (\phi_{i+1/2} - \phi_{i-1/2})}{\Delta x} = 0
   :label: eq_dis_con1

In order to predict the varible value at the cell-face, :math:`\phi_{i+1/2}` and :math:`\phi_{i-1/2}`. Many methods could be found in the literature.

:math:`\kappa`-scheme
-----------------------
:math:`\kappa`-scheme firstly introduecd by Van Leer. The numerical flux of the :math:`\kappa`-scheme on uniform 1D grids reads:

.. math::
   \phi_{i+1/2} = \phi_{i} + (\frac{1 + \kappa}{4} (\phi_{i + 1} - \phi_{i}) + \frac{1 - \kappa}{4} (\phi_{i} - \phi_{i - 1}))
   :label: eq_k_i12

Eq. :eq:`eq_k_i12` can be extend to non-uniform 1D grids:

.. math::
   \phi_{i+1/2} = \phi_{i} + \frac{\Delta x_i}{2}(\frac{1 + \kappa}{2} (\frac{d \phi}{d x})_{i+1/2} + \frac{1 - \kappa}{2} (\frac{d \phi}{d x})_{i-1/2} ) 

where,

.. math::
   (\frac{d \phi}{d x})_{i+1/2} &= \frac{\phi_{i+1} - \phi_{i}}{x_{i+1} - x_{i}} \\
   (\frac{d \phi}{d x})_{i-1/2} &= \frac{\phi_{i} - \phi_{i-1}}{x_{i} - x_{i-1}} \\
   \Delta x_i &= x_{i+1/2} - x_{i-1/2}
