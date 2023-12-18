# CMPE 283 Assignment 3

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

## Question 1:

- **Raviraj Wadnerkar** and **Ashish Veda**

- Satya Ashish Veda: Successfully generated the kernel. Conducted research on CPUID instructions and CPU leaf nodes. Identifying the specific modifications required for completing the assignment and made the necessary changes in cpuid.c and vmx.c file. Updated the README.md with comprehensive information and created documentation for future reference.

- Raviraj Wadnerkar: Accomplished the creation of the kernel and proceeded to install essential kernel modules within the VM. Gained a solid understanding of the assignment's code. Installed the nested VM and done the testing through CPUID.

  The output is in [this repository](https://github.com/RavirajWadnerkar/linux)
  <br />

## Question 2: (Steps used to complete the assignment)

Run this code in the VM

```
git clone https://github.com/RavirajWadnerkar/linux.git
```

Get the correct Linux version

```
ls /boot/
```

Copy the config file to root directory

```
cp /boot/config-5.11.0-1029-gcp .config
```

```
sudo apt install make
```

```
make oldconfig
```

Install all the required libraries

```
sudo apt install gcc
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install bison
sudo apt-get install qemu-kvm qemu virt-manager virt-viewer libvirt-daemon
```

Make the modules parallely using 8 cores in our machine
Update the cpuid.c and vmx.c files and make the modules

```
cd arch
cd x86
cd kvm
vi cpuid.c
vi vmx/vmx.c
cd ..
make modules
sudo make -j 8 modules
sudo make install
```

Create a user-data file and save the nested VM login config

```
#cloud-config
chpasswd: { expire: False }
ssh_pwauth: True
```

Download and Install the nested VM

```
wget https://cloud-images.ubuntu.com/bionic/current/bionic-server-cloudimg-amd64.img
sudo virt-customize -a bionic-server-cloudimg-amd64.img --root-password password:newpass
sudo virt-customize -a bionic-server-cloudimg-amd64.img --uninstall cloud-init
cloud-localds user-data.img user-data
sudo qemu-system-x86_64 -enable-kvm -hda bionic-server-cloudimg-amd64.img -drive "file=user-data.img,format=raw" -m 512 -curses -nographic
```

- username: root
- password: newpass

  Inside the Nested VM Install cpuid to test

  ```
  sudo apt-get update
  sudo apt-get install cpuid
  ```

  Test using CPUID

  ```
  cpuid -l 0x4FFFFFFD
  cpuid -l 0x4FFFFFFC
  ```

## Question 3:

Comment on the frequency of exits – does the number of exits increase at a stable rate? Or are there
more exits performed during certain VM operations? Approximately how many exits does a full VM
boot entail?

The number of exits increases at a stable rates as these exits are called fairly regularly

## Question 4:

Of the exit types defined in the SDM, which are the most frequent? Least ?

Most frequent exit is
Exit #30 - IO_INSTRUCTION

Least frequest exit is
Exit #0 - EXCEPTION_NMI
