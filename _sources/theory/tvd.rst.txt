.. tvd_schemes:

=================
TVD schemes
=================
 
Introduction
=============
The TVD schemes originally developed by Harten. The schemes written here are follow the definition in :cite:`Zhang2015`

One-Step Time-Space-Coupled Unsteady TVD Schemes
=================================================

Semi-discrete steady-state TVD schemes
=======================================
The section describes the method for obtaining the solution of the **steady-state** of the advection equation. Essentially, Eq. :eq:`eq_con` is equivalent to 

.. math::
   \frac{d \phi}{dt} = - \frac{\partial (\mathbf{u} \phi)}{\partial \mathbf{x}} 
   :label: eq_con_rhs

where :math:`\phi` denotes the dependent variable and :math:`\mathbf{u}` is the advection velocity. Eq. :eq:`eq_con_rhs` moves flux term on the right-hand side to emphasize the **semi-discrete nature**. Obtianing the steady-state solution of Eq. :eq:`eq_con` equivalent to solving the **steady** equation 

.. math::
   \frac{\partial (\mathbf{u} \phi)}{\partial \mathbf{x}} = 0 
   :label: eq_con_steady

This explained why the schemes discuss in this section called **semi-discrete steady-state** TVD schemes. For simplicity, we discuss the Eq. :eq:`eq_con_steady` in one-dimension. Therefore, the eqution becomes

.. math::
   \frac{d (u \phi)}{d \mathbf{x}} = 0
   :label: eq_con1

Discretization method in the X direction was shown in following figure.


.. plot:: py_matplot/convection_1d.py
   :align: center 

The variables are

.. table:: Summary of Variables.
   :widths: auto
   :align: center 

   =================== ======================================================================
     Sign              Note                                    
   =================== ======================================================================
    :math:`u`            Velocity is assumed to be positive (:math:`u>0`)
    :math:`\phi`         Scalar on center of the control volume
    :math:`x_{i-1/2}`    Left face of the control volume i
    :math:`x_{i+1/2}`    Right face of the control volume i
    :math:`\Delta x_i`   Size of the control volume :math:`\Delta x_i = x_{i+1/2}-x_{i-1/2}`
   =================== ======================================================================


The finite volume method is used to obtain the flux-conseravtive form over a control volume :math:`C_i = [x_{i-1/2}, x_{i-1/2}]`

.. math::
   \frac{ u (\phi_{i+1/2} - \phi_{i-1/2})}{d x} = 0
   :label: eq_dis_con1

In order to predict the varible value at the cell-face, :math:`\phi_{i+1/2}` and :math:`\phi_{i-1/2}`. Many methods could be found in the literature.

First order upwind
-----------------------
The first order upwind (**FOU**) scheme is straight forward way to get the value on the cell-face. When :math:`u >0`, the :math:`\phi_{i+1/2}` is simply equal to :math:`\phi_{i}`. The :math:`\phi_{i-1/2}` is simply equal to :math:`\phi_{i-1}`, respectively. The flow information always comes from upwind direction, e.g.

.. math::
  \phi_{i+1/2} = \Biggl \lbrace{\phi_{i} ,\text{ if } u > 0
                                \atop \
                                \phi_{i+1}, \text{ if } u < 0}


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


This scheme can be deemed as a combination of a dissipative FOU scheme. The first term on the right hand side of Eq. :eq:`eq_k_i12` is FOU. The rest of the right hand side of Eq. :eq:`eq_k_i12` is **high-order correction** term.

It is well known that some of the classical linear high-order advection schemes can be derived by taking different values of :math:`\kappa`, e.g.


.. table:: Summary of :math:`\kappa` scheme
   :widths: auto
   :align: center 

   ===================================== =====================================
        Name                              Value of :math:`\kappa`       
   ===================================== =====================================
    Second Order Upwind (SOU)             -1
    Fromm                                  0
    CUI                                   1/3
    QUICK                                 1/2
    Center Difference Scheme (CDS)         1          
   ===================================== =====================================


.. bibliography:: ../library.bib
   :style: plain
