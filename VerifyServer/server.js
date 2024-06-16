const grpc = require('@grpc/grpc-js');
const message_proto = require('./proto');
const const_module = require('./const')
const emailModule = require('./email')

function generateRandomNumber() {
    var min = 100000; // 最小值
    var max = 999999; // 最大值
    var randomNumber = Math.floor(Math.random() * (max - min + 1)) + min;
    return randomNumber;
}

async function GetVerifyCode(call, callback) {
	console.log("email is ", call.request.email)
	try {
		let verify_code =generateRandomNumber() ;
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
}

main()