#include <cpuid.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

/*
	Resources:
	https://en.wikipedia.org/wiki/CPUID#EAX.3D1:_Processor_Info_and_Feature_Bits
	https://handlers.sans.org/tliston/ThwartingVMDetection_Liston_Skoudis.pdf
	Qemu: https://wiki.archlinux.org/title/QEMU#Preparing_an_Arch_Linux_guest and https://wiki.osdev.org/QEMU_fw_cfg
	VMWare: https://wiki.archlinux.org/title/VMware/Install_Arch_Linux_as_a_guest
	VBox: https://wiki.archlinux.org/title/VirtualBox/Install_Arch_Linux_as_a_guest#Load_the_VirtualBox_kernel_modules
*/

/*
	TODO: (Priority order) 
		- Check current code in variety of VMs (vmware, virtualbox etc.)
		- Check if these modules are present in hosts (make sure guest only)
		- Add more module names
		- Check running process/services
		- Check RAM (point 2 in https://handlers.sans.org/tliston/ThwartingVMDetection_Liston_Skoudis.pdf)
		- Check if 31st bit ecx works on other arch types (e.g arm processors), and if not, support for those?
		- Windows, mac etc. support?
			- Check reg keys (windows)
*/

bool isBitSet(unsigned int num, unsigned int bit)
{
	return 1 == ((num >> bit) & 1);
}

static inline void ___get_cpuid(unsigned int reg, unsigned int* eax, unsigned int* ebx, unsigned int* ecx, unsigned int* edx)
{
    __asm__ volatile("__get_cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "0" (reg));
}
 
/*Vendor-strings from Virtual Machines.*/
#define CPUID_VENDOR_VMWARE			"VMwareVMware"
#define CPUID_VENDOR_KVM			" KVMKVMVKM "
#define CPUID_VENDOR_QEMU			"TCGTCGTCGTCG"
#define CPUID_VENDOR_XENHVM			"XenVMMXenVMM"
#define CPUID_VENDOR_MICROSOFT_HV 	"Microsoft Hv"
#define CPUID_VENDOR_PARALLELS		" lrpepyh vr"
#define CPUID_VENDOR_VIRTUALBOX		"VBoxVBoxVBox"

#define RED     "\033[31m"      
#define GREEN   "\033[32m"     
#define RESET   "\033[0m"

#define PASSED std::cout << GREEN	<< "[Passed]\n" << RESET; ++passes; ++tests
#define FAILED std::cout << RED 	<< "[Failed]\n" << RESET; ++tests

int main(void)
{
	unsigned int passes = 0, tests = 0;
	/* 
		----------------------
		Test for vendor string
		----------------------
	*/ 
	std::cout << "Testing for vendor string ... ";
	unsigned int eax, ebx, ecx, edx;
	__get_cpuid(0, &eax, &ebx, &ecx, &edx);
	std::string vendor = std::string((const char*)&ebx, 4) + std::string((const char*)&edx, 4) +  std::string((const char*)&ecx, 4);

	//Is CPU vendor one of the VM vendors?
	if(vendor == CPUID_VENDOR_VMWARE || vendor == CPUID_VENDOR_KVM || vendor == CPUID_VENDOR_QEMU || vendor == CPUID_VENDOR_XENHVM || vendor == CPUID_VENDOR_MICROSOFT_HV || vendor == CPUID_VENDOR_PARALLELS || vendor == CPUID_VENDOR_VIRTUALBOX)
	{
		PASSED;
	}
	else 
	{
		FAILED;
	}
	/* 
		-----------------
		Test for 31st bit
		-----------------
	*/ 
	std::cout << "Testing for 31st bit in leaf 1, ecx ... ";
	__get_cpuid(1, &eax, &ebx, &ecx, &edx);
	if(isBitSet(ecx, 31))
	{
		PASSED;
	}
	else
	{
		FAILED;
	}
	/*
		-----------------------
		Test for kernel modules
		-----------------------
	*/
	//In order: qemu, vmware, vbox
	std::vector<std::string> moduleNames = 
	{
		"qemu_fw_cfg", "virtio", "virtio_pci", "virtio_blk", "virtio_vlk", "virtio_net", "virtio_ring"
 		"vmw_balloon", "vmw_pvscsi", "vmw_vmci", "vmwgfx", "vmxnet3", "vsock", "vmw_vsock_vmci_transport"
		"vboxguest", "vboxsf", "vboxvideo"
 	};
	std::ifstream modules("/proc/modules");
	std::stringstream buf;
	buf << modules.rdbuf();
	std::string modulesText = buf.str();
	bool hasFoundModule = false;
	std::cout << "Testing for module names ... ";
	for(std::string moduleName : moduleNames)
	{
		if(modulesText.find(moduleName) != std::string::npos)
		{
			hasFoundModule = true;
			break;
		}
	}
	if(hasFoundModule) { PASSED; }
	else { FAILED; }
	/*
		-------
		Outcome
		-------		
	*/
	std::cout << passes << " / " << tests << " passed the VM check\n";
	/*
		----
		TODO
		----
		Other tests that are available (if there are any?)
		Score indicator? (confidence that we're in a VM)
	*/
}
