import matplotlib.pyplot as plt

fig, axes = plt.subplots()
x = [-2, -1, 0, 1, 2]
y = [0,   0, 0, 0, 0]
plt.axis('off')
plt.xlim(-2.5, 2.5)
plt.ylim(-1.5, 1.5)
# points ===================
points, = axes.plot(x, y, "o", markersize=12, color="black")

for v in x:
	di = v - 0
	if di == 0:
		axes.text(v, -0.5, r'$i$',
         	fontsize = 20,
	     	horizontalalignment='center',
         	verticalalignment='center')
	else:
		axes.text(v, -0.5, r'$i' + "%+d" % (v - 0) +'$',
         	fontsize = 20,
	     	horizontalalignment ='center',
         	verticalalignment   ='center')
# lines
line,   = axes.plot([-3, 3], [0,0], "-", linewidth=4, color="black")
#face
for i in range(0, len(x) - 1):
	fx = (x[i] + x[i+1]) * 0.5
	ax = [fx, fx]
	ay = [-0.75, 0.75]
	faces = axes.plot(ax, ay, "--", linewidth = 2, color = "black")

# anotation
# control volume
for i in range(0, len(x) - 2):
    fx = (x[i] + x[i+1]) * 0.5
    fy = 0.5

    axes.annotate("", xy=(fx, fy), 
    	           xytext=(fx + 1.0, fy),
                   arrowprops=dict(arrowstyle="<->"))
    axes.text(fx + 0.5, fy + 0.1, "C", 
    	      fontsize = 20,
	     	  horizontalalignment ='center',
         	  verticalalignment   ='center')

ann = axes.annotate("Control Volume",
                  xy    =(1.0, 0.8), xycoords='data',
                  xytext=(0.8, 1.4), textcoords='data',
                  size  =16, va="center", ha="center",
                  arrowprops=dict(arrowstyle="fancy",
                                  connectionstyle="arc3,rad=-0.3",
                                  fc="grey"), 
                  )

ann = axes.annotate("Face",
                  xy    =(1.5, 0.5), xycoords='data',
                  xytext=(1.8, 0.9), textcoords='data',
                  size  =16, va="center", ha="center",
                  arrowprops=dict(arrowstyle="fancy",
                                  connectionstyle="arc3,rad=-0.3",
                                  fc="grey"), 
                  )

ann = axes.annotate("Center",
                  xy    =(2.0, 0.1), xycoords='data',
                  xytext=(2.2, 0.5), textcoords='data',
                  size  =16, va="center", ha="center",
                  arrowprops=dict(arrowstyle="fancy",
                                  connectionstyle="arc3,rad=-0.3",
                                  fc="grey"), 
                  )
plt.show()