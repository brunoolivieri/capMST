#!/bin/bash

#find data/ -iname *.TXT -exec ./cmst $1 1 $2 {} \;


nohup find data/ -iname *.TXT -exec ./cmst 2 1 $1 {} \; > thread_avl_Q10_UB_$1.txt &
nohup find data/ -iname *.TXT -exec ./cmst 1 1 $1 {} \; > thread_avl_Q5_UB_$1.txt &
nohup find data/ -iname *.TXT -exec ./cmst 0 1 $1 {} \; > thread_avl_Q3_UB_$1.txt &
