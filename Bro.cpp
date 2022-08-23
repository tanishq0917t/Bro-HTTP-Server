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
class Bro
{
    private:
        string staticResourcesFolder;
        map<string,void (*)(Request &,Response &)> urlMappings;
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
                urlMappings.insert({url,callBack});
            }
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
                if(clientSocketDescriptor<0)
                {
                    //not decided yet
                }
                forward_list<string> requestBufferDS;
                forward_list<string> :: iterator requestBufferDSIterator;
                requestBufferDSIterator=requestBufferDS.before_begin();
                int requestBufferDSSize=0;
                int requestDataCount=0;
                while(1)
                {
                    requestLen=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
                    if(requestLen==0) break;
                    requestBuffer[requestLen]='\0';
                    requestBufferDSIterator=requestBufferDS.insert_after(requestBufferDSIterator,string(requestBuffer));
                    requestBufferDSSize++;
                    requestDataCount+=requestLen;
                }
                if(requestBufferDSSize>0)
                {
                    char *requestData=new char[requestDataCount+1];
                    char *p;
                    p=requestData;
                    const char *q;
                    requestBufferDSIterator=requestBufferDS.begin();
                    while(requestBufferDSIterator!=requestBufferDS.end())
                    {
                        q=(*requestBufferDSIterator).c_str();
                        while(*q)
                        {
                            *p=*q;
                            p++;
                            q++;
                        }
                        ++requestBufferDSIterator;
                    }
                    *p='\0';
                    requestBufferDS.clear();
                    printf("------------------Request Data Begins---------------\n");
                    printf("%s\n",requestData);
                    printf("-------------------Request Data Ends----------------\n");
                    delete [] requestData;
                }
                else
                {
                    //something if no data was received
                }
                close(clientSocketDescriptor);
            }
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