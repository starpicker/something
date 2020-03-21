import sys
import qrcode

if len(sys.argv) != 3:
	print("usage: " + sys.argv[0] + " text imagefilename")
	exit()

img = qrcode.make(sys.argv[1])
img.save(open(sys.argv[2], 'wb'))
