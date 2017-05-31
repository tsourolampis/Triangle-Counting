# Exact triangle counting 


The code counts triangles exactly, code used for exact counting in the paper "Efficient Triangle Counting in Large Graphs via Degree-based Vertex Partitioning" by Mihail N. Kolountzakis, Gary L. Miller, Richard Peng, Charalampos E. Tsourakakis. Code takes as input a compressed graph, as compressed by [Graph-Compression](https://github.com/tsourolampis/Graph-Compression).   

- To compile, run g++ -o triExact triExact.cc
- To run, first compress the graph using ./compress < toy.txt > toycompressed.txt, and then ./triExact < toy_compressed.txt 
- Toy example  

time ./triExact < toy_compressed.txt 
# of triangles encountered: 20666503

real	0m0.961s
user	0m0.834s
sys	    0m0.108s

- One can compare with the code of M. Latapy [available here](https://www-complexnetworks.lip6.fr/~latapy/Triangles/). 
time ./tr -c -cc -n 100 <  latapy_toy.txt 
Found 20666503 triangles...

real	0m1.525s
user	0m1.476s
sys	    0m0.020s