#include <iostream>

#include "phoenix_socket.hpp"

int main(int argc, char *argv[])
{
  PhoenixChannel channel("ws://localhost:4000/socket/websocket");

  while(true) {}
  
  // WebSocket::pointer ws = WebSocket::from_url("ws://localhost:4000/socket/websocket");

  // while(true)
  //   {
  //     ws->poll();
  //     ws->send("{\"topic\": \"phoenix\", \"event\": \"heartbeat\", \"payload\": \"\", \"ref\": 1}");
  //     ws->dispatch([](const std::string &msg)
  // 		   {
  // 		     std::cout << msg << std::endl;
  // 		   });
  //   }

  // delete ws;

  return 0;
}
