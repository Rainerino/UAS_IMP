#!/usr/bin/env python
from decimal import Decimal
f = open("GPSExifTags.txt")
for i, line in enumerate(f):
	if i == 0:
		a = Decimal(line[2])
		print "%.10f" % a
	elif i == 1:
		print "nop"
f.close()
