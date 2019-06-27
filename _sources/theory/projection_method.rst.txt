.. projection_method:

=================
Projection Method
=================



Introduction
=================



Non-incremental projection method
==================================

The usual non-incremental algorithm is proposed by Chorin. The definition of the parameters are shown as follows:

=================  ============================================= 
 Parameters         Illustion
=================  =============================================
Time interval        :math:`[0, T]`
Time subinterval     :math:`N`
Time step            :math:`\Delta t = T / N`
Velocities           :math:`{\tilde{\mathbf{u}}^{n+1}}_{n = 0,...,N}`
Velocities           :math:`{\mathbf{u}^{n+1}}_{n = 0,...,N}`
Pressure             :math:`{\mathbf{p}^{n+1}}_{n = 0,...,N}`
=================  =============================================

The governing equations are:

- Momentum equation: 

.. math::

   \frac{\rho}{\Delta t}(\tilde{\mathbf{u}}^{n+1} - \mathbf{u}^{n+1}) - \mu \Delta \tilde{\mathbf{u}}^{n+1} = \mathbf{u}^{n+1}
  