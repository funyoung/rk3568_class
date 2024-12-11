#/bin/sh

#ip应该改成开发板的ip地址，而且ssh可用
#本地~/.ssh/目录下放自己的密钥对
#1. 复制公钥到服务器： $ssh-copy-id username@remote_host
#2. 配置SSH客户端可选：~/.ssh/config
# Host remote_host
#    HostName remote_host
#    User username
#    IdentityFile ~/.ssh/id_rsa

files=$@
if [ $# -gt 0 ]; then
    files=$@
else
    files="app *.ko"
fi

scp $files 192.168.101.76:~/
