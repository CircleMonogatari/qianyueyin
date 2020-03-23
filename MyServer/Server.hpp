

class Server
{
private:
    Server(/* args */);

    ~Server();

    Server(const Server &);
//    Server& operator=(const Server&);

    /* data */

public:
    static Server *instance;

    static Server *getServerInstance();

    /**
     * 启动socket
     * @param port
     * @return
     */
    int run(int port);

};


