rm a.out
make
g++ TCPserver.cpp chabot.cpp -I /usr/include libchabot.a /usr/lib/x86_64-linux-gnu/libboost_thread.so /usr/lib/x86_64-linux-gnu/libboost_system.so -o server.o
