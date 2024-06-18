const config_module = require('./config')
const redis = require('ioredis')

// 创建redis客户端
const redis_cli = new redis({
	host:config_module.redis_host,
	port: config_module.redis_port,
	password: config_module.redis_passwd
});

// 监听错误信息
redis_cli.on("error", function(err) {
	console.log("redis_cli connect error");
	redis_cli.quit();
});
process.on('SIGINT', () => {
    console.log('Received SIGINT. Quitting Redis connection...');
    Quit();
    process.exit(0); // 结束程序
});

// 根据key查找value
async function GetRedis(key) {
	try {
		const result = await redis_cli.get(key);
		if (result == null) {
			console.log('result:', '[' + result + ']', 'the key not found!');
			return null;
		}
		console.log('result:', '[' + result + ']', 'get key success');
		return result;
	} catch (error) {
		console.log('GetRedis error:', error);
		return null;
	}
}


// 查询key是否存在

async function QueryRedis(key) {
	try{
			const result = await redis_cli.exists(key);
			//  判断该值是否为空 如果为空返回null
			if (result === 0) {
				console.log('result:<','['+result+']','This key is null');
				return null
			}
			console.log('Result:','['+result+']','The key exists');
			return result
	}catch(error){
			console.log('QueryRedis error is', error);
			return null
	}
}

// 设置kv和过期时间
async function SetRedisExpire(key,value, exptime){
    try{
        // 设置键和值
        await redis_cli.set(key,value);
        // 设置过期时间（以秒为单位）
        await redis_cli.expire(key, exptime);
        return true;
    }catch(error){
        console.log('SetRedisExpire error is', error);
        return false;
    }
}

// 断开连接
function Quit(){
    redis_cli.quit();
}


module.exports = {GetRedis, QueryRedis, Quit, SetRedisExpire}