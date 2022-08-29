## ifconfig



## 路由和网关设置

```sh
route add
route del

```



## DNS客户端配置

### /etc/host



### /etc/resolv.conf

```sh
nameserver 127.0.0.53 //ip主机地址
options edns0 trust-ad
search localdomain #serch后面跟域名
```



## 网络测试工具

### ping



### host

查询DNS记录

```sh
host www.baidu.com
www.baidu.com is an alias for www.a.shifen.com.
www.a.shifen.com has address 110.242.68.4
www.a.shifen.com has address 110.242.68.3
```

### traceroute

