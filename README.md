# CursorLimiter

A program that limits the cursor to ONE screen on macOS.

macOS多屏幕环境中，限制鼠标在某一个屏幕。

# 使用方法

## 初始设定
1. 编译

2. 在**安全性与隐私**的**辅助功能**中添加**终端.app**

3. 双击运行CursorLimiter

## 切换鼠标所在屏幕

1. 按住**Command**键

2. 将鼠标移动到其他屏幕

3. 松开**Command**键

## 热键修改

屏幕切换热键在源码的``const CGEventFlags SwitchKey``中定义为``kCGEventFlagMaskCommand``，可按需修改。

如修改为``const CGEventFlags SwitchKey = kCGEventFlagMaskCommand | kCGEventFlagMaskControl;``

则表示使用**Command + Control**键进行切换。
