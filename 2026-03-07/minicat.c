#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
int main(int argc, char *argv[])
{   char *filename=argv[1];
    int fd = open(filename, O_RDONLY);
    if (fd == -1){
        close(fd);
        return 1;}
    char buf[4096];
    int nread=0;
    nread=read(fd, buf, sizeof(buf));
    if(nread==-1){  close(fd);return 1;}

    int n_write=write(1,buf,nread);
    if(n_write==-1){  close(fd);return 1;}
    write(1,"\n",1);
    close(fd);
    return 0;
}
 