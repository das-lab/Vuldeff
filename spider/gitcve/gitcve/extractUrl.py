# -*- coding: utf-8 -*-
import os
import json
from pathlib import Path
import re
import shutil

def loadSource(rootDirectory):
    # returns the list of .src files under the specified root directory.
    walkList = os.walk(rootDirectory)
    srcFileList = []
    for path, dirs, files in walkList:
        for fileName in files:
            ext = fileName.lower()
            if ext.endswith('.json') :
                absPathWithFileName = path.replace('\\', '/') + '/' + fileName
                srcFileList.append(absPathWithFileName)
    return srcFileList
def mkdir(path):
    folder = os.path.exists(path)
    if not folder:
        os.makedirs(path)
        print
        "---  new folder...  ---"
        print
        "---  OK  ---"

    else:
        print
        "---  There is this folder!  ---"

def writeURL(vulpath, name):
    jsonlist = loadSource(vulpath)#"E:\\vulDetect\\spider\\cvelist-master\\cvelist-master\\2021"
    print(jsonlist)
    with open(name, 'w') as u:
        for file in jsonlist:
            # 读取数据
            with open(file, 'r', encoding='utf-8') as f:
                data = json.load(f)
                if "references" in data:
                    for ref in data["references"]["reference_data"]:
                        if "gitlab.com" in ref["url"] :
                            cve_number = Path(file).stem
                            u.writelines(cve_number+" "+ref["url"]+'\n')
            f.close()
    u.close()

def mkDiffDir(path, urlFile):
    with open(urlFile, 'r') as f:
        for line in f.readlines():
            newPath = path + line.split()[0]
            mkdir(newPath)

def selectCCode(rootDirectory):
    walkList = os.walk(rootDirectory)
    srcFileList = []
    desFileList = []
    for path, dirs, files in walkList:
        for fileName in files:
            if fileName.endswith('.c') or fileName.endswith('.C'):
                absPathWithFileName = path.replace('\\', '/') + '/' + fileName
                srcFileList.append(absPathWithFileName)
    for file in srcFileList:
        newfile = file.replace('/2020/', '/2020-C/')
        newDir = newfile.rsplit("/", 1)[0]
        diff = file.rsplit("/", 1)[0]+"/.diff"
        mkdir(newDir)
        shutil.copy(file, newfile)
        shutil.copy(diff, newDir+"/.diff")
        desFileList.append(newfile)
    print(srcFileList)
    print(desFileList)
    return srcFileList

def countDiffsAFiles(rootDirectory):
    walkList = os.walk(rootDirectory)
    diffCount = 0
    fileCount = 0
    for path, dirs, files in walkList:
        for fileName in files:
            fileCount += 1
            ext = fileName.lower()
            if ext.endswith('.diff'):
                diffCount += 1
    return fileCount - diffCount,diffCount

def countCFiles(rootDirectory):
    walkList = os.walk(rootDirectory)
    fileCount = 0
    for path, dirs, files in walkList:
        for fileName in files:
            ext = fileName.lower()
            if ext.endswith('.c') or ext.endswith('.cpp') or ext.endswith('.cc') or ext.endswith(
                    '.c++') or ext.endswith('.cxx'):
                fileCount += 1
    return fileCount

def selectCCode2(rootDirectory):
    walkList = os.walk(rootDirectory)
    srcFileList = []
    desFileList = []
    for path, dirs, files in walkList:
        for fileName in files:
            ext = fileName.lower()
            if ext.endswith('.c') or ext.endswith('.cpp') or ext.endswith('.cc') or ext.endswith('.c++') or ext.endswith('.cxx'):
                absPathWithFileName = path.replace('\\', '/') + '/' + fileName
                srcFileList.append(absPathWithFileName)
    for file in srcFileList:
        #newfile = file.replace('/2020/', '/2020-C/')
        tokens = file.split("/",10)
        newFile = 'E:/torvalds/' + tokens[9] + '/' + tokens[5] + '/' + tokens[10]
        newDir = 'E:/torvalds/' + tokens[9] + '/' + tokens[5] + '/' + tokens[10].rsplit('/',1)[0]
        diff = tokens[0] + "/" + tokens[1] + "/" + tokens[2] + "/" + tokens[3] + "/" + tokens[4] + "/" + tokens[5] + "/.diff"
        newDiff = 'E:/torvalds/' + tokens[9] + '/' + tokens[5] + "/.diff"
        if (len(tokens[10].rsplit('/', 1)) == 2):
            mkdir(newDir)
        else:
            mkdir('E:/savannah/' + tokens[9] + '/' + tokens[5])
        print(file)
        shutil.copy(file, newFile)
        shutil.copy(diff, newDiff)
        desFileList.append(newFile)
    print(srcFileList)
    print(desFileList)
    return srcFileList

