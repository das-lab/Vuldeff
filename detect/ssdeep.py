# -*- coding: utf-8 -*-

import numpy as np
import collections
import doctest
import pprint


def INSERTION(A, cost=1):
    return cost


def DELETION(A, cost=1):
    return cost


def SUBSTITUTION(A, B, cost=1):
    return cost


Trace = collections.namedtuple("Trace", ["cost", "ops"])


class WagnerFischer(object):
    # Initializes pretty printer (shared across all class instances).
    pprinter = pprint.PrettyPrinter(width=75)

    def __init__(self,
                 A,
                 B,
                 insertion=INSERTION,
                 deletion=DELETION,
                 substitution=SUBSTITUTION):
        # Stores cost functions in a dictionary for programmatic access.
        self.costs = {"I": insertion, "D": deletion, "S": substitution}
        # Initializes table.
        self.asz = len(A)
        self.bsz = len(B)
        self._table = [[None for _ in range(self.bsz + 1)]
                       for _ in range(self.asz + 1)]
        # From now on, all indexing done using self.__getitem__.
        ## Fills in edges.
        self[0][0] = Trace(0, {"O"})  # Start cell.
        for i in range(1, self.asz + 1):
            self[i][0] = Trace(self[i - 1][0].cost + self.costs["D"](A[i - 1]),
                               {"D"})
        for j in range(1, self.bsz + 1):
            self[0][j] = Trace(self[0][j - 1].cost + self.costs["I"](B[j - 1]),
                               {"I"})
        ## Fills in rest.
        for i in range(len(A)):
            for j in range(len(B)):
                # Cleans it up in case there are more than one check for match
                # first, as it is always the cheapest option.
                if A[i] == B[j]:
                    self[i + 1][j + 1] = Trace(self[i][j].cost, {"M"})
                # Checks for other types.
                else:
                    costD = self[i][j + 1].cost + self.costs["D"](A[i])
                    costI = self[i + 1][j].cost + self.costs["I"](B[j])
                    costS = self[i][j].cost + self.costs["S"](A[i], B[j])
                    min_val = min(costI, costD, costS)
                    trace = Trace(min_val, set())
                    # Adds _all_ operations matching minimum value.
                    if costD == min_val:
                        trace.ops.add("D")
                    if costI == min_val:
                        trace.ops.add("I")
                    if costS == min_val:
                        trace.ops.add("S")
                    self[i + 1][j + 1] = trace
        # Stores optimum cost as a property.
        self.cost = self[-1][-1].cost

    def __repr__(self):
        return self.pprinter.pformat(self._table)

    def __iter__(self):
        for row in self._table:
            yield row

    def __getitem__(self, i):
        """
        Returns the i-th row of the table, which is a list and so
        can be indexed. Therefore, e.g.,  self[2][3] == self._table[2][3]
        """
        return self._table[i]

    # Stuff for generating alignments.

    def _stepback(self, i, j, trace, path_back):
        """
        Given a cell location (i, j) and a Trace object trace, generate
        all traces they point back to in the table
        """
        for op in trace.ops:
            if op == "M":
                yield i - 1, j - 1, self[i - 1][j - 1], path_back + ["M"]
            elif op == "I":
                yield i, j - 1, self[i][j - 1], path_back + ["I"]
            elif op == "D":
                yield i - 1, j, self[i - 1][j], path_back + ["D"]
            elif op == "S":
                yield i - 1, j - 1, self[i - 1][j - 1], path_back + ["S"]
            elif op == "O":
                return  # Origin cell, so we"re done.
            else:
                raise ValueError("Unknown op {!r}".format(op))

    def alignments(self):
        """
        Generate all alignments with optimal-cost via breadth-first
        traversal of the graph of all optimal-cost (reverse) paths
        implicit in the dynamic programming table
        """
        # Each cell of the queue is a tuple of (i, j, trace, path_back)
        # where i, j is the current index, trace is the trace object at
        # this cell, and path_back is a reversed list of edit operations
        # which is initialized as an empty list.
        queue = collections.deque(
            self._stepback(self.asz, self.bsz, self[-1][-1], []))
        while queue:
            (i, j, trace, path_back) = queue.popleft()
            if trace.ops == {"O"}:
                # We have reached the origin, the end of a reverse path, so
                # yield the list of edit operations in reverse.
                yield path_back[::-1]
                continue
            queue.extend(self._stepback(i, j, trace, path_back))

    def IDS(self):
        """
        Estimates insertions, deletions, and substitution _count_ (not
        costs). Non-integer values arise when there are multiple possible
        alignments with the same cost.
        """
        npaths = 0
        opcounts = collections.Counter()
        for alignment in self.alignments():
            # Counts edit types for this path, ignoring "M" (which is free).
            opcounts += collections.Counter(op for op in alignment
                                            if op != "M")
            npaths += 1
        # Averages over all paths.
        return collections.Counter({o: c / npaths
                                    for (o, c) in opcounts.items()})


FNV_PRIME = 0x01000193
FNV_INIT = 0x28021967
MAX_LENGTH = 64
B64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"


class Last7chars(object):
    def __init__(self):
        self._reset_rollhash()

    def _reset_rollhash(self):
        self.roll_h1 = 0
        self.roll_h2 = 0
        self.roll_h3 = 0
        self.ringbuffer = [0] * 7
        self.writeindex = 0

    def _roll_hash(self, char):
        char7bf = self.readwrite(char)
        self.roll_h2 += 7 * char - self.roll_h1
        self.roll_h1 += char - char7bf
        self.roll_h3 <<= 5
        self.roll_h3 &= 0xffffffff
        self.roll_h3 ^= char
        return self.roll_h1 + self.roll_h2 + self.roll_h3

    def readwrite(self, num):
        retval = self.ringbuffer[self.writeindex]
        self.ringbuffer[self.writeindex] = num
        self.writeindex = (self.writeindex + 1) % 7
        return retval

    def __repr__(self):
        arr = self.ringbuffer[
            self.writeindex:] + self.ringbuffer[:self.writeindex]
        return " ".join(map(str, arr))


