#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <uv.h>
#include "helpers.h"

#define PORT 8000

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

static uint64_t data_cntr = 0;
static uint64_t timer_c = 0;

static void on_close(uv_handle_t* handle) {
    free(handle);
}


static void after_write(uv_write_t* req, int status) {
    write_req_t* wr = (write_req_t*)req;

    if (wr->buf.base != NULL)
        free(wr->buf.base);
    free(wr);

    if (status == 0)
        return;

    fprintf(stderr, "uv_write error: %s\n", uv_strerror(status));

    if (status == UV_ECANCELED)
        return;

    assert(status == UV_EPIPE);
    uv_close((uv_handle_t*)req->handle, on_close);
}


static void after_shutdown(uv_shutdown_t* req, int status) {
    /*assert(status == 0);*/
    if (status < 0)
        fprintf(stderr, "Error: %s\n", uv_strerror(status));
    fprintf(stderr, "Bytes recieved: %lu, time elapsed: %lus\n", data_cntr, timer_c);
    data_cntr = 0;
    timer_c = 0;
    uv_close((uv_handle_t*)req->handle, on_close);
    free(req);
}


static void after_read(uv_stream_t* handle,
                       ssize_t nread,
                       const uv_buf_t* buf) {
    write_req_t* wr;
    uv_shutdown_t* req;

    if (nread <= 0 && buf->base != NULL)
        free(buf->base);

    if (nread == 0)
        return;

    if (nread < 0) {
        /*assert(nread == UV_EOF);*/
        fprintf(stderr, "Error reading: %s\n", uv_strerror(nread));

        req = (uv_shutdown_t*) malloc(sizeof(*req));
        assert(req != NULL);

        Uv_shutdown(req, handle, after_shutdown);

        return;
    }

    data_cntr += nread;

    wr = (write_req_t*) malloc(sizeof(*wr));
    assert(wr != NULL);

    wr->buf = uv_buf_init(buf->base, nread);

    Uv_write(&wr->req, handle, &wr->buf, 1, after_write);
}


static void alloc_cb(uv_handle_t* handle,
                                        size_t suggested_size,
                                        uv_buf_t* buf) {
    buf->base = malloc(suggested_size);
    assert(buf->base != NULL);
    buf->len = suggested_size;
}


static void on_connection(uv_stream_t* server, int status) {
    uv_tcp_t* stream;
    int r;

    assert(status == 0);

    stream = malloc(sizeof(uv_tcp_t));
    assert(stream != NULL);

    r = uv_tcp_init(uv_default_loop(), stream);
    assert(r == 0);

    stream->data = server;

    r = uv_accept(server, (uv_stream_t*)stream);
    assert(r == 0);

    r = uv_read_start((uv_stream_t*)stream, alloc_cb, after_read);
    assert(r == 0);
}


static int tcp_echo_server() {
    uv_tcp_t* tcp_server;
    struct sockaddr_in addr;

    Uv_ip4_addr("0.0.0.0", PORT, &addr);

    tcp_server = Malloc_tcp_server();

    Uv_tcp_init(uv_default_loop(), tcp_server);

    Uv_tcp_bind(tcp_server, (const struct sockaddr*)&addr, 0);

    Uv_listen((uv_stream_t*)tcp_server, SOMAXCONN, on_connection);

    return 0;
}



static void simple_timer_cb (uv_timer_t* timer, int status) {
    timer_c++;
}

static void simple_timer(){
    uv_timer_t* timer = (uv_timer_t*) malloc(sizeof(uv_timer_t));
    uv_timer_init(uv_default_loop(), timer);
    uv_timer_start(timer, (uv_timer_cb) &simple_timer_cb, 0, 1000);
}

int main() {
    tcp_echo_server();
    simple_timer();
    Uv_run_default();

    return 0;
}
