## linux内核认识

### 硬件信息查看

> 查看cpu信息
>
> `lscpu`
>
> `cat /proc/cpuinfo`



> 查看内存信息
>
> `cat /proc/meminfo`
>
> `lsmem`
>
> 内存硬件
>
> dmidecode -t memory



> 磁盘
>
> `lsblk`
>
> `fdisk -l`



> 网卡
>
> `lspci`
>
> `ifconfig`







`lscpu`

![image-20211106134556128](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211106134556128.png)





### 目录

- `arch`目录：存放cpu体系结构之间有差异的那些功能模块，会存放到该目录

![image-20211106125910740](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211106125910740.png)

```javascript
解包：tar xvf FileName.tar
打包：tar cvf FileName.tar DirName
```



- Docmentation ：文档
- init 启动相关代码
- Block 块设备代码
- driver 代码驱动
- ipc 进程通信
- fs 文件系统
- kernel代码：核心代码 如进程调度
- mm ：内存管理相关代码

### 编译过程

```bash

https://mirrors.edge.kernel.org/pub/linux/kernel/v4.x/linux-4.9.229.tar.gz
export ARCH=x86

# 生成菜单 配置board 
make x86_64_defconfig 

# 配置内核
make menuconfig

# 编译
make 
# 编译完成在 arch/x86_64/boot/bzImage
```



- busybox

  busybox简介

  busybox是一个集成了一百多个最常用linux命令和工具的软件,他甚至还集成了一个http服务器和一个telnet服务器,而所有这一切功能却只有区区1M左右的大小.我们平时用的那些linux命令就好比是分立式的电子元件,而busybox就好比是一个集成电路,把常用的工具和命令集成压缩在一个可执行文件里,功能基本不变,而大小却小很多倍,在嵌入式linux应用中,busybox有非常广的应用,另外,大多数linux发行版的安装程序中都有busybox的身影,安装linux的时候案ctrl+alt+F2就能得到一个控制台,而这个控制台中的所有命令都是指向busybox的链接.
  Busybox的小身材大作用的特性,给制作一张软盘的linux带来了及大方便.

  ```bash
  #下载源码
  wget https://busybox.net/downloads/busybox-1.30.0.tar.bz2
  
  
  #配置源码
  make menuconfig
  
  
  # 编译安装
  make && make install 
  
  w
  # 编译完成后的busybox就安装在源码根目录下的_install目录了，我们进入_install目录，补充一些必要的文件或目录，相关的shell命令如下
  
  [root@tanqi _install]# mkdir etc dev mnt
  [root@tanqi _install]# mkdir -p proc sys tmp mnt
  [root@tanqi _install]# ls
  bin  dev  etc  linuxrc  mnt  proc  sbin  sys  tmp  usr
  
  ## 创建
   mkdir -p etc/init.d
   
   //创建文件系统表
   # vim etc/fstab
  proc        /proc           proc         defaults        0        0
  tmpfs       /tmp            tmpfs    　　defaults        0        0
  sysfs       /sys            sysfs        defaults        0        0 
  
  
  vim etc/init.d/rcS
  
  echo -e "Welcome to tinyLinux"
  /bin/mount -a
  echo -e "Remounting the root filesystem"
  mount  -o  remount,rw  /
  mkdir -p /dev/pts
  mount -t devpts devpts /dev/pts
  echo /sbin/mdev > /proc/sys/kernel/hotplug
  mdev -s
  
  
  # vim etc/inittab
  ::sysinit:/etc/init.d/rcS
  ::respawn:-/bin/sh
  ::askfirst:-/bin/sh
  ::ctrlaltdel:/bin/umount -a -r
  
  cd dev
  
  [root@tanqi dev]# mknod console c 5 1
  [root@tanqi dev]# mknod null c 1 3
  [root@tanqi dev]# mknod tty1 c 4 1
  
  
  #!/bin/bash
  rm -rf rootfs.ext3
  rm -rf fs
  dd if=/dev/zero of=./rootfs.ext3 bs=1M count=32
  mkfs.ext3 rootfs.ext3
  mkdir fs
  mount -o loop rootfs.ext3 ./fs
  cp -rf ./_install/* ./fs
  umount ./fs
  gzip --best -c rootfs.ext3 > rootfs.img.gz
  
  
  qemu-system-x86_64 -kernel ./linux-4.9.229/arch/x86_64/boot/bzImage -initrd ./busybox-1.30.0/rootfs.img.gz -append "root=/dev/ram init=/linuxrc" -serial file:output.txt
  
   qemu-system-x86_64
  ```

  



