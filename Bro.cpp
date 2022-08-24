#include<bits/stdc++.h>
#include<unistd.h>

#ifdef _WIN32
#include<windows.h>
#endif

#ifdef linux
#include<arpa/inet.h>
#include<sys/socket.h>
#endif


using namespace std;
//Amit [The Bro Programmer]
class Validator
{
    private:
        Validator()
        {

        }
    public:
        static bool isValidPath(string &path)
        {
            return true;
        }
        static bool isValidMIMEType(string &mimeType)
        {
            return true;
        }
        static bool isValidURLFormat(string &url)
        {
            return true;
        }
};
class Error
{
    private:
        string error="";
    public:
        Error(string error)
        {
            this->error=error;
        }
        bool hasError()
        {
            return this->error.length()>0;
        }
        string getError()
        {
            return this->error;
        }
};
class Request
{
    
};
class Response
{
    private:
        string contentType;
        forward_list<string> content;
        forward_list<string> ::iterator contentIterator;
        unsigned long contentLength;
    public:
        Response()
        {
            this->contentLength=0;
            this->contentIterator=this->content.before_begin();
        }
        void setContentType(string contentType)
        {
            if(Validator::isValidMIMEType(contentType))
            {
                this->contentType=contentType;
            }
        }
        Response & operator <<(string content)
        {
            this->contentLength+=content.length();
            this->contentIterator=this->content.insert_after(this->contentIterator,content);
            return *this;
        }
};
enum __request_method__{__GET__,__POST__,__PUT__,__DELETE__,__CONNECT__,__TRACE__,__HEAD__,__OPTIONS__};
typedef struct __url__mappings
{
    __request_method__ requestMethod;
    void (*mappedFunction)(Request &,Response &);
}URLMapping;
class Bro
{
    private:
        string staticResourcesFolder;
        map<string,URLMapping> urlMappings;
    public:
        Bro()
        {
            //not yet decided
        }
        ~Bro()
        {
            //not yet decided
        }
        void setStaticResourcesFolder(string staticResourceFolder)
        {
            if(Validator::isValidPath(staticResourceFolder))
            {
                this->staticResourcesFolder=staticResourcesFolder;
            }
            else
            {
                //not yet decided
            }
        }
        void get(string url,void (*callBack)(Request &,Response &))
        {
            if(Validator::isValidURLFormat(url))
            {
                urlMappings.insert({url,{__GET__,callBack}});            }
        }
        void listen(int portNumber,void (*callBack)(Error &))
        {
            #ifdef _WIN32
                WSADATA wsaData;
                WORD ver;
                ver=MAKEWORD(1,1);
                WSAStartup(ver,&wsaData);
            #endif
            int serverSocketDescriptor;
            int clientSocketDescriptor;
            int successCode;
            char requestBuffer[4097];
            int requestLen;
            serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
            if(serverSocketDescriptor<0)
            {
                #ifdef _WIN32
                    WSACleanup();
                #endif
                Error error("Unable to create socket");
                callBack(error);
                return;
            }
            struct sockaddr_in serverSocketInformation;
            serverSocketInformation.sin_family=AF_INET;
            serverSocketInformation.sin_port=htons(portNumber);
            serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
            successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
            if(successCode<0)
            {
                close(serverSocketDescriptor);
                #ifdef _WIN32
                    WSACleanup();
                #endif
                Error error("Unable to bind socket to port");
                callBack(error);
                return;
            }
            successCode=::listen(serverSocketDescriptor,10);
            if(successCode<0)
            {
                close(serverSocketDescriptor);
                #ifdef _WIN32
                    WSACleanup();
                #endif
                Error error("Unable to accept client connections");
                callBack(error);
                return;
            }
            Error error("");
            callBack(error);
            struct sockaddr_in clientSocketInformation;
            #ifdef linux
                socklen_t len=sizeof(clientSocketInformation);
            #endif
            #ifdef _WIN32
                int len=sizeof(clientSocketInformation);
            #endif
            while(1)
            {
                clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
                requestLen=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
                if(requestLen==0 || requestLen==-1)
                {
                    close(clientSocketDescriptor);
                    continue;
                }
                requestBuffer[requestLen]='\0';
                close(clientSocketDescriptor);
            } //Infinite loop ends here for accepting requests
            #ifdef _WIN32
                WSACleanup();
            #endif
        }
};

// Bobby [ The Web Application Developer - Using BRO Web Server ]
int main()
{
    Bro bro;
    bro.setStaticResourcesFolder("whatever");
    bro.get("/",[](Request &request,Response &response) {
        const char *html=R""""(
            <!DOCTYPE HTML>
            <html>
            <head>
            <meta charset='UTF-8'>
            <title>Whatever</title>
            </head>
            <body>
            <h1>Welcome</h1>
            <h3>Administrator</h3>
            <a href='getCustomers'>Customers List</a>
            </body>
            </html>
        )"""";
        response.setContentType("text/html");
        response<<html;
    });
    bro.get("/getCustomers",[](Request &request,Response &response) {
        const char *html=R""""(
            <!DOCTYPE HTML>
            <html>
            <head>
            <meta charset='UTF-8'>
            <title>Whatever</title>
            </head>
            <body>
            <h1>List of Customers</h1>
            <ul>
            <li>Ramesh</li>
            <li>Suresh</li>
            <li>Jitesh</li>
            </ul>
            <a href='/'>Home</a>
            </body>
            </html>
        )"""";
        response.setContentType("text/html");
        response<<html;
    });
    bro.listen(6060,[](Error &error){
        if(error.hasError())
        {
            cout<<error.getError()<<endl;
            return;
        }
        cout<<"BRO Http Web Server is ready to accept request on port 6060"<<endl;
    });
    return 0;
}