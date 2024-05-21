#!/usr/bin/env python
# -- coding: utf-8 --
import os
'''
返回每个文件行数，其中行数不包括以“#”开头的包含文件，宏定义等，
排除了c,cpp文件中的“//”, “/*...*/”等的注释，
排除了python文件中import, from 等开头的导入
'''
def get_lines(file_name):
    f = open(file_name, encoding='latin-1')
    #flag用于处理c,cpp中“/*...*/”多行注释
    flag = False
    count = 0
    while True:
        #读取文件并去除开头的空格，制表符
        line = f.readline()
        line = line.lstrip(' \t')
        if not line:
            break
        #如果该行有“#”， “import”等打头的字符，忽略该行
        if flag == False:
            if line[0:1] == "#" or line[0:6] == "import" or line[0:4] == "from" or line == "\n" or line[0:2] == "//":
                continue
        #如果该行存在“/*”并且不存在“*/”,表明多行注释未在一行结束，flag=True
        if line.find("/*") != -1 :
            if line.find("*/") != -1:
                continue
            else:
                flag = True
                continue
        #如果flag=True，表明处于多行注释中，判断是否有“*/”结尾
        if flag == True :
            if line.find("*/") != -1:
                flag = False
                if line[-2:] != "*/":
                    count = count+1
            continue
        #排除以上条件后，行数增加一
        count = count+1
    f.close()
    return count
'''
计算该文件目录下所有符合条件的行数
'''
def count_lines(file_dir):
    #total_lines表示总行数，file_nums表示总文件数
    total_lines = 0
    file_nums = 0
    for root, dirs, files in os.walk(file_dir):
        for file in files:
            #不计算本文件的行数
            if file == "countLines.py":
                continue
            #只计算规范命名文件, 如[文件名.文件类型]
            file_type = file.split('.')
            if len(file_type) > 1 :
                #如果想计算其他类型的文件，可以在这里进行修改
                if file_type[1] not in ["cxx", "c", "cc", "cpp", "c++"]:
                    continue
            else:
                continue
            file_name = root +"\\" + file
            lines = get_lines(file_name)
            total_lines = total_lines + lines
            print(file_name + " contains lines : " + repr(lines))
            file_nums = file_nums + 1
    #输出结果
    print("------------------------------------")
    print("Total Files : " + repr(file_nums))
    print("Total lines : " + repr(total_lines))
    print("------------------------------------")

path = "E:\\vulDetect\目标程序\FFmpeg-master\cfile"
count_lines(path)