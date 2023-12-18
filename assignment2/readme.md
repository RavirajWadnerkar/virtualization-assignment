# CMPE 283 Assignment 2 

#### **Perfromed the step to generate the rsa key-pair**

![image](https://github.com/RavirajWadnerkar/virtualization-assignment/assets/47893967/293eebc0-bd97-48b9-a3d4-3f6d4a308f4c)

**Code**
```
gcloud compute instances create cmpe283-vm-us --enable-nested-virtualization --zone=us-central1-a --machine-type=n2-standard-8 --network-interface=network-tier=PREMIUM,subnet=default --create-disk=auto-delete=yes,boot=yes,device-name=instance-1,image=projects/ubuntu-os-cloud/global/images/ubuntu-2004-focal-v20220204,mode=rw,size=200,type=projects/cmpe-283-f23/zones/us-central1-a/diskTypes/pd-ssd --metadata=ssh-keys="SHA256:vi3yBWRq56nOqMVoNi4HOIrEIuoN119ZSSOf9gzv+Xk raviraj wadnerkar@DESKTOP-7NQO4PH"
```

- The assignment is performed on Google Cloud VM supporting nested virtualiztion(`--enable-nested-virtualization`).
- Configuration: 
  - Machine Type: **n2-standard-8**
  - CPU platform: **Intel Cascade Lake**
  - Zone: **us-central1-a**

![image](https://github.com/RavirajWadnerkar/virtualization-assignment/assets/47893967/64463f06-3794-4d42-8dd0-4571f423af46)


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

  
  The output is in [this repository](https://github.com/RavirajWadnerkar/linux)
  <br />



  Run this code in the SSH

  ```
  git clone https://github.com/RavirajWadnerkar/linux.git
  ```

  To review all the files inside boot
  
  ```
  ls /boot/
  ```
  
  ```
  sudo apt install make
  ```
  
  ```
  cp /boot/config-5.11.0-1029-gcp .config
  ```
  
  ```
  make oldconfig
  ```
  
  ```
  sudo apt install gcc
  sudo apt-get update
  sudo apt-get upgrade
  sudo apt-get install bison
  ```
  
  ```
  make prepare
  ```
  
  ```
  make -j 8 modules
  ```
  
  ```
  scripts/config --disable SYSTEM_TRUSTED_KEYS
  scripts/config --disable SYSTEM_REVOCATION_KEYS
  ```
  
  ```
  cd arch
  cd x86
  cd kvm
  vi cpuid.c
  vi vmx/vmx.c
  cd ..
  make modules
  sudo make -j 8 modules
  ```
