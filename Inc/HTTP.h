//Protótipos em HTTP.c
int HttpResponse(const char *httpmsg, char *response);
int TesteConexao();
int HttpPost(char *url, char *params, char *response);
int _EnviaSocket(char *msg, size_t msglen, char *buffer, size_t bufferlen);
int EnviaSocket(char *msg, size_t msglen, char *buffer, size_t bufferlen);
