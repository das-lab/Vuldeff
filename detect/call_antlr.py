# coding=gbk
import os
import subprocess
import re
global osName
global bits
global javaCallCommand
osName = "win"
bits = ""
#base_path = "java -jar E:\\vulDetect\���뼰����\����\\"
javaCallCommand = base_path+"FuncParser-opt.jar "

delimiter = "\r\0?\r?\0\r".encode()
stanfuncs = [
'abort',
'abs',
'acos',
'asctime',
'asctime_r',
'asin',
'assert',
'atan',
'atan2',
'atexit',
'atof',
'atoi',
'atol',
'bsearch',
'btowc',
'calloc',
'catclose6',
'catgets6',
'catopen6',
'ceil',
'clearerr',
'clock',
'cos',
'cosh',
'ctime',
'ctime64',
'ctime_r',
'ctime64_r',
'difftime',
'difftime64',
'div',
'erf',
'erfc',
'exit',
'exp',
'fabs',
'fclose',
'fdopen5',
'feof',
'ferror',
'fflush1',
'fgetc1',
'fgetpos1',
'fgets1',
'fgetwc6',
'fgetws6',
'fileno5',
'floor',
'fmod',
'fopen',
'fprintf',
'fputc1',
'fputs1',
'fputwc6',
'fputws6',
'fread',
'free',
'freopen',
'frexp',
'fscanf',
'fseek1',
'fsetpos1',
'ftell1',
'fwide6',
'fwprintf6',
'fwrite',
'fwscanf6',
'gamma',
'getc1',
'getchar1',
'getenv',
'gets',
'getwc6',
'getwchar6',
'gmtime',
'gmtime64',
'gmtime_r',
'gmtime64_r',
'hypot',
'isalnum',
'isalpha',
'isascii4',
'isblank',
'iscntrl',
'isdigit',
'isgraph',
'islower',
'isprint',
'ispunct',
'isspace',
'isupper',
'iswalnum4',
'iswalpha4',
'iswblank4',
'iswcntrl4',
'iswctype4',
'iswdigit4',
'iswgraph4',
'iswlower4',
'iswprint4',
'iswpunct4',
'iswspace4',
'iswupper4',
'iswxdigit4',
'isxdigit4',
'j0',
'j1',
'jn',
'labs',
'ldexp',
'ldiv',
'localeconv',
'localtime',
'localtime64',
'localtime_r',
'localtime64_r',
'log',
'log10',
'longjmp',
'malloc',
'mblen',
'mbrlen4',
'mbrtowc4',
'mbsinit4',
'mbsrtowcs4',
'mbstowcs',
'mbtowc',
'memchr',
'memcmp',
'memcpy',
'memmove',
'memset',
'mktime',
'mktime64',
'modf',
'nextafter',
'nextafterl',
'nexttoward',
'nexttowardl',
'nl_langinfo4',
'perror',
'pow',
'printf',
'putc1',
'putchar1',
'putenv',
'puts',
'putwc6',
'putwchar6',
'qsort',
'quantexpd32',
'quantexpd64',
'quantexpd128',
'quantized32',
'quantized64',
'quantized128',
'samequantumd32',
'samequantumd64',
'samequantumd128',
'raise',
'rand',
'rand_r',
'realloc',
'regcomp',
'regerror',
'regexec',
'regfree',
'remove',
'rename',
'rewind1',
'scanf',
'setbuf',
'setjmp',
'setlocale',
'setvbuf',
'signal',
'sin',
'sinh',
'snprintf',
'sprintf',
'sqrt',
'srand',
'sscanf',
'strcasecmp',
'strcat',
'strchr',
'strcmp',
'strcoll',
'strcpy',
'strcspn',
'strerror',
'strfmon4',
'strftime',
'strlen',
'strncasecmp',
'strncat',
'strncmp',
'strncpy',
'strpbrk',
'strptime4',
'strrchr',
'strspn',
'strstr',
'strtod',
'strtod32',
'strtod64',
'strtod128',
'strtof',
'strtok',
'strtok_r',
'strtol',
'strtold',
'strtoul',
'strxfrm',
'swprintf',
'swscanf',
'system',
'tan',
'tanh',
'time',
'time64',
'tmpfile',
'tmpnam',
'toascii',
'tolower',
'toupper',
'towctrans',
'towlower4',
'towupper4',
'ungetc1',
'ungetwc6',
'va_arg',
'va_copy',
'va_end',
'va_start',
'vfprintf',
'vfscanf',
'vfwprintf6',
'vfwscanf',
'vprintf',
'vscanf',
'vsprintf',
'vsnprintf',
'vsscanf',
'vswprintf',
'vswscanf',
'vwprintf6',
'vwscanf',
'wcrtomb4',
'wcscat',
'wcschr',
'wcscmp',
'wcscoll4',
'wcscpy',
'wcscspn',
'wcsftime',
'wcslen',
'wcslocaleconv',
'wcsncat',
'wcsncmp',
'wcsncpy',
'wcspbrk',
'wcsptime',
'wcsrchr',
'wcsrtombs4',
'wcsspn',
'wcsstr',
'wcstod',
'wcstod32',
'wcstod64',
'wcstod128',
'wcstof',
'wcstok',
'wcstol',
'wcstold',
'wcstombs',
'wcstoul',
'wcsxfrm4',
'wctob',
'wctomb',
'wctrans',
'wctype4',
'wcwidth',
'wmemchr',
'wmemcmp',
'wmemcpy',
'wmemmove',
'wmemset',
'wprintf6',
'wscanf6',
'y0',
'y1',
'yn'
]
stantype=[
    'char',
    'short',
    'int',
    'long',
    'float',
    'double',
    'unsigned',
    'signed',
    'size_t',
    'ssize_t'
    'struct',
    'union',
    'enum',
    'void',
    'auto',
    'extern',
    'register',
    'static',
    'const',
    'sizeof',
    'typedf',
    'volatile'
]
def loadSource(rootDirectory):
    # returns the list of .src files under the specified root directory.
    maxFileSizeInBytes = None
