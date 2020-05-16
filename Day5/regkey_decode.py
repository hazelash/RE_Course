encrypted_key = "\x7F\xAF\x03\xF4\x6C\xDE\x47\xC0\x7E\xAC\x6C\xDC\x52\xED\x56\xF9\x46\xAE\x02\xE6"

count = 0
key1 = 0x9f
key2 = 0x33

decrypted_key = ""

for c in encrypted_key:
	count += 1
	if count % 2 == 0:
		key = key1
	else:
		key = key2
	decrypted_key += chr(ord(c) ^ key )
	
print decrypted_key