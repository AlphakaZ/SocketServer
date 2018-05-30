# SocketServer
C言語によるソケットサーバ with cmake

## 使い方
ルートディレクトリで
`$ ./build.sh`
を叩くとcmakeとmakeが走り、コンパイルされます。

実行ファイルはルートディレクトリに移動されます。

$ ./SocketServer 8080 //実行。ただし'8080'の部分は任意で指定するポート番号

## Todo:
htmlその他ファイルの準備
そもそもどうやらSegmentation Faultで落ちているのでfix必須