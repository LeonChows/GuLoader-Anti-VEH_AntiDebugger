# 使用方法
## X64配置
使用方法 x64dbg->选项->异常->异常处理者->调试器

<img width="410" height="645" alt="image" src="https://github.com/user-attachments/assets/ff27b62f-7684-41ba-a636-94f78f9e9bd1" />

# 原理
## 基于 VEH
该恶意样本通过 `RtlAddVectoredExceptionHandler` 的方式添加了新的异常处理CALL 里面的伪代码去除混淆之后：
<img width="1007" height="587" alt="image" src="https://github.com/user-attachments/assets/01fb61f6-c43c-43cf-a5fd-1899a8803f9d" />
