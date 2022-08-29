### 创建文件系统

`fdisk`

```sh
fdisk -l
Disk /dev/sda: 64 GiB, 68719476736 bytes, 134217728 sectors
...

Disk /dev/sdb: 1 GiB, 1073741824 bytes, 2097152 sectors  //第二块硬盘




//加一块硬盘
Disk /dev/sdb: 1 GiB, 1073741824 bytes, 2097152 sectors
//分区
fdisk /dev/sdb

Device does not contain a recognized partition table.
Created a new DOS disklabel with disk identifier 0x178dc332.

Command (m for help): n n表示new
Partition type 分区类型
   p   primary (0 primary, 0 extended, 4 free)
   e   extended (container for logical partitions)
Select (default p): p 主分区
Partition number (1-4, default 1): 1 分区1
First sector (2048-2097151, default 2048): 2048
Last sector, +sectors or +size{K,M,G,T,P} (2048-2097151, default 2097151): 

Created a new partition 1 of type 'Linux' and of size 1023 MiB.
 
Command (m for help): w 表示写入


//接着是 格式化
mkfs-t ext3 /dev/sdb1


挂载
```

![image-20210802153044715](文件系统.assets/image-20210802153044715.png)

- 分区操作
- 

### 文件挂载

`mount`指令

`sudo mount DEVICE MOUNT_POINT` device指设备即刚刚分区的硬盘sdb1，挂载点只能是目录



解除挂载` umount`

```sh
mkdir newDisk
root@ubuntu:~# mkdir newDisk
root@ubuntu:~# mount /dev/sdb1 newDisk //将磁盘挂载到了newDisk目录

mount 

/dev/sdb1 on /root/newDisk type ext3 (rw,relatime,data=ordered)

umount /dev/sdb1

f
```

### 设置自动挂载 /etc/fstab

`echo "dev/sdb1 /root/newDisk ext3 default 0 0">>/etc/fstab`



- 磁盘检查` fsck，badblocks`







## linux逻辑卷

为什么需要逻辑卷？因为fdisk无法对目录扩容

新增加磁盘这叫做物理设备，如/dev/sdb，物理设备是无法直接使用的，需要进行文件系统建设和格式化，并且挂载到特定的目录下方可使用

接下来，如果准备继续对这个目录扩容，直接用fdisk是做不到的，这就需要lvm逻辑卷管理了，也就是处于裸设备和文件系统之间一层桥梁的作用。其步骤很简单，新增设备，如/dev/sdd，首先进行分区操作，利用fdisk进行操作，选择主分区，每个磁盘有四个分区，这里可以自己写分区编号，分区完成以后，利用lvm进行物理卷生成，卷组创建，以及逻辑卷创建，最终扩容的，是针对这个逻辑卷！



物理卷 PV 物理磁盘分区 /dev/sdb1

卷组 VG ：PV的集合

逻辑卷LV：PV中画出来的一块逻辑磁盘





#### 创建逻辑卷

- 创建物理卷 pvcreate,pvdisplay

  ```shel
  首先将硬盘分区
  fdisk /dev/sdc
  command:n 表示new
  选择p主分区还是逻辑分区
  p
  选择开始段
  选择结尾段 +300M 表示这个分区大小300
  w 写入
  Device     Boot   Start     End Sectors  Size Id Type
  /dev/sdc1          2048  616447  614400  300M 83 Linux
  /dev/sdc2        616448 1230847  614400  300M 83 Linux
  /dev/sdc3       1230848 2097151  866304  423M 83 Linux
  
  可以看到各个分区ID为83 需要改为8e   t   change a partition type
  Command (m for help): t
  Partition number (1-3, default 3): 1
  Partition type (type L to list all types): 8e L可以查看 8e  Linux LVM 
  
  Device     Boot   Start     End Sectors  Size Id Type
  /dev/sdc1          2048  616447  614400  300M 8e Linux LVM	
  //接着创建物理卷
  pvcreate /dev/sdc1
  pvcreate /dev/sdc2
  pvcreate /dev/sdc3
  
  创建卷组
  vgcreate First_VG /dev/sdc1 /dev/sdc2
  
  
  创建逻辑卷
  lvcreate -L 100M -n Fist_LV First_VG
  
  //创建文件系统
  root@ubuntu:/home/matytan# mkfs.ext3 /dev/First_VG/Fist_LV
  
  
  //挂载
  mount /dev/First_VG/Fist_LV /root/newLv
  ```

- 创建卷组 `vgcreate VG_NAME`
- 扩容卷组` vgextend`
- 创建逻辑卷：`lvcreate,lvdisplay`
  - `lvcreate -L 100M -n Fist_LV First_VG`





## 硬链接和软连接

硬链接：实际链接。每个文件有一个inode，多个文件名指向同一个索引节点，这种链接叫做硬链接（引用计数）

创建链接：`ln hardfile hardfile_hlink` ，

ls -li 可以显示inode，可以发现两个文件inode相同

```she
 ls -li
1967213 -rw-r--r-- 2 root root 0 Aug  3 14:57 hardfile
1967213 -rw-r--r-- 2 root root 0 Aug  3 14:57 hardfile_hlink
```



软链接：符号连接（类似于快捷方式），是另外一个文件指向一个文件

`ln -s softfile soft_slink`

```sh
root@ubuntu:~/soft# ls -li
total 0
2490377 -rw-r--r-- 1 root root 0 Aug  3 15:00 softfile
2490378 lrwxrwxrwx 1 root root 8 Aug  3 15:00 soft_slink -> softfile
```