def _update_fnv(fnvhasharray, newchar):
    fnvhasharray *= FNV_PRIME
    fnvhasharray &= 0xffffffff#取最低32位
    fnvhasharray ^= newchar
    return fnvhasharray


def _calc_initbs(length):
    bs = 3
    while bs * MAX_LENGTH < length:
        bs *= 2

    if bs > 3:  #proably checking for integer overflow here?
        return bs
    return 3


def ssdeep_hash(content):
    bs = _calc_initbs(len(content))
    #print "bs: ", bs

    hash1 = ''
    hash2 = ''

    last7chars = Last7chars()

    while True:
        last7chars._reset_rollhash()
        fnv1 = FNV_INIT
        fnv2 = FNV_INIT
        hash1 = ''
        hash2 = ''
        fnvarray = np.array([fnv1, fnv2])

        for i in range(len(content)):   # 逐bytes扫描
            c = ord(content[i])
            # 使用Alder-32 算法作为弱哈希。
            h = last7chars._roll_hash(c)
            fnvarray = _update_fnv(fnvarray, c)

            # 当Alder-32哈希值除以n的余数恰好等于n-1时，就在当前位置分片；
            # 否则，不分片，窗口往后滚动一个字节，
            # 然后再次计算Alder-32哈希值并判断，如此继续
            # 1. 使用bs作为分片值
            if h % bs == (bs - 1) and len(hash1) < (MAX_LENGTH - 1):
                # 对每片分别计算哈希，使用Fowler-Noll-Vo hash哈希算法
                b64char = B64[fnvarray[0] & 63]
                hash1 += b64char
                fnvarray[0] = FNV_INIT
            # 2. 使用2*bs作为分片值
            if h % (2 * bs) == (2 * bs - 1) and len(hash2) < (
                    MAX_LENGTH / 2 - 1):
                b64char = B64[fnvarray[1] & 63]
                hash2 += b64char
                fnvarray[1] = FNV_INIT
        # 将每片压缩后的哈希值连接到一起，就得到该函数的模糊哈希值
        hash1 += B64[fnvarray[0] & 63]
        hash2 += B64[fnvarray[1] & 63]  # 这里 &63，等价于取最低6bit


        if bs <= 3 or len(hash1) > (MAX_LENGTH / 2):
            break
        bs = int(bs / 2)
        if bs < 3:
            bs = 3
    # 对每一个文件，它同时使用n和n/2作为分片值，算得两个不同的模糊哈希值，而这两个值都使用。
    # 因此，最后得到的一个文件的模糊哈希值是: n:h(n):h(n/2)
    return ':'.join([str(bs), hash1, hash2])


#from https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Longest_common_substring#Python_2
def longest_common_substring(s1, s2):
    m = [[0] * (1 + len(s2)) for i in range(1 + len(s1))]
    longest, x_longest = 0, 0
    for x in range(1, 1 + len(s1)):
        for y in range(1, 1 + len(s2)):
            if s1[x - 1] == s2[y - 1]:
                m[x][y] = m[x - 1][y - 1] + 1
                if m[x][y] > longest:
                    longest = m[x][y]
                    x_longest = x
            else:
                m[x][y] = 0
    return s1[x_longest - longest:x_longest]


def _likeliness(min_lcs, a, b):
    # 如果最长公共子串长度不满足要求，则直接退出
    if len(longest_common_substring(a, b)) < min_lcs:
        return 0

    # Wagner Fischer算法
    dist = WagnerFischer(a, b).cost
    # 将这个距离除以s1和s2的长度和，以将绝对结果变为相对结果，
    # 再映射到0-100的一个整数值上，其中，100表示两个字符串完全一致，而0表示完全不相似
    dist = int(dist * MAX_LENGTH / (len(a) + len(b)))
    dist = int(100 * dist / 64)
    if dist > 100:
        dist = 100
    return 100 - dist


def ssdeep_compare(hashA, hashB, min_lcs=3):
    bsA, hs1A, hs2A = hashA.split(':')  #blocksize, hash1, hash2
    bsB, hs1B, hs2B = hashB.split(':')

    bsA = int(bsA)
    bsB = int(bsB)

    like = 0

    # 在比较时，如果两个函数的分片值分别为n和m，则判断是否有n==m, n==2m, 2n==m三种情况，
    # 如果有之一，则将两者相应的模糊哈希值进行比较。例如，如果n==2m，则比较h(n/2)与h(m)是否相似
    #块尺寸比较
    if bsA == bsB:
        #compare both hashes
        like1 = _likeliness(min_lcs, hs1A, hs1B)
        like2 = _likeliness(min_lcs, hs2A, hs2B)
        like = max(like1, like2)
    elif bsA == 2 * bsB:
        # Compare hash_bsA with hash_2*bsB
        like = _likeliness(min_lcs, hs1A, hs2B)
    elif 2 * bsA == bsB:
        # Compare hash_2*bsA with hash_bsB
        like = _likeliness(min_lcs, hs2A, hs1B)
    else:
        like = 0
    return like
'''
body =

line = "static int codec_set_pass_key(sqlite3* db, int nDb, const void *zKey, int nKey, int for_ctx) {"
print(ssdeep_hash(body))
print(ssdeep_hash(line))
'''
