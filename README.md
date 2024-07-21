# texteditor
## 一、简介
  TextEditor 是一个简易的类 vi 的文本编辑器，由 C 语言编写而成。
  TextEditor 最初编写的目的是为了总结有关 Xterm 终端控制序列的知识，所以并没有参考 VIM 等成熟编辑器的源代码，大部分运行逻辑为独立实现，仅追求对类 Vi 编辑器其最基本功能上的还原。
  由于已经有 VIM 等成熟的文本编辑器，TextEditor 不作为 VIM 等软件的替代品，所以实现较为简单，主要为单片机等平台设计，所以并未采用动态内容分配的设计，且尽量减少内存的消耗。
