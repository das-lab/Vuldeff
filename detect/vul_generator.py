# coding=gbk
import json
import call_antlr as ca
import ssdeep as sd
import os

from zlib import crc32

def getCrc32(string):
	return hex(crc32(string))

def extractFuncLs(functionInstanceList):
    funcLs = []
    for funcInstance in functionInstanceList:
        funcLs.append([funcInstance.name, funcInstance.lines, funcInstance])
    return funcLs

def patchToFunc(targetPath):
    walkList = os.walk(targetPath)
    vul_fingerprint = []
    #for path, dirs, files in walkList:
    for dir in os.listdir(targetPath):
        unipath = targetPath+ '\\' + dir
        if dir.endswith("json"):
            break
        if ".diff" in os.listdir(unipath):
            patchFile = unipath + "\\.diff"
            try:
                with open(patchFile, 'r', encoding='utf-8') as u:
                    patchList = []
                    flag = False
                    for line in u.readlines():
                        #print(line)
                        line = line.rstrip('\n')  # 可能会出错
                        # try:
                        if line.startswith("diff --git"):
                            flag = True
                        if flag:
                            if line.startswith("--- "):
                                fileName = line.split('/', 1)[1]
                                fileDic = {"fileName": fileName, "blocks": []}
                                patchList.append(fileDic)
                                # print(patchList)
                            elif line.startswith("@@ -"):
                                location = line.split(' ')[1].lstrip('-')
                                try:
                                    loctup = (
                                        int(location.split(',')[0]),
                                        int(location.split(',')[0]) + int(location.split(',')[1]) - 1)
                                except:
                                    loctup = (int(location), int(location))
                                blockDic = {"location": loctup, "lines": []}
                                patchList[-1]["blocks"].append(blockDic)
                                # print(patchList)
                            elif line.startswith("diff ") or line.startswith("+++ ") or line.startswith("index "):
                                continue
                            else:
                                try:
                                    if len(patchList[-1]["blocks"][-1]["lines"]) == 0:
                                        line = {"location": patchList[-1]["blocks"][-1]["location"][0], "content": line}
                                    else:
                                        if line.startswith("+"):
                                            if patchList[-1]["blocks"][-1]["lines"][-1]["content"].startswith('+'):
                                                line = {
                                                    "location": patchList[-1]["blocks"][-1]["lines"][-1]["location"],
                                                    "content": line}
                                            else:
                                                line = {"location": patchList[-1]["blocks"][-1]["lines"][-1][
                                                                        "location"] + 0.5,
                                                        "content": line}
                                        else:
                                            line = {"location": int(
                                                patchList[-1]["blocks"][-1]["lines"][-1]["location"] + 1),
                                                    "content": line}
                                    patchList[-1]["blocks"][-1]["lines"].append(line)
                                except:
                                    break
                        else:
                            continue
                    for patchFile in patchList:
                        ext = patchFile["fileName"].rsplit('/')[-1].lower()
                        if ext.endswith('.c') or ext.endswith('.cpp') or ext.endswith('.cc') or ext.endswith(
                                '.c++') or ext.endswith('.cxx'):
                            functionInstanceList = ca.parseFile_source(
                                unipath + '\\' + patchFile["fileName"])
                            funcLs = extractFuncLs(functionInstanceList)
                            # print(funcLs)
                            # print(patchFile)
                            patchFuncs = {"fileName": patchFile["fileName"], "funcPatchs": {}}
                            for block in patchFile["blocks"]:
                                for line in block["lines"]:
                                    if line["content"].startswith("+") or line["content"].startswith("-"):
                                        lineLoc = line["location"]
                                        print(line["content"])
                                        for func in funcLs:
                                            if lineLoc < func[1][0]:
                                                break
                                            elif lineLoc > func[1][1]:
                                                continue
                                            else:
                                                if func[0] not in patchFuncs["funcPatchs"]:
                                                    patchFuncs["funcPatchs"][func[0]] = {"instance": func[2], '+': "",
                                                                                         '-': ""}
                                                    patchFuncs["funcPatchs"][func[0]][(line["content"][0])] += (
                                                                line["content"][1:] + '\n')
                                                else:
                                                    patchFuncs["funcPatchs"][func[0]][(line["content"][0])] += (
                                                                line["content"][1:] + '\n')
                                        print(patchFuncs["funcPatchs"][func[0]][(line["content"][0])])
                            #print(patchFuncs)
                            for funcName, funcPatch in patchFuncs["funcPatchs"].items():
                                instance, adds, subs = funcPatch["instance"], funcPatch["+"], funcPatch["-"]
                                vul_ab = ca.abstract(instance)[1]
                                vul_ab = ca.normalize(vul_ab)
                                add_ab = ca.abstract(instance, adds)[1]
                                sub_ab = ca.abstract(instance, subs)[1]
                                adds_ab = add_ab.split('\n')
                                subs_ab = sub_ab.split('\n')
                                adds_nor = []
                                subs_nor = []
                                for add in adds_ab:
                                    add = ca.normalize(add)
                                    if add != '':
                                        adds_nor.append(add)
                                for sub in subs_ab:
                                    sub = ca.normalize(sub)
                                    if sub != '':
                                        subs_nor.append(sub)
                                vul_fingerprint.append(
                                    {"CVE": dir,
                                     "fileName": (targetPath.split("\\")[-2]+ '\\' +targetPath.split("\\")[-1]+ '\\' + dir+'\\' + patchFuncs["fileName"]).replace('\\', '/'), "funcName": funcName,
                                     "funcLoc": funcPatch["instance"].lines, "bodyHash": sd.ssdeep_hash(vul_ab),
                                     "addLines": adds_nor, "subLines": subs_nor})
                u.close()
            except:
                pass
        else:
            for item in os.listdir(unipath):
                patchFile = unipath + "\\" + item + "\\.diff"
        #print(patchFile)
                try:
                    with open(patchFile, 'r', encoding='utf-8') as u:
                        patchList = []
                        flag = False
                        for line in u.readlines():
                            #print(line)
                            line = line.rstrip('\n')  # 可能会出错
                            #try:
                            if line.startswith("diff --git"):
                                flag = True
                            if flag:
                                if line.startswith("--- "):
                                    fileName = line.split('/',1)[1]
                                    fileDic = {"fileName": fileName, "blocks": []}
                                    patchList.append(fileDic)
                                    #print(patchList)
                                elif line.startswith("@@ -"):
                                    location = line.split(' ')[1].lstrip('-')
                                    try:
                                        loctup = (
                                        int(location.split(',')[0]), int(location.split(',')[0]) + int(location.split(',')[1]) - 1)
                                    except:
                                        loctup = (int(location), int(location))
                                    blockDic = {"location": loctup, "lines": []}
                                    patchList[-1]["blocks"].append(blockDic)
                                    #print(patchList)
                                elif line.startswith("diff ") or line.startswith("+++ ") or line.startswith("index "):
                                    continue
                                else:
                                    try:
                                        if len(patchList[-1]["blocks"][-1]["lines"]) == 0:
                                            line = {"location": patchList[-1]["blocks"][-1]["location"][0], "content": line}
                                        else:
                                            if line.startswith("+"):
                                                if patchList[-1]["blocks"][-1]["lines"][-1]["content"].startswith('+'):
                                                    line = {"location": patchList[-1]["blocks"][-1]["lines"][-1]["location"] ,
                                                            "content": line}
                                                else:
                                                    line = {"location": patchList[-1]["blocks"][-1]["lines"][-1]["location"] + 0.5,
                                                            "content": line}
                                            else:
                                                line = {"location": int(patchList[-1]["blocks"][-1]["lines"][-1]["location"] + 1),
                                                        "content": line}
                                        patchList[-1]["blocks"][-1]["lines"].append(line)
                                    except:
                                        break
                            else:
                                continue
                        for patchFile in patchList:
                            ext = patchFile["fileName"].rsplit('/')[-1].lower()
                            #print(ext)
                            if ext.endswith('.c') or ext.endswith('.cpp') or ext.endswith('.cc') or ext.endswith(
                                    '.c++') or ext.endswith('.cxx'):
                                functionInstanceList = ca.parseFile_source(unipath + '\\' + item + "\\" + patchFile["fileName"])
                                funcLs = extractFuncLs(functionInstanceList)
                                #print(funcLs)
                                #print(patchFile)
                                patchFuncs = {"fileName": patchFile["fileName"], "funcPatchs": {}}
                                for block in patchFile["blocks"]:
                                    for line in block["lines"]:
                                        if line["content"].startswith("+") or line["content"].startswith("-"):
                                            lineLoc = line["location"]
                                            #print(line["content"])
                                            for func in funcLs:
                                                if lineLoc < func[1][0]:
                                                    break
                                                elif lineLoc > func[1][1]:
                                                    continue
                                                else:
                                                    if func[0] not in patchFuncs["funcPatchs"]:
                                                        patchFuncs["funcPatchs"][func[0]] = {"instance": func[2], '+':"", '-':""}
                                                        patchFuncs["funcPatchs"][func[0]][(line["content"][0])]+=(line["content"][1:]+'\n')
                                                    else:
                                                        patchFuncs["funcPatchs"][func[0]][(line["content"][0])]+=(line["content"][1:]+'\n')
                                            #print(patchFuncs["funcPatchs"])
                                #print(patchFuncs)
                                for funcName, funcPatch in patchFuncs["funcPatchs"].items():
                                    instance, adds, subs = funcPatch["instance"], funcPatch["+"], funcPatch["-"]
                                    vul_ab = ca.abstract(instance)[1]
                                    print(vul_ab)
                                    vul_ab = ca.normalize(vul_ab)
                                    add_ab = ca.abstract(instance, adds)[1]
                                    sub_ab = ca.abstract(instance, subs)[1]
                                    adds_ab = add_ab.split('\n')
                                    subs_ab = sub_ab.split('\n')

                                    #print(adds)
                                    print(adds_ab)
                                    adds_nor = []
                                    subs_nor = []
                                    for add in adds_ab:
                                        add = ca.normalize(add)
                                        if add != '':
                                            adds_nor.append(add)
                                    for sub in subs_ab:
                                        sub = ca.normalize(sub)
                                        if sub != '':
                                            subs_nor.append(sub)
                                    vul_fingerprint.append({"CVE": dir, "fileName":(targetPath.split("\\")[-2]+ '\\' +targetPath.split("\\")[-1]+ '\\' + dir+'\\' + item + '\\' + patchFuncs["fileName"]).replace('\\', '/'), "funcName": funcName,
                                                            "funcLoc": funcPatch["instance"].lines, "bodyHash": sd.ssdeep_hash(vul_ab),
                                                            "addLines": adds_nor, "subLines": subs_nor})
                                    #print(vul_fingerprint)
                    u.close()
                except:
                    pass
    return vul_fingerprint

patchToFunc("E:\\test")
'''
def vul_generator(targetPath):
    json_str = json.dumps(patchToFunc(targetPath), indent=4)
    #print(json_str)
    with open(targetPath+'\\vul_fingerprint3.json', 'w') as json_file:
        json_file.write(json_str)
    json_file.close()

path = "E:\\vulData"
flag = 0
for dir in os.listdir(path):
    # if dir == "torvalds":
    #     flag = 1
    # if flag == 0:
    #     continue
    for item in os.listdir(path+"\\"+dir):
        vul_generator(path+"\\"+dir+"\\"+item)
        

functionInstanceList = ca.parseFile_source("E:\\data\\git.videolan.org\\CVE-2011-3934\\[ffmpeg.git]\\libavcodec\\vp3.c")
funcLs = extractFuncLs(functionInstanceList)
print(funcLs)
'''