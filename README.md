### 环境
*c++相关*：
c++ 17
grpc 1.64.0
boost 1.74
clang++ 14.0.0
cmake 3.22.1
redis++ 1.13.12
*nodejs相关*:
nodejs 12.22.9
@grpc/grpc-js 1.10.9   
@grpc/proto-loader 0.7.13
express 4.19.2
nodemailer 6.9.13
ioredis 5.4.1
*数据库相关*:
redis5

## grpc 处理gateServer与Server的通信
客户端发起注册请求后,会发送一个post请求到网关服务器,网关服务器通过rpc调用与处理注册请求服务器通信
处理注册请求的服务器会生成验证码并发送邮件给用户邮箱,使用redis设置过期时间