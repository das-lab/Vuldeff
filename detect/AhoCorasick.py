# !/usr/bin/env python
# -*- coding:utf-8 -*-

"""
@Author   :geekzw
@Contact  :1223242863@qq.com
@File     :AC_ner.py
@Time     :2020/3/5 12:56 AM
@Software :Pycharm
@Copyright (c) 2020,All Rights Reserved.
"""

import time


class node(object):
    def __init__(self):
        self.next = {}  # 相当于指针，指向树节点的下一层节点
        self.fail = None  # 失配指针，这个是AC自动机的关键
        self.isWord = False  # 标记，用来判断是否是一个标签的结尾
        self.word = ""  # 用来储存标签


class ac_automation(object):
    def __init__(self, list):
        self.root = node()
        self.list = list

    def add(self, word):
        temp_root = self.root
        for char in word:
            if char not in temp_root.next:
                temp_root.next[char] = node()
            temp_root = temp_root.next[char]
        temp_root.isWord = True
        temp_root.word = word

    # 添加文件中的关键词
    def add_keyword(self):
        for key in self.list:
            self.add(key)

    def make_fail(self):
        temp_que = []
        temp_que.append(self.root)
        while len(temp_que) != 0:
            temp = temp_que.pop(0)
            p = None
            for key, value in temp.next.item():
                if temp == self.root:
                    temp.next[key].fail = self.root
                else:
                    p = temp.fail
                    while p is not None:
                        if key in p.next:
                            temp.next[key].fail = p.fail
                            break
                        p = p.fail
                    if p is None:
                        temp.next[key].fail = self.root
                temp_que.append(temp.next[key])

    def search(self, content):
        p = self.root
        #result = []
        result = set()
        index = 0
        while index < len(content) - 1:
            currentposition = index
            while currentposition < len(content):
                word = content[currentposition]
                while word in p.next == False and p != self.root:
                    p = p.fail

                if word in p.next:
                    p = p.next[word]
                else:
                    p = self.root

                if p.isWord:
                    end_index = currentposition + 1
                    result.add(p.word)
                    break
                currentposition += 1
            p = self.root
            index += 1
        return result