def selectCCode3(rootDirectory):
    walkList = os.walk(rootDirectory)
    srcFileList = []
    desFileList = []
    for path, dirs, files in walkList:
        for fileName in files:
            ext = fileName.lower()
            if ext.endswith('.c') or ext.endswith('.cpp') or ext.endswith('.cc') or ext.endswith(
                    '.c++') or ext.endswith('.cxx'):
                absPathWithFileName = path.replace('\\', '/') + '/' + fileName
                srcFileList.append(absPathWithFileName)
    for file in srcFileList:
        tokens = file.split("/", 5)
        newFile = rootDirectory + '/cfile/' + tokens[5]
        newDir = rootDirectory + '/cfile/' + tokens[5].rsplit('/', 1)[0]
        if (len(tokens[5].rsplit('/', 1)) == 2):
            mkdir(newDir)
        else:
            mkdir(rootDirectory + '/cfile')
        shutil.copy(file, newFile)
    print(srcFileList)
    return srcFileList

def countFingerprint(jsonFile):
    with open(jsonFile, 'r') as json_file:
        json_str = json_file.read()
    list = json.loads(json_str)
    count = len(list)
    return count
def countFingerprint2(rootDirectory):
    walkList = os.walk(rootDirectory)
    srcFileList = []
    for path, dirs, files in walkList:
        for fileName in files:
            ext = fileName.lower()
            if ext == "vul_fingerprint3.json":
                absPathWithFileName = path.replace('\\', '/') + '/' + fileName
                srcFileList.append(absPathWithFileName)
    count = 0
    for file in srcFileList:
        count += countFingerprint(file)
    return count
#selectCCode("E:\\vulDetect\\spider\\gitcve\\gitcve\\data\\2020\\")
#path = "E:\\vulDetect\\spider\\cvelist-master\\cvelist-master"
#name = "gitlaburl.txt"
#path2 = "E:\\data2\\1999\\"
#writeURL(path, name)
#mkDiffDir(path2, name)
path3 = "E:\\vulDetect\目标程序\\GraphicsMagick-1.3.37"
#print(countDiffsAFiles(path3))
selectCCode3(path3)
#path4 = "E:\\data\\github"
#print(countDiffsAFiles(path4))
#path5 = "E:\\vulData"
#print(countCFiles(path5))


'''
for dir in os.listdir(path3):
    print(dir,countDiffsAFiles(path3+"/"+dir))
'''
'''
#'(?:^\t[^\<\>\/\\\|\:\*\?]+\/)*[^\<\>\/\\\|\:\*\?]*\.[a-zA-Z0-9\.\_]*

pattern1 = r'\-\-\-\n[\s\S]*?\n\n'
pattern2 = r'^[ ][\s\S]*\|'
with open("url.txt", 'r') as u:
    for line in u.readlines():
        path = "E:\\vulDetect\\spider\\gitcve\\gitcve\\data\\2021\\" + line.split()[0]
        file = os.path.exists(path + "\\.patch")
        if file:
            try:
                with open(path + "\\.patch", 'r',errors='ignore') as f:
                    patch = f.read()
                    rawFiles = re.search(pattern1, patch).group()
                    print(rawFiles)
                    #pattern2 = re.compile(pattern2)
                    for line2 in rawFiles.split('\n'):
                        rawPath = re.search(pattern2, line2)
                        if re.search(pattern2, line2):
                            rawPath =rawPath.group().rstrip('|').strip()
                            #print(line.split()[1] + ":" + rawPath)
            except:
                print("file:"+path)
'''