==========
ENO
==========

Intorduction
==============


ENO-Roe
==============


algotithm

Step 1 Compute the divided difference table of :math:`f`
-----------------------------------------------------------
and identify

.. math::
   H[x_{l-1/2}, x_{l+1/2}] = f[u(x_l)]

.. math::
   H[x_{l-1/2}, x_{l+1/2}, ... , x_{l+k+1/2}] = \frac{1}{1 + k} f[u(x_l), ..., u(x_{l+k})]

.. math::
   k = 1, 2, ... , r

if :math:`k = 1`

.. math::
   H[x_{l-1/2}, x_{l+1/2}, x_{l+1+1/2}] &= \frac{1}{1 + 1} f[u(x_l), u(x_{l + 1})] \\
   H[x_{l-1/2}, x_{l+1/2}, x_{l+3/2}]   &= \frac{1}{2} f[u(x_l), u(x_{l + 1})] \\
                                        &= \frac{1}{2} \frac{u(x_{l + 1}) - u(x_l)}{x_{l + 1} - x_l}

if :math:`k = 2`

.. math::
   H[x_{l-1/2}, x_{l+1/2}, x_{l+1+1/2}, x_{l+2+1/2}] &= \frac{1}{1 + 2} f[u(x_l), u(x_{l + 1}), u(x_{l+2})] \\
   H[x_{l-1/2}, x_{l+1/2}, x_{l+3/2}, x_{l+5+1/2}]   &= \frac{1}{3} f[u(x_l), u(x_{l + 1})] \\
                                        &= \frac{1}{3} \frac{f[u(x_{l + 1}),u(x_{l + 2})] - f[u(x_{l}),u(x_{l+1})]}{x_{l+2} - x_l}
