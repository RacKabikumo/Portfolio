import time
import threading
import multiprocessing as mp
from multiprocessing import Pool
from datetime import datetime,timezone,timedelta
from multiprocessing.dummy import Pool as ThreadPool


class Proj(object):
    def __init__(self, num, k):
        self.number = num
        self.sortNum = []
        self.k = k
    def BubbleSort_2( self, i, size ) :
        while size > i :     
            j = i              # 只要執行 size-1 次-->最後一個不用排
            while j < size-1 :             # 從第1個開始比較直到倒數第二個 
                if self.number[j] > self.number[j+1]:        # 比大小然後互換
                    self.number[j], self.number[j+1] = self.number[j+1], self.number[j] # 前面跟前面換後面跟後面換
                j = j + 1
            size = size - 1

    def BubbleSort_3( self, number ) :
        i = 0
        size = len( number )
        while size > i :    
            j = i              # 只要執行 size-1 次-->最後一個不用排
            while j < size-1 :             # 從第1個開始比較直到倒數第二個 
                if number[j] > number[j+1]:        # 比大小然後互換
                    number[j], number[j+1] = number[j+1], number[j] # 前面跟前面換後面跟後面換
                j = j + 1
            size = size - 1

        return number

        


    def MergeSort(self, start, end):
        if start < end :
            mid = int( ( start + end ) / 2 )
            self.MergeSort( start, mid )
            self.MergeSort( mid + 1, end )
            self.Merge( start, mid, end )
            
    def MergeSort_P(self, lists, start, end):
        if start < end :
            mid = int( ( start + end ) / 2 )
            self.MergeSort_P( lists, start, mid )
            self.MergeSort_P( lists, mid + 1, end )
            self.Merge_P( lists, start, mid, end )

    def Merge_P(self, number, start, mid, end):
        left = number[start:mid+1]
        right = number[mid+1:end+1]
        i = 0
        j = 0
        while i < len(left) and j < len(right) :
            if left[i] <= right[j] :
                number[start] = left[i]
                i = i + 1
            else :
                number[start] = right[j]
                j = j + 1
            start = start + 1

        while i < len(left) :
            number[start] = left[i]
            i = i + 1
            start = start + 1
        
        while j < len(right) :
            number[start] = right[j]
            j = j + 1
            start = start + 1

    def MergeSort_3(self, lists):
        self.MergeSort_P( lists, 0, len(lists) )
        return lists
   
    def Merge(self, start, mid, end):
        left = self.number[start:mid+1]
        right = self.number[mid+1:end+1]
        i = 0
        j = 0
        while i < len(left) and j < len(right) :
            if left[i] <= right[j] :
                self.number[start] = left[i]
                i = i + 1
            else :
                self.number[start] = right[j]
                j = j + 1
            start = start + 1

        while i < len(left) :
            self.number[start] = left[i]
            i = i + 1
            start = start + 1
        
        while j < len(right) :
            self.number[start] = right[j]
            j = j + 1
            start = start + 1

    def run_4( self, fileName ) :
        lists = []
        start = 0
        size = int( len(self.number) / self.k )
        start_time = time.time()
        for i in range(self.k):
            if ( i == self.k-1 ) :
                size = len(self.number) - start
            last = start + size
            self.BubbleSort_2(start,last)
            lists.append( self.number[start:last] ) # 要拿size個
            start = last
        i = 0
        start = 0
        while ( len(lists) > 0 ) :
            if len(lists) > 1 :
                size = start + len(lists[0]) + len(lists[1]) - 1 # 幾個數字
            else :
                break
            if size >= len(self.number) :
                start = 0
                size = start + len(lists[0]) + len(lists[1]) - 1 # 幾個數字
            self.MergeSort(start,size)
            if len(lists) > 1 :
                lists.pop(0)
            lists.pop(0)
            lists.append( self.number[start:size+1] )
            start = size + 1
            i = i + 1
        end_time = time.time()
        cpuTime = end_time - start_time
        Output( fileName, self.number, "4", cpuTime )

    def run_3( self, fileName ) :
        lists = []
        start = 0
        last = 0
        size = int( len(self.number) / self.k )
        pool = Pool()
        arr = []
        start_time = time.time()
        for i in range(self.k):
            if ( i == self.k-1 ) :
                size = len(self.number) - start
            last = start + size
            number = self.number[start:last]
            lists.append( number ) # 要拿size個
            start = last

        x = pool.map(self.BubbleSort_3, lists )
        self.number = []
        for it in x :
            self.number = self.number + it

        pool.close()
        i = 0
        start = 0
        pool = Pool()
        while ( len(lists) > 0 ) :
            if len(lists) > 1 :
                size = start + len(lists[0]) + len(lists[1]) - 1 # 幾個數字
            else :
                break
            if size >= len(self.number) :
                start = 0
                size = start + len(lists[0]) + len(lists[1]) - 1 # 幾個數字
            number = self.number[start:size+1]

            arr.append(number)
            if len(lists) > 1 :
                lists.pop(0)
            lists.pop(0)
            lists.append( number )
            start = size + 1
            i = i + 1
        x = pool.map(self.MergeSort_3, arr )
        self.number = x[len(x)-1]
        pool.close()

        end_time = time.time()
        cpuTime = end_time - start_time
        Output( fileName, self.number, "3", cpuTime )

    def run_2( self, fileName ):
        lists = []
        start = 0
        start_time = time.time()
        size = 0
        size = int( len(self.number) / self.k )
        last = 0
        pool = ThreadPool(self.k)
        arr = []
        for i in range(self.k):
            if ( i == self.k-1 ) :
                size = len(self.number) - start
            last = start + size
            number = self.number[start:last]
            lists.append( number ) # 要拿size個
            start = last
        
        x = pool.map(self.BubbleSort_3, lists)
        pool.close()
        self.number = []
        for it in x :
            self.number = self.number + it

        i = 0
        start = 0
        while ( len(lists) > 0 ) :
            if len(lists) > 1 :
                size = start + len(lists[0]) + len(lists[1]) - 1 # 幾個數字
            else :
                break
            if size >= len(self.number) :
                start = 0
                size = start + len(lists[0]) + len(lists[1]) - 1 # 幾個數字
            number = self.number[start:size+1]
            arr.append(number)
            if len(lists) > 1 :
                lists.pop(0)
            lists.pop(0)
            lists.append( number )
            start = size + 1
            i = i + 1

        pool = ThreadPool(len(arr))
        x = pool.map(self.MergeSort_3, arr)
        pool.close()
        self.number = x[len(x)-1]
        end_time = time.time()
        cpuTime = end_time - start_time
        Output( fileName, self.number, "2", cpuTime )


        
    
    


