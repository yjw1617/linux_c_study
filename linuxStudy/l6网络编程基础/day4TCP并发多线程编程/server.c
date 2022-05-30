void* sig_child_handle(int signal){
    if(SIGCHLD == signal) {
		waitpid(-1, NULL,  WNOHANG);//不阻塞的回收子进程
	}
}
//子进程推出信号
signal(SIGCHLD, sig_child_handle);
//用进程来多线程中的while
while(1){
    if((newfd = accept(fd, (struct sockaddr*)*cin, &addrlen) < 0)){
        perror("accept");
        break;
    }
    //创建一个子进程用于处理已经建立连接的交互数据
    pid = fork();
    if(pid < 0){
        perror("fork");
        break;
    }
    if(pid > 0){//父进程

        close(newfd);
    }else{//子进程
        close(fd);
        char ipv4_addr[16];
        if(!inet_ntop(AF_INET, &cin.sin_addr.s_addr, ipv4_addr, sizeof(cin))){
            perror("inet_ntop");
            exit(1);
        }
        printf("client %s is comming\n",ipv4_addr);
        }  

        cli_data_handle(&newfd);//将文件句柄传到函数进行读写操作
        return 0;
}