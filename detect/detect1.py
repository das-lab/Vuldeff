# -*- coding: utf-8 -*-
import click
import os
import call_antlr as ca
import json
import ssdeep as sd
import AhoCorasick as AC
import time
import math
from zlib import crc32

def detect(targetPath, vulPaths):
    #vulPath = os.path.join('E:/vulData/' + vuldatabase + '/vul_fingerprint3.json')
    start1 = time.perf_counter()
    threshold = 50
    threshold2 = 80.00
    threshold3 = 30.00
    threshold4 = 90
    files = ca.loadSource(targetPath)
    walkList = os.walk(vulPaths)
    vuls = []
    for path, dirs, files2 in walkList:
        for fileName in files2:
            if fileName == "vul_fingerprint3.json":
                absPathWithFileName = path.replace('\\', '/') + '/' + fileName
                print(absPathWithFileName)
                vulFile = open(absPathWithFileName, 'r')
                pfs = json.loads(vulFile.read())
                vuls.extend(pfs)
                vulFile.close()
    print(vuls)
    functionInstanceList = []
    funcNum = 0
    vulFuncs = {}
    vulNum = 0
    print("开始源代码语法解析：")
    for file in files:
        funclist = ca.parseFile_source(file)
        if funclist != None:
            functionInstanceList.extend(funclist)

    end1 = time.perf_counter()
    click.echo("语法解析结束!\n")
    click.echo("开始漏洞检测：")
    start2 = time.perf_counter()

    for functionInstance in functionInstanceList:
        funcNum += 1
        func_abs = ca.abstract(functionInstance)[1]
        func_normal = ca.normalize(func_abs)
        func_ssdeep = sd.ssdeep_hash(func_normal)
        if func_ssdeep.startswith("3"):
            continue
        for vul in vuls:
            if vul["bodyHash"].startswith("3"):
                continue
            similarity = sd.ssdeep_compare(func_ssdeep, vul["bodyHash"])
            print(similarity)
            if similarity >= threshold:
                print(similarity)
                #print(functionInstance.parentFile + "." + functionInstance.name)

                ac_adds = AC.ac_automation(vul["addLines"])  # 增加行ac向量机
                ac_subs = AC.ac_automation(vul["subLines"])  # 删除行ac向量机

                if (len(vul["addLines"]) == 0) and (len(vul["subLines"]) == 0):
                    if similarity >= threshold4:
                        vulNum += 1
                        vul["similarity"] = str(similarity)
                        try:
                            if similarity >= int(
                                    vulFuncs[functionInstance.parentFile + "." + functionInstance.name]["similarity"]):
                                vulFuncs[functionInstance.parentFile + "." + functionInstance.name] = vul
                        except KeyError:
                            vulFuncs[functionInstance.parentFile + "." + functionInstance.name] = vul
                elif (len(vul["addLines"]) == 0) and (len(vul["subLines"]) != 0):# 增加行为空只看删除行
                    ac_subs.add_keyword()
                    res_subs = ac_subs.search(func_normal)
                    print("增加行为空只看删除行")
                    similarity2 = float(len(res_subs))/float(len(set(vul["subLines"])))
                    if similarity2 >= (threshold2/100.00):
                        vulNum += 1
                        vul["similarity"] = str(similarity)
                        print(vul)
                        try:
                            if similarity >= int(
                                    vulFuncs[functionInstance.parentFile + "." + functionInstance.name]["similarity"]):
                                vulFuncs[functionInstance.parentFile + "." + functionInstance.name] = vul
                        except KeyError:
                            vulFuncs[functionInstance.parentFile + "." + functionInstance.name] = vul
                elif (len(vul["addLines"]) != 0) and (len(vul["subLines"]) == 0):#删除行为空只看增加行
                    ac_adds.add_keyword()
                    res_adds = ac_adds.search(func_normal)

                    print("删除行为空只看增加行")
                    similarity3 = float(len(res_adds)) / float(len(set(vul["addLines"])))
                    if similarity3 <= (threshold3 / 100.00):
                        print(func_normal)
                        vulNum += 1
                        vul["similarity"] = str(similarity)
                        print(vul)
                        try:
                            if similarity >= int(
                                    vulFuncs[functionInstance.parentFile + "." + functionInstance.name]["similarity"]):
                                vulFuncs[functionInstance.parentFile + "." + functionInstance.name] = vul
                        except KeyError:
                            vulFuncs[functionInstance.parentFile + "." + functionInstance.name] = vul
                else:
                    ac_subs.add_keyword()
                    res_subs = ac_subs.search(func_normal)
                    ac_adds.add_keyword()
                    res_adds = ac_adds.search(func_normal)
                    print("都不为空")
                    similarity2 = float(len(res_subs)) / float(len(set(vul["subLines"])))
                    similarity3 = float(len(res_adds)) / float(len(set(vul["addLines"])))
                    if (similarity2 >= (threshold2 / 100.00)) \
                            and (similarity3 <= (threshold3 / 100.00)):
                        print(func_normal)
                        vulNum += 1
                        vul["similarity"] = str(similarity)
                        print(vul)
                        try:
                            if similarity >= int(
                                    vulFuncs[functionInstance.parentFile + "." + functionInstance.name]["similarity"]):
                                vulFuncs[functionInstance.parentFile + "." + functionInstance.name] = vul
                        except KeyError:
                            vulFuncs[functionInstance.parentFile + "." + functionInstance.name] = vul


    json_str = json.dumps(vulFuncs, indent=4)
    click.echo("漏洞检测结束!")
    print('共检测了%s个函数' % (funcNum))
    print('检测出了%s个漏洞函数' % (vulNum))
    #print(json_str)
    end2 = time.perf_counter()
    print('代码解析时间: %.2f秒' % (end1 - start1))
    print('漏洞检测时间: %.2f秒' % (end2 - start2))
    return vulFuncs


targetPath = "E:\\vulDetect\目标程序\\tcpdump-master\\cfile"
json_str = json.dumps(detect(targetPath, "E:\\vulData\\the-tcpdump-group\\tcpdump"), indent=4)
with open(targetPath + '\\vulFuncs-50-80-30-90-4.json', 'w') as json_file:
    json_file.write(json_str)
json_file.close()