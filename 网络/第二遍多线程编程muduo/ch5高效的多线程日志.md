# 高效的多线程日志
日志有两个意思：
- 诊断日志：diagnostic log：如log4j
- 交易日志

- 诊断日志是服务端编程必备的，是事故调查时的唯一线索 ，生产环境中应该做到Log Everything All the time,日志通常要记录
  - 收到的每条内部消息的id（包括关键字段，长度，hash等）
  - 收到的每条外部消息的全文
  - 发出每条消息的全文，每个消息有全局id
  - 关键内部状态的变更，等等

- 如何将多个日志文件从多个前端高效的传输到后端？是一个多生产者-单消费者问题
  ```cpp
  //C/java的printf(format,...)风格
  log_info("Received %d bytes from %s",len,getClientName().c_str());
  //C++ stream <<风格，例如
  LOG_INFO << "Received" <<len <<"bytes from" << getClientName();
  ```

- 尽量采用C++ stream风格，写起来更自然，不必费心保持格式字符串与参数类型的一致性


## 功能需求

1. 日志消息有多种级别（level）（必要）如TRACE，DEBUG，INFO，WARN，ERROR，FATAL
2. 日志消息可能有多个目的地appender，如文件 socket（不必要）
3. 日志消息格式可配置 （不必要）
4. 可以设置运行时的过滤器（不必要）

- 调整日志输出级别
- 对于分布式系统中的服务进程，日志目的地只有一个：本地文件
- 以本地文件为日志的目的地，日志文件的滚动时必须的，可以简化日志归档。rolling条件大小通常是文件大小和时间。
- 一个典型的日志名由以下几部分组成
  - `logfile_test.20120603-144022.hostname.365.log`
  - 第1部分logfile_test是进程的名字，通常是main()参数中argv[0]的名字，必要时可以加版本
  - 第2部分是20120603-144022 文件创建时间
  - 第3部分是hostname机器名称。便于把日志拷贝到别的机器上也能追溯其来源
  - 第4部分是进程id，如果一个成员一秒内反复重启，每次会生成不同的日志文件
  - 第5部分是统一后缀.log

## 多线程异步日志
- 线程安全
- 作者认为一个进程最好只写一个日志文件，用一个背景线程负责收集日志消息，并写入文件。其他线程称这个线程为日志线程
- 需要一个队列将日志前端的数据送到后端（日志线程），不必是那个线程的阻塞队列，因为并不是每产生一次就通知接收方
- 双缓冲技术，准备两块buffer：A和B。前端负责往bufferA填数据，后端负责将bufferB的数据写入文件。当bufferA写满后交换A和B，实际上现在B满了因此后端开始将B的写入文件。如此往复
- 即使A未满，日志库也会每3秒执行一次上述交换写入操作
  ```cpp
    typedef std::vector<std::unique_ptr<LargeBuffer>> BufferVecotr; //具有移动语意，自动管理对象声明期
    typedef BufferVecotr::value_type BufferPtr;
    BufferPtr currentBuffer_ GUARDED_BY(mutex_); //当前缓冲
    BufferPtr nextBuffer_ GUARDED_BY(mutex_); //预备缓冲
    BufferVector buffers_ GUARDED_BY(mutex_);

    //前端
    void AsyncLogging::append(const char* logline, int len)
    {
    muduo::MutexLockGuard lock(mutex_);
    if (currentBuffer_->avail() > len) //最常见的情况，直接添加到curr A
    {
        currentBuffer_->append(logline, len);
    }
    //如果当前缓冲满了
    else
    {
        buffers_.push_back(std::move(currentBuffer_)); //添加当前缓冲

        if (nextBuffer_) //将预备缓冲移入
        {
        currentBuffer_ = std::move(nextBuffer_);
        }
        else
        {
        currentBuffer_.reset(new Buffer); // Rarely happens
        }
        currentBuffer_->append(logline, len);
        cond_.notify();
    }
    }

    //后端部分核心实现 接收方
    //一共分配了四块缓冲
    void AsyncLogging::threadFunc()
    {
    assert(running_ == true);
    latch_.countDown();
    LogFile output(basename_, rollSize_, false);
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    newBuffer1->bzero();
    newBuffer2->bzero();
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    while (running_)
    {
        assert(newBuffer1 && newBuffer1->length() == 0);
        assert(newBuffer2 && newBuffer2->length() == 0);
        assert(buffersToWrite.empty());

        {
        muduo::MutexLockGuard lock(mutex_);
        //buffers不空，说明当前缓冲满了 移入到了buffers
        if (buffers_.empty())  // unusual usage! 不常见的用法if 如果不空不会进入等待
        {
            cond_.waitForSeconds(flushInterval_);
        }
        //将当前缓冲移入buffer_ 有可能是三秒时间到，有可能是currentBuffer_满
        buffers_.push_back(std::move(currentBuffer_));
        currentBuffer_ = std::move(newBuffer1);
        buffersToWrite.swap(buffers_);
        if (!nextBuffer_)
        {
            nextBuffer_ = std::move(newBuffer2);
        }
    }
  ```
- AsyncLogging.cc


