### **使用 [docker-connector](https://github.com/wenjunxiao/mac-docker-connector)**解决网络问题

Mac 通过 brew 安装 docker-connector

```
brew install wenjunxiao/brew/docker-connector
```

执行以下命令把 docker 的所有 `bridge` 网络都添加到路由中(拷贝已下指令)

```
docker network ls --filter driver=bridge --format "{{.ID}}" | xargs docker network inspect --format "route {{range .IPAM.Config}}{{.Subnet}}{{end}}" >> /usr/local/etc/docker-connector.conf
```

> 可以手动修改 `/usr/local/etc/docker-connector.conf` 文件中的路由，格式是
>
> 这个文件之前是注释了的需要手动修改
>
> ```
> route 172.17.0.0/16
> ```
>
> 路由的子网决定了你能访问那些容器

配置完成，直接启动服务（需要 `sudo`，路由配置启动之后仍然可以修改，并且无需重启服务立即生效）

```
sudo brew services start docker-connector
Copy
```

然后使用以下命令在 docker 端运行 wenjunxiao/mac-docker-connector，需要使用 `host` 网络，并且允许 `NET_ADMIN`

```
docker run -it -d --restart always --net host --cap-add NET_ADMIN --name connector wenjunxiao/mac-docker-connector
Copy
```

如果需要将对应的网络共享给其他人访问，需要修改配置文件 `/usr/local/etc/docker-connector.conf`
在 `route` 后增加 `expose`

```
route 172.17.0.0/16 expose
Copy
```

并增加监听端口、token 和 ip 分配

```
expose 0.0.0.0:2512
token user1 192.168.251.10
token user2 192.168.251.11
Copy
```



如果是其他机器要安装 docker-accessor，不同系统的安装、使用方法不同（即不是宿主机器）

- Mac

  ```
  brew install wenjunxiao/brew/docker-accessor
  sudo docker-accessor -remote 192.168.1.100:2512 -token user1
  Copy
  ```

- Windows

  从 [OpenVPN](https://community.openvpn.net/openvpn/wiki/ManagingWindowsTAPDrivers) 下载并安装 tap 驱动 [tap-windows](http://build.openvpn.net/downloads/releases/)

  > 下载地址：http://build.openvpn.net/downloads/releases/latest/tap-windows-latest-stable.exe

  从 [mac-docker-connector/releases](https://github.com/wenjunxiao/mac-docker-connector/releases) 下载最新适合当前操作系统的 `docker-accessor-win-i686.zip` 或 `docker-accessor-win-x86_64.zip` 解压即可

  以**管理员权限**打开控制台

  ```
  D:\Downloads\>.\docker-accessor.exe -remote 192.168.1.100:2512 -token user2
  Copy
  ```

- Linux

  直接从 [mac-docker-connector](https://github.com/wenjunxiao/mac-docker-connector/releases) 下载最新的 `docker-accessor-linux.tar.gz` 解压即可

  ```
  curl -L -o- https://github.com/wenjunxiao/mac-docker-connector/releases/download/v2.0/docker-accessor-linux.tar.gz | tar -xzf - -C /usr/local/bin
  sudo docker-accessor -remote 192.168.1.100:2512 -token user1
  Copy
  ```

如果和本地的 docker 网络发生了冲突，可以使用 `-exclude` 来避免

```
docker-accessor -remote 192.168.1.100:2512 -token my-token -exclude 172.1.0.0/24,172.2.0.0/24
```



