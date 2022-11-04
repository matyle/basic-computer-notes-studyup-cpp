

- State Transfer
  - tansfor memory



- **Replicated state machine**
  - transfer Primary backup operations 

 





OutPut Rule

当Primary收到指令之后，不会立即产生output给客户端

而是备份指令到从机 并收到从机ack后，再发送成功指令给客户端



- Input to the primary output from the backup  





Test and Set server？





