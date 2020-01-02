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