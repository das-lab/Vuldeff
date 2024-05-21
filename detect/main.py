# -*- coding: utf-8 -*-
import click
import os
import call_antlr as ca
import json
import ssdeep as sd
import AhoCorasick as AC
import time
from tqdm import tqdm

def detect(targetPath, vuldatabase):
    vulPath = os.path.join('E:\\vulDetect\代码及数据\数据\\CVE\\' + vuldatabase + '\\vul_fingerprint.json')
    start1 = time.perf_counter()
    threshold = 50
    files = ca.loadSource(targetPath)
    vulFile = open(vulPath, 'r')
    vuls = json.loads(vulFile.read())
    vulFile.close()
    functionInstanceList = []

    click.echo("开始源代码语法解析：")
    for file in tqdm(files):
        funclist = ca.parseFile_source(file)
        if funclist != None:
            functionInstanceList.extend(funclist)
    end1 = time.perf_counter()
    click.echo("语法解析结束!\n")
    click.echo("开始漏洞检测：")
    start2 = time.perf_counter()
    funcNum = 0
    vulFuncs = {}
    vulNum = 0

    for functionInstance in tqdm(functionInstanceList):
        funcNum += 1
        func_abs = ca.abstract(functionInstance)[1]
        func_normal = ca.normalize(func_abs)
        func_ssdeep = sd.ssdeep_hash(func_normal)
        for vul in vuls:
            similarity = sd.ssdeep_compare(func_ssdeep, vuls[vul][0])
            #print(similarity)
            if similarity >= threshold:
                #print(functionInstance.parentFile + "." + functionInstance.name)

                ac_adds = AC.ac_automation(vuls[vul][1])  # 增加行ac向量机
                ac_subs = AC.ac_automation(vuls[vul][2])  # 删除行ac向量机
                if len(vuls[vul][2]) != 0:
                    ac_subs.add_keyword()  # 删除行不为空只看删除行
                    res = ac_subs.search(func_normal)
                    if len(res) == len(set(vuls[vul][2])):  # 删除行全部匹配
                        vulNum += 1
                        try:
                            vulFuncs[functionInstance.parentFile + "." + functionInstance.name].append(vul)
                        except KeyError:
                            vulFuncs[functionInstance.parentFile + "." + functionInstance.name] = [vul]
                else:
                    ac_adds.add_keyword()
                    res = ac_adds.search(func_normal)
                    if len(res) == 0:
                        vulNum += 1
                        try:
                            vulFuncs[functionInstance.parentFile + "." + functionInstance.name].append(vul)
                        except KeyError:
                            vulFuncs[functionInstance.parentFile + "." + functionInstance.name] = [vul]

    json_str = json.dumps(vulFuncs, indent=4)
    click.echo("漏洞检测结束!")
    print('共检测了%s个函数' % (funcNum))
    print('检测出了%s个漏洞函数' % (vulNum))
    print(json_str)
    end2 = time.perf_counter()
    print('代码解析时间: %.2f秒' % (end1 - start1))
    print('漏洞检测时间: %.2f秒' % (end2 - start2))

    with open(targetPath + '\\vulFuncs-50.json', 'w') as json_file:
        json_file.write(json_str)
    json_file.close()


def print_help(ctx, param, value):
    if value is False:
        return
    click.echo(ctx.get_help())
    ctx.exit()

@click.command()
@click.option('--program', '-p', help = '输入待检测程序的路径')
@click.option('--vuldatabase', '-v', multiple = True,
              help = '选择一个或多个漏洞数据库[tcpdump, linux, FFmpeg, libgit2, ImageMagick]')
@click.option('--help','-h',
 is_flag=True,
 expose_value=False,
 is_eager=False,
 callback=print_help,
 help="Print help message")
@click.pass_context

def call_detect(ctx, program, vuldatabase):
    vuls = ['tcpdump', 'linux', 'FFmpeg', 'ImageMagick', 'libgit2']
    if program ==None or vuldatabase == ():
        print_help(ctx, vuldatabase, value=True)
        return
    if os.path.exists(program):
        for item in vuldatabase:
            if item not in vuls:
                click.echo("请在漏洞数据库[tcpdump, linux, FFmpeg, libgit2, ImageMagick]中选择!")
                print_help(ctx, vuldatabase, value=True)
                return
        for item in vuldatabase:
            detect(program, item)
    else:
        click.echo("待检测程序的路径无效!")
        print_help(ctx, vuldatabase, value=True)


if __name__=="__main__":
    call_detect()