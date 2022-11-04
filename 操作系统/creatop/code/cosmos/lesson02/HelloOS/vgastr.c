//彭东 @ 2021.01.09

void _strwrite(char* string)//字符串写入
{
    char* p_strdst = (char*)(0xb8000);//先找到 地址
    while (*string)
    {

        *p_strdst = *string++;
        p_strdst += 2; //每次加两个字节 跳过字符颜色信息空间

    }
    return;
}

void printf(char* fmt, ...)
{
    _strwrite(fmt);
    return;
}
