pw2 = "L00k_At_M3_Carefu11y"

count = 0

key1 = 0x9f
key2 = 0x33

for p in pw2:
	count += 1
	if count % 2 == 0:
		key = key1
	else:
		key = key2
		
		
	print ("%02x" % (ord(p) ^ key ))
	
