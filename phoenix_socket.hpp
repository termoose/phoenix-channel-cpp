#include <iostream>
#include <future>
#include <queue>
#include <memory>

#include "easywsclient.hpp"
#include "json.hpp"

using easywsclient::WebSocket;
using json = nlohmann::json;

class PhoenixChannel
{
public:
  PhoenixChannel(std::string uri)
    : ws(WebSocket::from_url(uri)),
      ref(0),
      running(true),
      run_future(std::async(std::launch::async, &PhoenixChannel::run, this))
  {
    join("rooms:lobby");
  }

  ~PhoenixChannel()
  {
    running = false;
    cond.notify_one();
  }

  void join(std::string topic, std::string payload = std::string())
  {
    std::lock_guard<std::mutex> lock(mutex);
    send(get_join(topic, payload));
  }

private:
  void send(std::string msg)
  {
    std::cout << "-> " << msg << std::endl;
    ws->send(msg);
  }

  unsigned int make_ref()
  {
    if(std::numeric_limits<unsigned int>::max() == ref) ref = 0;
    else ++ref;
    return ref;
  }

  std::string get_heartbeat()
  {
    return json({
	{"topic", "phoenix"},
	{"event", "heartbeat"},
	{"payload", {}},
	{"ref", make_ref()}}
      ).dump();
  }
  
  std::string get_join(std::string topic, std::string payload)
  {
    return json({
	{"topic", topic},
	{"event", "phx_join"},
	{"payload", payload},
	{"ref", make_ref()}}
      ).dump();
  }
  
  void run()
  {
    while(running)
      {
	std::unique_lock<std::mutex> lock(mutex);
	cond.wait_for(lock, std::chrono::milliseconds(300), [&]() { return !running; });
	
	ws->poll();
	ws->send(get_heartbeat());
	ws->dispatch([](const std::string &msg)
		     {
		       std::cout << "<- " << msg << std::endl;
		     });
      }
  }

  std::unique_ptr<WebSocket> ws;
  unsigned int ref;

  std::atomic_bool running;
  std::future<void> run_future;
  std::mutex mutex;
  std::condition_variable cond;
};
