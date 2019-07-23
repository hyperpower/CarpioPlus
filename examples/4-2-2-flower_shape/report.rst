Problem Description
===================

This example solves Poisson's equation

.. math::
   
   \nabla \cdot \beta \nabla \phi = 7 r^2 cos 3 \theta

where, :math:`\beta = 1` in present case.

The domain is defined by 

.. math::
   
   \Omega = {(r, \theta) : r \le 0.30 + 0.15 cos 6 \theta}


Dirichlet boundary conditions on :math:`\partial \Omega` are specified by the exact solution,

.. math::

   \varphi (r, \theta) = r^4 cos3 \theta

Error Definition
======================
Volume-weighted norm of a variable :math:`e`,

.. math::
   
   ||e||^{\Lambda, \Omega_k}_{p} = (\sum_{\Omega_k} |e|^p \Lambda V)^{1/p}

where :math:`V = \Delta x \Delta y` is the cell volume. :math:`\Lambda` is volume fraction for cut cell. 

The rate of convergence between two norms, :math:`e_1` and :math:`e_2`, with two different grid spacings :math:`h_1` and :math:`h2`, as

.. math::
   
   r = log(\frac{e_1}{e_2}) / log(\frac{h_1}{h_2})

Thus with :math:`h_1 < h_2`, a rate of :math:`r=1` for the two errors :math:`e_1` and :math:`e_2` indicates a first-order accurate method.


Masked Cell Solution
======================