#    maxFileSizeInBytes = 2*1024*1024  # remove this line if you don't want to restrict
    # the maximum file size that you process.
    walkList = os.walk(rootDirectory)
    srcFileList = []
    for path, dirs, files in walkList:
        for fileName in files:
            ext = fileName.lower()
            if ext.endswith('.c') or ext.endswith('.cpp') or ext.endswith('.cc') or ext.endswith(
                    '.c++') or ext.endswith('.cxx'):
                absPathWithFileName = path + '\\' + fileName
                if maxFileSizeInBytes is not None:
                    if os.path.getsize(absPathWithFileName) < maxFileSizeInBytes:
                        srcFileList.append(absPathWithFileName)
                else:
                    srcFileList.append(absPathWithFileName)
    return srcFileList

def loadPatchCode(rootDirectory):
    # returns the list of .src files under the specified root directory.
    maxFileSizeInBytes = None
#    maxFileSizeInBytes = 2*1024*1024  # remove this line if you don't want to restrict
    # the maximum file size that you process.
    walkList = os.walk(rootDirectory)
    srcFileList = []
    for path, dirs, files in walkList:
        for fileName in files:
            absPathWithFileName = path + '\\' + fileName
            if maxFileSizeInBytes is not None:
                if os.path.getsize(absPathWithFileName) < maxFileSizeInBytes:
                    srcFileList.append(absPathWithFileName)
            else:
                srcFileList.append(absPathWithFileName)
        for dirName in dirs:
            nextRoot = path + '\\' + dirName
            srcFileList.extend(loadPatchCode(nextRoot))
    return srcFileList