def GetInput( fileName ) :
    fileName = fileName + ".txt"
    f = open( fileName, "r" )
    input_number = f.read()
    arr = input_number.split()
    number = []
    for item in arr :
        number.append( int(item) )
    f.close()
    return number

def Output( fileName, number, num, cpuTime ) :
    fileName = fileName + "_output" + num + ".txt"
    f = open( fileName, "w" )
    f.write( "Sort :\n" )
    for item in number :
        f.write( str(item) )
        f.write( "\n" )
    f.write( "CPU Time : " )
    f.write( str(cpuTime) )
    f.write( "\nOutput Time : " )
    date_now = datetime.utcnow().replace( tzinfo = timezone.utc )
    date_UTC = date_now.astimezone( timezone( timedelta( hours = 8 ) ) ) # 轉換時區
    f.write( str( date_UTC ) )
    f.close()

def BubbleSort( fileName, number ) :
    start = time.time()
    size = len(number)
    for i in range(size-2):                   # 但只要執行 size-1 次-->最後一個不用排
        for j in range(size-i-1):             # 從第1個開始比較直到倒數第二個 
            if number[j] > number[j+1]:        # 比大小然後互換
                number[j], number[j+1] = number[j+1], number[j] # 前面跟前面換後面跟後面換
    end = time.time()
    cpuTime = end - start
    Output( fileName, number, "1", cpuTime )
    
    

if __name__ == '__main__' :
    fileName = input( "請輸入檔名 : " )
    k = input( "請輸入要分成幾份 : " )
    k = int(k)
    number = GetInput( fileName )
    
    number_2 = number.copy()
    BubbleSort( fileName, number_2 )

    number_2 = number.copy()
    work2 = Proj( number_2, k )
    work2.run_2(fileName)

    number_2 = number.copy()
    work3 = Proj( number_2, k )
    work3.run_3( fileName )

    number_2 = number.copy()
    work4 = Proj( number_2, k )
    work4.run_4( fileName )


