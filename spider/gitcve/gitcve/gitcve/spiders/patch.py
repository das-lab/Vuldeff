# -*- coding: utf-8 -*-
import scrapy
import os

class PatchSpider(scrapy.Spider):
    name = 'patch'
    allowed_domains = ['github.com']
    start_urls = ['https://github.com/']
    def parse(self, response):
        path = "E:\\data2\\1999\\"
        count = 0
        with open("../1999url.txt", 'r') as u:
            for line in u.readlines():
                link = line.split( )[1].rstrip(',')+".diff"
                diffDir = path + line.split( )[0] + "\\" + line.split( )[1].rstrip(',').split('/')[-1]
                if os.path.exists(diffDir):
                    continue
                else:
                    print(line)
                    count+=1
                    yield scrapy.Request(
                        link,
                        callback=self.parse_detail,
                        meta = {"cveNumber":diffDir }
                    )
            print(count)
        u.close()

    def parse_detail(self, response):
        if response.status == 200:
            diffPath = response.meta["cveNumber"] + "\\.diff"
            os.makedirs(response.meta["cveNumber"])
            with open( diffPath, 'wb+') as f:
                f.write(response.body)
            f.close()