import os
import sys

if __name__ == "__main__":
  ifs = sys.argv[1]
  ofs = sys.argv[2]
  os.system("sort %s -o %s" % (ifs, ofs))
  
  