class function:
    parentFile = None  # Absolute file which has the function
    parentNumLoc = None  # Number of LoC of the parent file
    name = None  # Name of the function
    lines = None  # Tuple (lineFrom, lineTo) that indicates the LoC of function
    parameterList = []  # list of parameter variables
    variableList = []  # list of local variables
    dataTypeList = []  # list of data types, including user-defined types
    funcCalleeList = []  # list of called functions' names
    funcBody = None

    def __init__(self, fileName):
        self.parentFile = fileName
        self.parameterList = []
        self.variableList = []
        self.dataTypeList = []
        self.dataTypeList2 = []
        self.funcCalleeList = []
    def removeListDup(self):
        # for best performance, must execute this method
        # for every instance before applying the abstraction.
        self.parameterList = list(set(self.parameterList))
        self.variableList = list(set(self.variableList))
        self.dataTypeList = list(set(self.dataTypeList))
        self.dataTypeList2 = list(set(self.dataTypeList2))
        self.funcCalleeList = list(set(self.funcCalleeList))

        # def getOriginalFunction(self):
        #   # returns the original function back from the instance.
        #   fp = open(self.parentFile, 'r')
        #   srcFileRaw = fp.readlines()
        #   fp.close()
        #   return ''.join(srcFileRaw[self.lines[0]-1:self.lines[1]])

def parseFile_vul(srcFileName):
    # this parse vul patch.
    global javaCallCommand
    global delimiter
    addPatchList = ""
    subPatchList = ""
    javaCallCommand += "\"" + srcFileName + "\" 1"
    functionInstanceList = []
    try:
        astString = subprocess.check_output(javaCallCommand, stderr=subprocess.STDOUT, shell=True)
    except subprocess.CalledProcessError as e:
        print ("Parser Error:", e)
        astString = ""
    javaCallCommand = "java -jar E:\\vulDetect\���뼰����\����\\FuncParser-opt.jar "
    funcList = astString.split(delimiter)
    functionInstance = function(srcFileName)
    elemsList = funcList[1].decode(errors = 'ignore').split('\n')[1:-1]
    if len(elemsList) > 9:
        functionInstance.parentNumLoc = int(elemsList[1])
        functionInstance.name = elemsList[2]
        functionInstance.parameterList = elemsList[5].rstrip().split('\t')
        functionInstance.variableList = elemsList[6].rstrip().split('\t')
        functionInstance.dataTypeList = elemsList[7].rstrip().split('\t')
        functionInstance.dataTypeList2 = [words for segments in functionInstance.dataTypeList for words in
                                              segments.split()]
        functionInstance.funcCalleeList = elemsList[8].rstrip().split('\t')
        functionInstance.funcBody = '\n'.join(elemsList[9:])
        functionInstance.removeListDup()
        functionInstanceList.append(functionInstance)
    f = open(srcFileName)
    for line in f.readlines():
        if line.startswith('+'):
            addPatchList = addPatchList + line.lstrip('+') + '\n'
        if line.startswith('-'):
            subPatchList = subPatchList + line.lstrip('-') + '\n'
    f.close()
    return functionInstance, addPatchList, subPatchList

def parseFile_source(srcFileName):
    global javaCallCommand
    global delimiter
    # this parses function definition plus body.
    javaCallCommand += "\"" + srcFileName + "\" 1"
    functionInstanceList = []
    try:
        astString = subprocess.check_output(javaCallCommand, stderr=subprocess.STDOUT, shell=True)
    except subprocess.CalledProcessError as e:
        print ("Parser Error:", e)
        astString = ""
        javaCallCommand = "java -jar E:\\vulDetect\���뼰����\����\\FuncParser-opt.jar "
        return None
    javaCallCommand = "java -jar E:\\vulDetect\���뼰����\����\\FuncParser-opt.jar "
    funcList = astString.split(delimiter)
    for func in funcList[1:]:
        functionInstance = function(srcFileName)
        elemsList = func.decode(errors = 'ignore').split('\n')[1:-1]
        # print elemsList
        if len(elemsList) > 9:
            functionInstance.parentNumLoc = int(elemsList[1])
            functionInstance.name = elemsList[2]
            functionInstance.lines = (int(elemsList[3].split('\t')[0]), int(elemsList[3].split('\t')[1]))
            functionInstance.funcId = int(elemsList[4])
            functionInstance.parameterList = elemsList[5].rstrip().split('\t')
            functionInstance.variableList = elemsList[6].rstrip().split('\t')
            functionInstance.dataTypeList = elemsList[7].rstrip().split('\t')
            functionInstance.dataTypeList2 = [words for segments in functionInstance.dataTypeList for words in segments.split()]
            functionInstance.funcCalleeList = elemsList[8].rstrip().split('\t')
            functionInstance.funcBody = '\n'.join(elemsList[9:])
            functionInstance.removeListDup()
            #print(functionInstance.parentNumLoc)
            #print(functionInstance.name)
            #print(functionInstance.lines)
            #print(functionInstance.funcId)
            #print(functionInstance.parameterList)
            #print(functionInstance.variableList)
            #print(functionInstance.dataTypeList)
            #print(functionInstance.funcCalleeList)
            #print(functionInstance.funcBody)
            #print("-------------------")


            functionInstanceList.append(functionInstance)

    return functionInstanceList

