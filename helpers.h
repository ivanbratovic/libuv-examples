#include <uv.h>

int Uv_tcp_init(uv_loop_t* loop, uv_tcp_t* handle){
	int r = uv_tcp_init(loop, handle);
	assert(r==0);
	return r;
}

int Uv_ip4_addr(char *ip, int port, struct sockaddr_in* addr){
	int r = uv_ip4_addr(ip, port, addr);
	assert(r==0);
	return r;
}

uv_tcp_t* Malloc_tcp_server(){
	uv_tcp_t* tcp_server = malloc(sizeof(uv_tcp_t));
	assert(tcp_server != NULL);
	return tcp_server;
}

int Uv_tcp_bind(uv_tcp_t* server, const struct sockaddr* addr, int flags){
	int r = uv_tcp_bind(server, addr, flags);
	assert(r == 0);
	return r;
}

int Uv_listen(uv_stream_t* server, int conn, void* on_conn){
	int r = uv_listen((uv_stream_t*)server, SOMAXCONN, on_conn);
  	assert(r == 0);
  	return r;
}

int Uv_run(void* loop, int flags){
	int r = uv_run(loop, flags);
	assert(r==0);
	return r;
}

int Uv_run_default(){
	return Uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

int Uv_write(uv_write_t* req, uv_stream_t* handle, uv_buf_t* buf, int flags, void* after_write){
	int r = uv_write(req, handle, buf, 1, after_write);
    assert(r == 0);
    return r;
}

int Uv_shutdown(uv_shutdown_t* req, uv_stream_t* handle, void* after_shutdown){
	int r = uv_shutdown(req, handle, after_shutdown);
    assert(r == 0);
    return r;
}
