import requests

# cityCode 替换为具体某一个城市的对应编号# 1、发送请求，获取数据
#url = f'http://wthrcdn.etouch.cn/weather_mini?citykey={cityCode}'
cities = ('阜阳', '杭州')
for c in cities:
	url = f'http://wthrcdn.etouch.cn/weather_mini?city='+c
	res = requests.get(url)
	res.encoding = 'utf-8'
	res_json = res.json()

# 2、数据格式化
	data = res_json['data']
	city = f"城市：{data['city']}"
# 字符串格式化的一种方式 f"{}" 通过字典传递值

	today = data['forecast'][0]
	date = f"日期：{today['date']}"
	now = f"实时温度：{data['wendu']}度"
	temperature = f"温度：{today['high']} {today['low']}"
	fengxiang = f"风向：{today['fengxiang']}"
	type = f"天气：{today['type']}"
	tips = f"贴士：{data['ganmao']}"

#result = city + ", " + date + ", " + now + ", " + temperature + ", " + type
#result = data['city'] + ': ' + today['type'] + ', ' + data['wendu'] + ', ' + today['low'] + '~' + today['high']
	for i in range(1, 4):
		day =  data['forecast'][i]
		seq = [data['city'], day['type'], day['low'][2:] + '~' + day['high'][2:], day['date']]
		result = '    '.join(seq)
		print(result)
