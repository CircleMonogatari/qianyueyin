class Server
{
private:
    Server(/* args */);
    ~Server();
    Server(const Server&);
    Server& operator=(const Server&);
    
    /* data */
        static Server* instance;
public:
    static Server* getServer ();
};

Server::Server(/* args */)
{

}

Server::~Server()
{
    
}
