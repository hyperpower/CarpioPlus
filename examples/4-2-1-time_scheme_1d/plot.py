import os


def process(n):
	os.system("mkdir ./fig/" + n)
	arr = os.listdir('./fig')
	for name in arr:
		if n in name and os.path.isfile("./fig/" + name):
			sp = name.split("_")
			os.system("mv ./fig/" + name + " ./fig/" + n +"/%03d.png" % int(sp[2]))
	os.system("convert -delay 20 -loop 0 ./fig/"+ n + "/*.png ./fig/"+ n +"/" + n + ".gif")
	os.system("mv ./fig/%s/%s.gif ./fig/" % (n,n))
	os.system("rm -r ./fig/%s" % n)

if __name__ == '__main__':
	process("explicit")
	process("implicit")
	process("CN")
