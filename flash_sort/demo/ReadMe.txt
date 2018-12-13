==== 选手需要提供至少三个文件 ====

./YumInstall
	列出需要yum安装的rpm
./build
	负责编译和其他准备工作
./run
	负责实际执行排序
	三个命令行参数，依次是 输入文件名 输出文件名 临时目录
	例如：./run in.txt out.txt /search/tmp

==== 举办方提供的辅助程序，供选手参考 ====

./gentest
	帮助生成测试样例的程序
	一个参数，需要生成多少M的文件
	例如：./gentest 10 > sample.in.txt

	