def removeComment(string):
    # Code for removing C/C++ style comments. (Imported from ReDeBug.)
    c_regex = re.compile(
        r'(?P<comment>//.*?$|[{}]+)|(?P<multilinecomment>/\*.*?\*/)|(?P<noncomment>\'(\\.|[^\\\'])*\'|"(\\.|[^\\"])*"|.[^/\'"]*)',
        re.DOTALL | re.MULTILINE)
    return ''.join([c.group('noncomment') for c in c_regex.finditer(string) if c.group('noncomment')])

def normalize(string):
    # Code for normalizing the input string.
    # LF and TAB literals, curly braces, and spaces are removed,
    # and all characters are lowercased.
    return ''.join(string.replace('\n', '').replace('\r', '').replace('\t', '').replace('{', '').replace('}', '').split(
        ' ')).lower()

def abstract(instance, patch = None):
    # �ں���ʵ����Ӧ�ó���Ȼ�󷵻���ԭʼ����ͳ���������ɵ�Ԫ�顣
    originalFunctionBody = instance.funcBody
    if patch != None:
        originalFunctionBody = patch
    originalFunctionBody = removeComment(originalFunctionBody)

    abstractBody = originalFunctionBody

    # �����βγ���
    parameterList = instance.parameterList
    for param in parameterList:
        if len(param) == 0:
            continue
        try:
            paramPattern = re.compile("(^|\W)" + param + "(\W)")
            abstractBody = paramPattern.sub("\g<1>FORPARA\g<2>", abstractBody)
        except:
            pass

    # �Զ����������ͳ���
    dataTypeList = instance.dataTypeList2
    for dtype in dataTypeList:
        if((len(dtype) == 0)|(dtype in stantype)):
            continue
        try:
            dtypePattern = re.compile("(^|\W)" + dtype + "(\W)")
            abstractBody = dtypePattern.sub("\g<1>CUSTYPE\g<2>", abstractBody)
        except:
            pass

    # �ֲ���������
    variableList = instance.variableList
    for lvar in variableList:
        if len(lvar) == 0:
            continue
        try:
            lvarPattern = re.compile("(^|\W)" + lvar + "(\W)")
            abstractBody = lvarPattern.sub("\g<1>LOVAR\g<2>", abstractBody)
        except:
            pass

    # �������ó���
    funcCalleeList = instance.funcCalleeList
    for fcall in funcCalleeList:
        if((len(fcall) == 0)|(fcall in stanfuncs)):
            continue
        try:
            fcallPattern = re.compile("(^|\W)" + fcall + "(\W)")
            abstractBody = fcallPattern.sub("\g<1>FUNCALL\g<2>", abstractBody)
        except:
            pass

    return (originalFunctionBody, abstractBody)

def vul_ab(targetPath):
    directory = targetPath
    fileList = loadPatchCode(directory)
    vul_dic = {}
    for file in fileList:
        instance, adds, subs = parseFile_vul(file)
        vul_ab = abstract(instance)[1]
        vul_ab = normalize(vul_ab)
        add_ab = abstract(instance, adds)[1]
        sub_ab = abstract(instance, subs)[1]
        adds_ab = add_ab.split('\n')
        subs_ab = sub_ab.split('\n')
        adds_nor = []
        subs_nor = []
        for add in adds_ab:
            add = normalize(add)
            if add != '':
                adds_nor.append(add)
        for sub in subs_ab:
            sub = normalize(sub)
            if sub != '':
                subs_nor.append(sub)
        vul_dic[file] = [vul_ab, adds_nor, subs_nor]
    return vul_dic



