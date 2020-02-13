# Day 2 Homeworks

## Easy 

* Edit these fields in hello.exe and try executing it. What happens?

  - MZ/PE signatures 
  - TimeDateStamp
  - Section names
  - Section characteristics

Feel free to try modifying other fields too :)

* Compare hello.exe (from Day1) and hello_na.exe (from Day2)

  - What is the offset that’s different?
  - Where does this field belong to within the PE header? 
  - How different is the result of execution caused by this modification?


## Advanced

* Use hello.exe as target program
* Use distorm to disassemble the program
https://github.com/gdabah/distorm
* Use pefile to parse the PE format and get to the code section
https://github.com/erocarrera/pefile
(Or if you want more challenge, write your own code for this - Not the whole parser but just getting to the code section!)
* Find out the 10 most used instructions within hello.exe’s code

