# RE_Course

## 1) download virtual machine image
https://mega.nz/#F!fjgzVApC!20cYsQNgIVAXjjeOnqLxaw

There will be three files.

* Malware-Analysis.7z.001
* Malware-Analysis.7z.002
* Malware-Analysis.7z.003

Once all three files are downloaded, they can be unpacked via 7zip/WinZip/WinRar. 
(right click on .001 file and extract to some folder)

The output is .ova file which can be loaded via VirtualBox. 

## 2) download virtualbox https://www.virtualbox.org/

In case Hyper-V is already active on the machine, it should be diabled before opening the image.
You can refer to this article for how to do it.
https://ugetfix.com/ask/how-to-disable-hyper-v-in-windows-10/

After installing VirtualBox, just double clicking the ova file should open the 'Settings' window for importing the image.
Uncheck the "USB Controller", and hit Import button then the importing starts (takes a few minutes).

![USB Controller](https://snipboard.io/2pDn8V.jpg)

![Import in progress](https://snipboard.io/Cv7Ui6.jpg)

Once importing is done, you can start the machine.

![Start!](https://snipboard.io/dvatbT.jpg)

After initial booting, the popup might show that the system should restart. Restart the machine and once reboot is done, take a snapshot. (also suggesting to remove the warning messages on top of the window) 

![Restart :)](https://snipboard.io/owYrXE.jpg)

## 3) guest plugin (extension pack)
It is advised you install VirtualBox's guest plugin beforehand.

https://download.virtualbox.org/virtualbox/6.1.2/Oracle_VM_VirtualBox_Extension_Pack-6.1.2.vbox-extpack

## ETC.....
Normally there should be no problem, but in case there's any error related to 'VT-x is not enabled', please follow the instructions here.
https://2nwiki.2n.cz/pages/viewpage.action?pageId=75202968

