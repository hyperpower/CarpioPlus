.. non_incremental

===========================
Non-incremental Projection
===========================

Introduction
========================
This is the simplest pressure-corrction scheme has originally been proposed by [C1]_. Using the implicit Euler time stepping, the algorithm is as follows: Set :math:`u^0 = u_0`, then for :math:`k \ge 0` compute :math:`(\hat{u}^{k+1}, u^{k+1}, p^{k+1})` by solving

.. math::
   \frac{(\tilde{u} - u^{k})}{\Delta t} + u^k \cdot (\nabla u^k) = \frac{1}{Re} \nabla^2 \tilde{u}    

.. math::
   \frac{(u^{k+1} - \tilde{u})}{\Delta t} + u^k \cdot (\nabla u^k) = \frac{1}{Re} \nabla^2 \tilde{u}

   
Take the divergence operator on both side of Eq. 

.. math::
   \frac{1}{\Delta t}(\nabla \cdot u^{k+1} - \nabla \cdot \tilde{u}) &= -\nabla \cdot \nabla p^{k+1}, \\
   - \frac{1}{\Delta t} \nabla \cdot \tilde{u} &= -\nabla \cdot \nabla p^{k+1}, \\
   \frac{1}{\Delta t} \nabla \cdot \tilde{u} &= \nabla \cdot \nabla p^{k+1}

Solve above equation, :math:`p^{k+1}` can be obtained. Then,

.. math::
   u^{k+1} = \tilde{u} - \Delta t \nabla p^{k+1} 

Calculation of an Auxiliary Veloticy Field
===========================================

In order to calculate an auxiliary velocity :math:`\tilde{u}`, we follow the scheme in [C2]_. The scheme is one of the splitting and fractional-step methods, which is a variant of the ADI method.

.. math::
   \frac{(u^* - u^{k})}{\Delta t} + u^k (\frac{\partial u^k}{\partial x}) = \frac{1}{Re} \frac{\partial^2 u^*}{\partial x^{2}} 


.. math::
   \frac{(u^{**} - u^{*})}{\Delta t} + u^* (\frac{\partial u^*}{\partial y}) = \frac{1}{Re} \frac{\partial^2 u^{**}}{\partial y^2}

.. math::
   \frac{(\tilde{u} - u^{**})}{\Delta t} + u^{**} (\frac{\partial u^{**}}{\partial z}) = \frac{1}{Re} \frac{\partial^2 \tilde{u}}{\partial z^2}

Note that Euquations are of implicit form for each intermediate field. These schems are unconditionally stable, but total stability condition is not strictly derived. 

Boundary condition for :math:`\tilde{u}`
---------------------------------------------
Since,

.. math::
   \tilde{u} = u^{k + 1} + \Delta t \nabla p^n

For stationary wall :math:`u^{k + 1} = 0`, Then, 

.. math::
   \tilde{u} = \Delta t \nabla p^n

For the direction normal to the wall, say :math:`x`,

.. math::
   \tilde{u} = \frac{\Delta t}{\Delta x} (P_g^n - P_b^n)

where, :math:`P_g^n` is ghost node,  :math:`P_b^n` is boundary node.   


.. [C1] A.J. Chorin, Numerical solution of the Navier-Stokes equation. Math. Comput. 22 (1968) 745-762 
.. [C2] K. Goda, A multistep technique with implicit difference schemes for calculation two- or three-dimensional cavity flows, JCP, 30 (1979) 76-95
