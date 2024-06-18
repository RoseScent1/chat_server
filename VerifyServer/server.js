const grpc = require('@grpc/grpc-js');
const message_proto = require('./proto');
const const_module = require('./const')
const emailModule = require('./email')
const redis_modules = require('./redis');

// 生成验证码
function generateRandomNumber() {
	var min = 100000; // 最小值
	var max = 999999; // 最大值
	var randomNumber = Math.floor(Math.random() * (max - min + 1)) + min;
	return randomNumber;
}
// 获取验证码并返回结果
async function GetVerifyCode(call, callback) {
	console.log("email is ", call.request.email)

	try {
		// 查询3分钟内是否已经获取过验证码了
		let query_res = await redis_modules.GetRedis(call.request.email);
		console.log("query_res is ", query_res);
		let verify_code = query_res;
		// 没获取
		if (query_res == null) {
			verify_code = generateRandomNumber();
			let res = redis_modules.SetRedisExpire(call.request.email, verify_code, 180);
			if (!res) {
				callback(null, {
					email: call.request.email,
					error: const_module.Errors.RedisErr
				});
				return;
			}
		}
		console.log("验证码： ", verify_code)
		let text_str = '您的验证码为:' + verify_code + ' 请三分钟内完成注册'
		
		
		//发送邮件
		let mailOptions = {
			from: 'm19558918113@163.com',
			to: call.request.email,
			subject: 'wahahahah',
			text: text_str,
		};
		let send_res = await emailModule.SendMail(mailOptions);
		console.log("send res is ", send_res)
		if (!send_res) {
				callback(null, {
					email: call.request.email,
					error: emailModule.Errors.error
				});
				return ;
		}
		callback(null, {
			email: call.request.email,
			error: const_module.Errors.Success
		});


	} catch (error) {
		console.log("catch error is ", error)

		callback(null, {
			email: call.request.email,
			error: const_module.Errors.Exception
		});
	}

}

function main() {
	var server = new grpc.Server()
	server.addService(message_proto.VerifyService.service, { GetVerifyCode: GetVerifyCode })
	server.bindAsync('0.0.0.0:50051', grpc.ServerCredentials.createInsecure(), () => {
		console.log('grpc server started')
	})
	redis_modules.Quit()
}

main()