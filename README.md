# Virtual-Machine-Detector
Simple code to detect whether you're in a virtual machine or not. 

# Usage
```
git clone https://github.com/CharlotteCross1998/Virtual-Machine-Detector.git
make
./vmdetector
```

# Install
```
git clone https://github.com/CharlotteCross1998/Virtual-Machine-Detector.git
make install
```

# Screenshots
![Image of program working in a virtual machine. Displays a fail for the vendor string but a pass for the 31st bit in leaf 1 of ecx](example_in_vm.png)

![Image of program working not in a virtual machine. Displays a fail for the vendor string and a fail for the 31st bit in leaf 1 of ecx](example_not_in_vm.png)
