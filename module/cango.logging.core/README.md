# cango.logging.core API 文档

```mermaid
---
title: 日志逻辑图
---

flowchart TB

	cfa[条件、格式、参数]
	cfa-->|"未格式化的消息"|logger["static/runtime logger"]
	logger-->|"格式化后的消息"|os

	os[output_stream]-->|std::string_view|output["文件\n控制台\n……"]

```

## static&runtime 静态与运行期

静态对象是指，在代码的编译过程中就已经明确的对象。  
`static logger` 指在编译期就确定了使用何种输出流的日志器。  
`runtime logger` 指在编译期确定使用在运行时给定输出流的日志器。  

`runtime_stream` 提供了在运行期指定输出流的功能。  
