## git实际操作

![b4ed976572777601d383673d1724222e](https://gitee.com/matytan/tupic/raw/master/uPic/b4ed976572777601d383673d1724222e.jpeg)

### 修改文件

> `git status`：查看哪些未提交的到git（add是暂存区）养成好习惯经常看看

```bash
(base) matytan@matytandeMacBook-Pro img % git status
On branch master
Your branch is up to date with 'origin/master'.

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
        modified:   readme.md

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        newone.txt

no changes added to commit (use "git add" and/or "git commit -a")
```



使用命令查看修改了哪些内容：`git diff readme.md`

```bash
 git clone git@gitlab.com:matytan/gitstudy.git
-cd gitstudy
+dd gitstudy
 git switch -c main
-touch README.md
 git add README.md
 git commit -m "add README"
 git push -u origin main
+111111:
+git diff
+git status
+1111111
```



> 查看修改了哪些之后，就可以提交了

- `git add filename`

  - `git add`的参数

  ```bash
  git add -A和 git add . git add -u在功能上看似很相近，但还是存在一点差别
  
  git add . ：会监控工作区的状态树，使用它会把工作时的所有变化提交到暂存区，包括文件内容修改(modified)以及新文件(new)，但不包括被删除的文件。
  
  git add -u ：仅监控已经被add的文件（即tracked file），会将被修改的文件提交到暂存区。add -u 不会提交新文件（untracked file）。（git add –update的缩写）
  
  git add -A ：是上面两个功能的合集（git add –all的缩写）
  
  下面是具体操作例子，方便更好的理解（Git version 1.x）：
  ```

  

- `git commit filename`

### 版本回退

- `git log`

```bash
(base) matytan@matytandeMacBook-Pro img % git log
commit 16f38a99716873eff0a19bd049fc23319e2eedfc (HEAD -> master)
Author: maty tan <tqtaylor@163.com>
Date:   Tue Nov 16 15:19:01 2021 +0800

    添加内容,已经条件新文件newone.txt

commit 7f9f26fc69aabbf861a4f4d6b3c11a520e972cf5 (origin/master)
Author: maty tan <tqtaylor@163.com>
Date:   Tue Nov 16 15:04:51 2021 +0800

```

- 回退

`git reset –hard HEAD^`

>第一种是： 那么如果要回退到上上个版本只需把HEAD^ 改成 HEAD^^ 以此类推。那如果要回退到前100个版本的话，使用上面的方法肯定不方便，我们可以使用下面的简便命令操作：git reset –hard HEAD~100 即可

```bash
(base) matytan@matytandeMacBook-Pro img % git reset --hard HEAD^
HEAD is now at 16f38a9 添加内容,已经条件新文件newone.txt
(base) matytan@matytandeMacBook-Pro img % cat readme.md 
git clone git@gitlab.com:matytan/gitstudy.git
dd gitstudy
git switch -c main
git add README.md
git commit -m "add README"
git push -u origin main
111111:
git diff
git status
1111111
```

- 发现新添加的内容没有了，那么我又想回到刚刚新版本该怎么弄？

  >git reset –hard 版本号 ，但是现在的问题假如我已经关掉过一次命令行或者333内容的版本号我并不知道呢？要如何知道增加3333内容的版本号呢？可以通过如下命令即可获取到版本号：git reflog 演示如下：

  `git reflog `命令查看刚刚新版本号！

  ```bash
  (base) matytan@matytandeMacBook-Pro img % git reflog
  16f38a9 (HEAD -> master) HEAD@{0}: reset: moving to HEAD^
  267c835 HEAD@{1}: commit: 添加3333
  16f38a9 (HEAD -> master) HEAD@{2}: commit: 添加内容,已经条件新文件newone.txt
  7f9f26f (origin/master) HEAD@{3}: commit (initial): init-first
  
  (base) matytan@matytandeMacBook-Pro img % git reset -hard 267c835
  error: did you mean `--hard` (with two dashes)?
  (base) matytan@matytandeMacBook-Pro img % git reset --hard 267c835
  HEAD is now at 267c835 添加3333
  ```

  

### 撤销修改和删除文件的操作（还没有add之前）

> 在未提交（未add）之前，发现添加内容有误，怎么恢复之前内容？

`git checkout — file` 可以丢弃工作区的修改

-  readme.txt修改后，还没有放到暂存区，使用`git checkout`撤销修改就回到和版本库一模一样的状态。
- 另外一种是readme.txt已经放入暂存区了，接着又作了修改，撤销修改就回到添加暂存区后的状态。



> 覆盖远程git

`git push origin 分支 –force`

> 参考：https://segmentfault.com/a/1190000005370037