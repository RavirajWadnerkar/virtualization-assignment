# CMPE 283 Assignment 2 

- The assignment is performed on Google Cloud VM supporting nested virtualiztion(`--enable-nested-virtualization`).
- Configuration: 
  - Machine Type: **n2-standard-8**
  - CPU platform: **Intel Cascade Lake**
  - Zone: **us-central1-a**

<br />

- Assignment 2 changes the behavior of the **cpuid** instruction for the following cases:
  - CPUID leaf node(`%eax=0x4fffffff`)
    - Return the total number of exits for all types in **%eax**.
  - CPUID lead node(`%eax=0x4ffffffe`) 
    - Return the high 32 bits of the total time spent processing all exits in **%ebx**.
    - Return the low 32 bits of the total time spent processing all exits in **%ecx**.
    - **%ebx** and **%ecx** return values are measured in processor cycles, across all vCPUs.

<br />

## Question 1:
  - **Raviraj Wadnerkar** and **Ashish Veda**

  - Raviraj

  Created the kernel
  In the VM, installed the necessary kernel modules.
  Comprehended and researched the assignment's code.
  The code has been updated to reflect the changes that were discussed.
  README.md has been updated and documentation has been created.

  - Ashish

  Created the kernel
  Reviewed the canvas lecture and understood the steps to be taken
  Did some research on CPUID instructions and CPU leaf nodes.
  Researched the commands to execute it properly.
  Recognized where and what changes to make in order to complete the assignment.

<br />
