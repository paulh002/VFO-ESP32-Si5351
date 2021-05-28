#ifndef _NETWORK_H_
#define _NETWORK_H_

#define TELNET_PORT 23
#define MAX_CLIENTS 10
#define CATCLIENT 1
#define NET_BUFSIZE	45
#define TRXNET_CLIENT 1
#define TRXNET_SERVER 0

#define TCP_NODELAY    0x01    /* don't delay send to coalesce packets */
#define TCP_KEEPALIVE  0x02    /* send KEEPALIVE probes when idle for pcb->keep_idle milliseconds */
#define TCP_KEEPIDLE   0x03    /* set pcb->keep_idle  - Same as TCP_KEEPALIVE, but use seconds for get/setsockopt */
#define TCP_KEEPINTVL  0x04    /* set pcb->keep_intvl - Use seconds for get/setsockopt */
#define TCP_KEEPCNT    0x05    /* set pcb->keep_cnt   - Use number of probes sent for get/setsockopt */

class TrxNetwork
{
public:
	uint8_t begin(uint8_t type_network);
	void network_loop();
	void scanWIFITask(void* pvParameters);
	void network_stop();
private:
	TaskHandle_t	Task1;
	uint8_t			m_type_network;

	void init_ota(void);
	static void task1_client(void* arg);
	static void task1_server(void* arg);
};


class Telnet
{
public:
  size_t write(int iclient, const uint8_t *buffer, size_t size);
  size_t writeAll(const uint8_t *buffer, size_t size);
  size_t readAll(const uint8_t *buffer, size_t size);
  int available(int iclient);
  uint8_t connected(int iclient);
  uint8_t Anyconnection();
  size_t read(int iclient, const uint8_t *buffer, size_t size);
  void print(int iclient =0 , String string = "");

  void handle();
  void begin();
  void stop();
  Telnet(WiFiServer *Server, int maxconnections=1, String intro = "Connected to ESP-VPO PA0PHH\r\n");
  ~Telnet();

private:
  WiFiServer *pServer;
  WiFiClient *clients[MAX_CLIENTS] = { NULL };
  int max_connections;
  void CheckForConnections();
  void CheckForDroppedConnections();
  String IntroMessage;
};

class TelnetQueue
{
public:
	size_t write(const uint8_t* buffer, size_t size);
	size_t read(const uint8_t* buffer, size_t size);
	void begin();
	void stop();
	uint8_t Anyconnection();
};

extern Telnet			DebugServer;
extern TelnetQueue		CatServer;

class TelnetClient
{
public:
  
  void		handle();
  void		begin(WiFiClient *client);
  size_t	read(const uint8_t *buffer, size_t size);
  size_t	write(const uint8_t *buffer, size_t size);
  uint8_t	connected();
  uint8_t	available();
  void		CheckForDroppedConnections();
private:
  WiFiClient	*pclient =  NULL;
};

class TelnetClientQueue
{
public:
	size_t write(const uint8_t* buffer, size_t size);
	size_t read(const uint8_t* buffer, size_t size);
	uint8_t		connected();
	void	begin();
	void	stop();
};

extern TelnetClientQueue	CatTelClient;
extern TrxNetwork			vfo_network;
extern QueueHandle_t		net_queue;
#endif
