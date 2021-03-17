* 本项目是 [redis源码](https://github.com/redis/redis/tree/6.0/src) 的 `aeEventLoop` 的样例，用于对 [redis源码](https://github.com/redis/redis/tree/6.0/src) 的探索学习。
* **ae.h**, **ae.c**, **ae_epoll.c** 是直接摘取自 [redis源码](https://github.com/redis/redis/tree/6.0/src) 。
* **config.h**, **zmalloc.h** 文件是为了减少 `aeEventLoop` 的依赖而 mock 的无意义的文件。
* **time_event_example.c**, **file_event_example.c**, **before_sleep_example.c** 分别实现 `aeTimeEvent`, `aeFiredEvent`, `aeSetBeforeSleepProc` 和 `aeSetAfterSleepProc` 的样例。
* **makefile**
	* `make time_event`: 每秒执行一次的计时器
	* `make file_event`: 监听 8080 端口的服务，用 `nc localhost 8080` 命令可以测试效果。
	* `make before_sleep`: `aeSetBeforeSleepProc` 和 `aeSetAfterSleepProc` 的效果
