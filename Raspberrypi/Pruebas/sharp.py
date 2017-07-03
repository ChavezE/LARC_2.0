import sys
sys.path.insert(0, '../lib/')
import Larc_vision_2017 as rb
import communication as com
import time
if __name__ == '__main__':
    t = time.time()
    com.getSharpDistance('0')
    print (time.time() - t)
