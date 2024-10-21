/home/yuwei/Documents/ShadowDash/src

export LD_LIBRARY_PATH=/home/yuwei/Documents/ShadowDash/build:$LD_LIBRARY_PATH

g++ -I /home/yuwei/Documents/ShadowDash/src -L /home/yuwei/Documents/ShadowDash/build -lninja_shared -o test1 test1.cc
or
g++ -I /home/yuwei/Documents/ShadowDash/src -L /home/yuwei/Documents/ShadowDash/build -lninja_shared -Wl,-rpath,/home/yuwei/Documents/ShadowDash/build -o test1 test1.cc