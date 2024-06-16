## grpc 处理gateServer与Server的通信
目前只有获取验证码
gprc 服务端用nodejs实现 `./VerifyServer/config.json`里填写配置信息
还没加redis和mysql,后续再加

客户端c++实现 `./gateServer/config/config.ini`里填写配置信息，目前还未做太多处理，都是127.0.0.1,后续更改