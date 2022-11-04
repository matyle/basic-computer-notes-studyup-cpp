## 请求和相应


- 请求报文
  - 请求行（请求方法，请求URI，协议版本
  - 请求首部
  - 内容实体
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-07-17-02-20.png
  width=450px>
  </center>
  
- 响应报文
  - 响应首部（版本，状态200 ok）
  - 响应首部
  - 主体
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-07-17-02-53.png
  width=450px>
  </center>
  


## HTTP不保存状态
- 无状态协议
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-07-17-04-43.png
  width=450px>
  </center>
  
  - 为了能够保存状态，引入了Cookie技术。


## 告知服务器意图的方法

- GET

- POST 传输实体主体
- PUT 传输文件
- HEAD 获得报文首部，用于确认URI有效性及资源更新的日期时间
- DELETE 删除文件
- OPTIONS：用来查询对请求URI指定的资源支持的方法
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-07-17-24-11.png
width=450px>
</center>


## 持久连接节省通信量
- 持久连接：一次TCP连接就能发送所有资源，


### 管线化
- 不需等待收到响应，就可以直接发下一个请求。

### Cookie保存状态
- 如果让服务器保存状态，压力太大。因此使用客户端保存Cookie
  - 服务器的响应报文里面使用一个Set-Cookie的首部字段信息，通知客户端保存Cookie。