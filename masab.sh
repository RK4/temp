wget "https://www.dropbox.com/s/7w7zp43tqvszyht/graphs.zip?dl=0" -O graphs.zip
unzip graphs.zip
cd graphs
g++ -c distance.cpp.gz
g++ -c max.cpp.gz
g++ -c ai.cpp.gz
g++ -c rp.cpp.gz
g++ -c ci.cpp.gz
g++ -c cage14.cpp.gz

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

g++ -c sssp_seq.cpp -std=c++11
g++ -c sssp_seq.cpp -std=c++11; g++ sssp_seq.o ./USA/* -o sssp_seq_usa

g++ -c sssp_software_msg.cpp -std=c++11 
g++ -c sssp_software_msg.cpp -std=c++11; g++ sssp_software_msg.o ./USA/* -o sssp_usa

g++ -c sssp_seq.cpp -std=c++11 
g++ -c sssp_seq.cpp -std=c++11; g++ sssp_seq.o ./USA/* -o sssp_seq_cage

g++ -c sssp_software_msg.cpp -std=c++11
g++ -c sssp_software_msg.cpp -std=c++11; g++ sssp_software_msg.o ./USA/* sssp_cage


g++ -c bfs_seq.cpp -std=c++11
g++ -c bfs_seq.cpp -std=c++11; g++ bfs_seq.o ./cage14/* -o bfs_seq_usa

g++ -c bfs_software_msg.cpp -std=c++11 
g++ -c bfs_software_msg.cpp -std=c++11; g++ bfs_software_msg.o ./cage14/* -o bfs_usa

g++ -c bfs_seq.cpp -std=c++11 
g++ -c bfs_seq.cpp -std=c++11; g++ bfs_seq.o ./cage14/* -o bfs_seq_cage

g++ -c bfs_software_msg.cpp -std=c++11
g++ -c bfs_software_msg.cpp -std=c++11; g++ bfs_software_msg.o ./cage14/* bfs_cage

echo "SSSP USA" >> results
./sssp_seq_usa >> results
echo "SSSP USA 64 9" >> results
./sssp_usa 64 9 >> results
echo "SSSP USA 64 8" >> results
./sssp_usa 64 8 >> results
echo "SSSP USA 64 7" >> results
./sssp_usa 64 7 >> results

echo "SSSP CAGE" >> results
./sssp_seq_cage >> results
echo "SSSP CAGE 64 9" >> results
./sssp_cage 64 9 >> results
echo "SSSP CAGE 64 8" >> results
./sssp_cage 64 8 >> results
echo "SSSP CAGE 64 7" >> results
./sssp_cage 64 7 >> results


echo "BFS USA" >> results
./bfs_seq_usa >> results
echo "BFS USA 64 9" >> results
./bfs_usa 64 9 >> results
echo "BFS USA 64 8" >> results
./bfs_usa 64 8 >> results
echo "BFS USA 64 7" >> results
./bfs_usa 64 7 >> results

echo "BFS CAGE" >> results
./bfs_seq_cage >> results
echo "BFS CAGE 64 9" >> results
./bfs_cage 64 9 >> results
echo "BFS CAGE 64 8" >> results
./bfs_cage 64 8 >> results
echo "BFS CAGE 64 7" >> results
./bfs_cage 64 7 >> results