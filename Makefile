all:
	g++ -std=c++11 easywsclient/easywsclient.cpp main.cpp -I./easywsclient/ -o socket
