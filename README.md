# 简介
https://www.virusview.net/malware/Trojan/Script/Guloader

## 起因
Guloader加载器使用RtlAddVectoredExceptionHandler函数用于反调试 基于处理异常来更改程序执行流程 所以这个库诞生了

# 效果
<img width="1806" height="639" alt="image" src="https://github.com/user-attachments/assets/4c56f390-43da-4ce5-aa43-5e25269cfbdb" />

# 使用方法
## X64配置
使用方法 x64dbg->选项->异常->异常处理者->调试器

<img width="410" height="645" alt="image" src="https://github.com/user-attachments/assets/ff27b62f-7684-41ba-a636-94f78f9e9bd1" />

# 原理
## 基于 VEH
该恶意样本通过 `RtlAddVectoredExceptionHandler` 的方式添加了新的异常处理CALL 里面的伪代码去除混淆之后：
<img width="1007" height="587" alt="image" src="https://github.com/user-attachments/assets/01fb61f6-c43c-43cf-a5fd-1899a8803f9d" />

# 其他
## x64版本没有弄 有需要的可以自已弄下 只弄好了x32版本的

