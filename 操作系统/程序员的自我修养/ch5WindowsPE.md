# WindowsPE /COFF

## PE
- PE 和ELF同源都是来自COFF
- .code .data
- 段名没有实际意义，只起提示作用。如果使用链接脚本控制链接段名可能会有一定作用
- GCC利用`__attribute__((section("name")))` 自定义段扩展属性
- VISUAL C++使用` #proma`

## COFF PE的前身

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-27-09-42-12.png
width=290px>
</center>

- `cl /c /za SimpleSection.c`
- `dumpbin /ALL SimpleSection.obj > SimpleSection.txt` 查看目标文件结构
- `dumpbin SimpleSection.obj /SUMARY`

## 链接提示信息 .drectve

## 调试信息 .debug$S

## 大家都有符号表

