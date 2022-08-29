---
title: conda环境创建以及jupyter-notebook安装
tag: python
date: 2021-11-20 20:48:43
categories: python
cover: https://gitee.com/matytan/tupic/raw/master/uPic/image-20211120204928023.png
---

### 安装

```bash
# 创建anaconda环境
conda create -n torch python=3.6

#激活环境
conda activate torch

# 退出环境
conda deactivate
```

![image-20211120205029053](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211120205029053.png)



```bash
# 安装jupyternotebook
pip install notebook
```



###  使用自定义环境

```python
conda activate torch
python -m ipykernel install --user --name torch --display-name "pytorch(torch)"
#例如

# 打开
jupyter-notebook

```

![image-20211120205104723](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211120205104723.png)



![image-20211120204628320](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211120204628320.png)