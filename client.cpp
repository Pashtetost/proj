#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>


#define MAX 65536 ///максимальная длина сообщения байт


using namespace std;
void *write(void *args);
void *read(void *args);
bool check=true; //глобальная для связи потоков
int main(int argc, char *argv[])
{
    pthread_t threadA[2]; // используем 2 потока для отправки и приема
    int sock,p_par;
    struct sockaddr_in addr;
    if (! strcmp(argv[1],"tcp"))
    {
       cout<<"Тип сокета тсп"<<endl;
        sock = socket(AF_INET, SOCK_STREAM, 0);
    } else if (! strcmp(argv[1],"udp"))
    {
        cout<<"Тип сокета юдп"<<endl;
        sock = socket(AF_INET, SOCK_DGRAM, 0);
    } else
    {
       cout<<"Неизвестный сокет,ошибка"<<endl;
        exit(-1);
    }
    if(sock < 0)
       {
           perror("socket");
           exit(1);
       }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3164); // рандомно выбранный порт
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("connect");
            exit(2);
        }
    pthread_create(&threadA[0], NULL, write,(void*) &sock);
    pthread_create(&threadA[1], NULL, read, (void*) &sock);
    pthread_join(threadA[0], (void**)&p_par);
    pthread_join(threadA[1], (void**)&p_par);

    return 0;
}
void *write (void *args)
{
    int *sock = (int*) args;
    while(1)
       {
           char s[MAX];
           cout << "Отправка сообщения"<<endl;
           bzero(s, MAX + 1);
           cin.getline(s, MAX);
           send(*sock, s, strlen(s), 0);
           if(strcmp(s,"server_exit")==0)
               break;
       }
    check=false;
    cout << "Закрываем поток на отправку" << endl;
    close(*sock);
}
void *read (void *args)
{
    int *sock = (int*) args;
    char test[MAX];
    bzero(test, MAX + 1);
    while(check)
    {
        bzero(test, MAX + 1);

        int rc = read(*sock, test, MAX);
        if ( rc > 0)
        {
        string tester (test);
        cout << "Ответ от сервера:"<<endl;
        cout << ": "<< tester << endl;
        }

    }
    cout << "Закрываем поток на прием" << endl;
    close(*sock);
}
