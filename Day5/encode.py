f = open("c:\windows\system32\calc.exe", 'rb')

ff = open(".\encoded.enc", 'wb')

buf = f.read()

key_begin = 0x80
x = []

for b in buf:
	x.append( ord(b) ^ ( key_begin%0xff ) )
	key_begin += 1
	
print x[0:256]

p = ''

for zz in x:
    p += chr((zz))

ff.write(p)

#print p[0:256]

# for zz in x:
#     p += chr((zz) ^ ( key_begin%0xff ))
#     key_begin += 1
# 
# print p[0:256]

f.close()
ff.close()
