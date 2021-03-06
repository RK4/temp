wget "https://www.dropbox.com/s/7w7zp43tqvszyht/graphs.zip?dl=0" -O graphs.zip
unzip graphs.zip
cd graphs
gzip -d distance.cpp.gz
gzip -d max.cpp.gz
gzip -d ai.cpp.gz
gzip -d rp.cpp.gz
gzip -d ci.cpp.gz
gzip -d cage14.cpp.gz

echo "Compiling graphs"

cd ..
g++ -c ./graphs/distance.cpp
g++ -c ./graphs/max.cpp
g++ -c ./graphs/ai.cpp
g++ -c ./graphs/rp.cpp
g++ -c ./graphs/ci.cpp
g++ -c ./graphs/cage14.cpp

mkdir USA cage14
mv distance.o max.o ai.o ci.o rp.o ./USA
mv cage14.o ./cage14 

echo "Compiling apps"

g++ -c sssp_seq.cpp -std=c++11
g++ -c sssp_seq.cpp -std=c++11; g++ sssp_seq.o ./USA/* -o sssp_seq_usa -lpthread

g++ -c sssp_software_msg.cpp -std=c++11 
g++ -c sssp_software_msg.cpp -std=c++11; g++ sssp_software_msg.o ./USA/* -o sssp_usa -lpthread

g++ -c sssp_seq.cpp -std=c++11 
g++ -c sssp_seq.cpp -std=c++11; g++ sssp_seq.o ./USA/* -o sssp_seq_cage -lpthread

g++ -c sssp_software_msg.cpp -std=c++11
g++ -c sssp_software_msg.cpp -std=c++11; g++ sssp_software_msg.o ./USA/* -o sssp_cage -lpthread


g++ -c bfs_seq.cpp -std=c++11
g++ -c bfs_seq.cpp -std=c++11; g++ bfs_seq.o ./cage14/* -o bfs_seq_usa -lpthread

g++ -c bfs_software_msg.cpp -std=c++11 
g++ -c bfs_software_msg.cpp -std=c++11; g++ bfs_software_msg.o ./cage14/* -o bfs_usa -lpthread

g++ -c bfs_seq.cpp -std=c++11 
g++ -c bfs_seq.cpp -std=c++11; g++ bfs_seq.o ./cage14/* -o bfs_seq_cage -lpthread

g++ -c bfs_software_msg.cpp -std=c++11
g++ -c bfs_software_msg.cpp -std=c++11; g++ bfs_software_msg.o ./cage14/* -o bfs_cage -lpthread

echo "Running"



echo "SSSP USA" >> results
./sssp_seq_usa >> results
echo "SSSP USA 40 9" >> results
./sssp_usa 40 9 >> results
echo "SSSP USA 40 8" >> results
./sssp_usa 40 8 >> results
echo "SSSP USA 40 7" >> results
./sssp_usa 40 7 >> results



echo "SSSP CAGE" >> results
./sssp_seq_cage >> results
echo "SSSP CAGE 40 9" >> results
./sssp_cage 40 9 >> results
echo "SSSP CAGE 40 8" >> results
./sssp_cage 40 8 >> results
echo "SSSP CAGE 40 7" >> results
./sssp_cage 40 7 >> results




echo "BFS USA" >> results
./bfs_seq_usa >> results
echo "BFS USA 40 9" >> results
./bfs_usa 40 9 >> results
echo "BFS USA 40 8" >> results
./bfs_usa 40 8 >> results
echo "BFS USA 40 7" >> results
./bfs_usa 40 7 >> results



echo "BFS CAGE" >> results
./bfs_seq_cage >> results
echo "BFS CAGE 40 9" >> results
./bfs_cage 40 9 >> results
echo "BFS CAGE 40 8" >> results
./bfs_cage 40 8 >> results
echo "BFS CAGE 40 7" >> results
./bfs_cage 40 7 >> results

git clone https://github.com/serifyesil/PMOD.git
