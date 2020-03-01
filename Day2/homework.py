import pefile
import distorm3
from distorm3 import Decode, Decode16Bits, Decode32Bits, Decode64Bits

filename = ".\hello_na.exe"
pe = pefile.PE(filename, fast_load = True)

# just to check if the file is parsed as expected
# print (pe.FILE_HEADER.NumberOfSections)
# print ("%x" % pe.sections[0].PointerToRawData)
# print ("%x" % pe.sections[0].SizeOfRawData)

start = pe.sections[0].PointerToRawData
end = pe.sections[0].PointerToRawData + pe.sections[0].SizeOfRawData

buf_code_section =  pe.__data__[start:end] 

# buf = ""
# count = 0
# for b in buf_code_section:
# 	buf += "%02x " % (b)
# 	count += 1
# 	
# 	if (count > 0 and count % 16 == 0):
# 		buf += "\n"
# 		
# print (buf)

# example can be found - https://github.com/gdabah/distorm/wiki/Python

explode = (distorm3.Decode(0x00, buf_code_section, Decode32Bits))

# list_inst = []
# list_uniq_inst = []
num_insts = 0
num_uniq_insts = 0

# e is Tuple type 
# (0, 1, 'PUSH EBP', '55')
# (1, 2, 'MOV EBP, ESP', '8bec')
# (3, 3, 'SUB ESP, 0x24', '83ec24')

# for e in explode:
# 	inst = e[2].split(' ')[0]
# 	binary = e[3]
# 	
# 	if (inst == "ADD" and binary == "0000"):
# 		# ignore incorrect interpretation of padding 
# 		# 'ADD [EAX], AL', '0000'
# 		continue 
# 
# 	list_inst.append(inst)
# 	num_insts += 1
# 	
# 	if ( inst not in list_uniq_inst ):
# 		list_uniq_inst.append(inst)
# 		num_uniq_insts += 1
# 	
# print ("%d instructions in this code" % num_insts)
# print ("%d unique instructions in this code" % num_uniq_insts)
# print (list_inst)
# print (list_uniq_inst)

# dictionary type to keep the occurrences per each instruction
# { 'INST' : COUNT }
dict_occur = {}
for e in explode:
	inst = e[2].split(' ')[0]
	binary = e[3]
	
	if (inst == "ADD" and binary == "0000"):
		# ignore incorrect interpretation of padding 
		# 'ADD [EAX], AL', '0000'
		continue 

	num_insts += 1 # count only correct instructions
	
	if inst not in (dict_occur.keys()):
		dict_occur.update({inst: 1}) # this starts from 1!!!
		num_uniq_insts += 1
	else:
		current_count = dict_occur[inst]
		current_count += 1
		dict_occur.pop(inst)
		dict_occur.update({inst: current_count})
		
print ("%d instructions in this code" % num_insts)		
print ("%d unique instructions in this code" % num_uniq_insts)		

#for i in dict_occur:
#	print (i, dict_occur[i])

# reference - https://stackoverflow.com/questions/613183/how-do-i-sort-a-dictionary-by-value
sorted_dict = (sorted(dict_occur.items(), key = lambda d:(d[1], d[0]), reverse=True))
# result of sorted is list 

for i in sorted_dict:
	print ("%s\t%d (%.2lf %%)" % (i[0], i[1], i[1]*100.0/num_insts))

