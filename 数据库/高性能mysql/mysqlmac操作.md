`sudo /usr/local/mysql/support-files/mysql.server start
sudo /usr/local/mysql/support-files/mysql.server stop
sudo /usr/local/mysql/support-files/mysql.server restart
alias mysql=/usr/local/mysql/bin/mysql`


设置终端环境变量：
(base) matytan@matytandeMacBook-Pro ~ % vim ~/.zshrc
(base) matytan@matytandeMacBook-Pro ~ % source ~/.zshrc
在.zshrc中添加alias mysql=/usr/local/mysql/bin/mysql`