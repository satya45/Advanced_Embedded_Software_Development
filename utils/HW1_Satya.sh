#!/bin/bash
echo -e "Welcome"
echo -e "Generated output.txt file"
echo -e "User Information" >output.txt
echo -e "$USER:\n" >> output.txt

echo -e "OS Type">> output.txt
uname -a >> output.txt
echo -e "\n" >> output.txt

echo -e "OS Distribution">> output.txt
lsb_release -a >> output.txt
echo -e "\n" >> output.txt

echo -e "OS Version" >> output.txt
uname -r >> output.txt 
echo -e "\n" >>output.txt

echo -e "Kernel Version" >> output.txt
uname -r >>output.txt
echo -e "\n" >>output.txt

echo -e "GCC Version" >> output.txt
gcc --version >> output.txt
echo -e "\n" >>output.txt


echo -e "Build Time" >> output.txt
uname -v >>output.txt
echo -e "\n" >>output.txt

echo -e "System Arch Info" >> output.txt
uname -m >> output.txt
echo -e "\n" >>output.txt

echo -e "Info on File System Memory" >> output.txt
cat /proc/meminfo >>output.txt
echo -e "\n" >>output.txt

