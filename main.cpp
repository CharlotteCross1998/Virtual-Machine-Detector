#include <cpuid.h>
#include <iostream>
#include <iomanip>

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
#define CPUID_VENDOR_VMWARE	   "VMwareVMware"
#define CPUID_VENDOR_KVM	   " KVMKVMVKM "
#define CPUID_VENDOR_QEMU	   "TCGTCGTCGTCG"
#define CPUID_VENDOR_XENHVM	   "XenVMMXenVMM"
#define CPUID_VENDOR_MICROSOFT_HV "Microsoft Hv"
#define CPUID_VENDOR_PARALLELS	" lrpepyh vr"
#define CPUID_VENDOR_VIRTUALBOX	"VBoxVBoxVBox"

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
