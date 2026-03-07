#include<unistd.h>
#include<fcntl.h>

int main(int argc,char*argv[]){
    char* input=argv[1];
    char* target=argv[2];
    int fdin=open(input,O_RDONLY);
     if(fdin==-1){
        close(fdin);
       
        return 1; }
    int fdout=open(target,O_RDWR|O_CREAT|O_TRUNC,0644);
        if(fdout==-1){
        close(fdin);
        close(fdout);
        return 1; }
    char buf[4096];
    int n_read=0;
    n_read=read(fdin,buf,sizeof(buf));
    if(n_read==-1){
        close(fdin);
        close(fdout);
        return 1; }
    int n_write=write(fdout,buf,n_read);
    if(n_write==-1)return 1;
    close(fdin);
    close(fdout);
    return 0;;

}