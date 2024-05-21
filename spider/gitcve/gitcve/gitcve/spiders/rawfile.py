# -*- coding: utf-8 -*-
import scrapy
import os
import re

def countFiles(path):
    file_count = 0
    for dirpath, dirnames, filenames in os.walk(path):
        for file in filenames:
            file_count = file_count + 1
    return file_count
pattern1 = r'\-\-\-\n[\s\S]*?\n\n'
pattern2 = r'^[ ][\s\S]*\|'
class RawfileSpider(scrapy.Spider):
    name = 'rawfile'
    allowed_domains = ['github.com','raw.githubusercontent.com']
    start_urls = ['https://github.com']

    def parse(self, response):
        with open("../1999url.txt", 'r') as u:
            for line in u.readlines():
                path = "E:/data2/1999/" + line.split()[0] +  \
                       "/" + line.split( )[1].rstrip(',').split('/')[-1]
                print(path)
                if os.path.exists(path):
                    if countFiles(path) == 1:
                        yield scrapy.Request(
                            line.split()[1],
                            callback=self.parse_patch,
                            meta={"path": path}
                        )
        u.close()

    def parse_patch(self, response):
        parent = response.xpath("//span[@class='sha-block ml-0']/a/@href").extract()
        parent = parent[0].replace("/commit/", "/")
        try:
            with open(response.meta["path"] + "/.diff", 'r', encoding='gbk') as u:
                for line in u.readlines():
                    # print(line)
                    line = line.rstrip('\n')  # 可能会出错
                    if line.startswith("--- a/"):
                        fileName = line.split('/',1)[1]
                        print(line)
                        yield scrapy.Request(
                            'https://raw.githubusercontent.com' + parent + '/' + fileName,
                            callback=self.parse_rawFile,
                            meta={
                                "parent": parent,
                                "fileName": fileName,
                                "path": response.meta["path"]}
                        )
            u.close()
        except:
            with open(response.meta["path"] + "/.diff", 'r', encoding='utf-8') as u:
                for line in u.readlines():
                    # print(line)
                    line = line.rstrip('\n')  # 可能会出错
                    if line.startswith("--- a/"):
                        fileName = line.split('/', 1)[1]
                        print(line)
                        yield scrapy.Request(
                            'https://raw.githubusercontent.com' + parent + '/' + fileName,
                            callback=self.parse_rawFile,
                            meta={
                                "parent": parent,
                                "fileName": fileName,
                                "path": response.meta["path"]}
                        )
            u.close()

    def parse_rawFile(self, response):
        if response.status == 200:
            #print(response.meta["path"])
            #print(response.meta["parent"])
            #print(response.meta["fileName"])
            if (len(response.meta["fileName"].rsplit('/',1))==2):
                filename = (response.meta["path"] + "/" + response.meta["parent"].split('/')[1] + "/" + \
                        response.meta["parent"].split('/')[2] + "/" + response.meta["fileName"].rsplit('/',1)[0])
            else:
                filename = (response.meta["path"] + "/" + response.meta["parent"].split('/')[1] + "/" + \
                            response.meta["parent"].split('/')[2])
            if not os.path.exists(filename):
                os.makedirs(filename)
            #print(filename)
            with open(filename + "/" + response.meta["fileName"].split('/')[-1], 'wb+') as f:
                f.write(response.body)
            f.close()
