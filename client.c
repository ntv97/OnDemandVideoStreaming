#define PY_SSIZE_T_CLEAN
#include <stdio.h>
#include <Python.h>
#include </usr/include/python3.10/pyconfig.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

char path[SIZE];

void WriteFile(int sockfd) {
	size_t n;
	FILE *fp;
	char buffer[SIZE];
	char filename[SIZE];
	strcpy(path, "ClientVideos/");

	int i = 0;

	while (1) {
		if (i == 0) {
			while(1) {
				n = recv(sockfd, buffer, SIZE, 0);
				printf("%s\n", buffer);
				if (!strcmp(buffer, "\nWhich movie would you like to watch?")) break;
			}
			i++;
		} else if (i ==1 ) {
			scanf("%[^\n]%*c", filename);
			if (send(sockfd, filename, SIZE, 0) == -1) {
				perror("[-]Error in sending file.");
				exit(1);
			}
			i++;
			strcpy(path+strlen(path), filename);
			fp = fopen(path, "wb");
		} else {
			n = recv(sockfd, buffer, SIZE, 0);
			if (n <= 0) break;
			fwrite(buffer, n, 1, fp);
			bzero(buffer, SIZE);
		}
	}
	fclose(fp);
	return;
}

void PlayVideo() {
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;
    char *module = "PlayVideo", *fn = "PlayVideo";

    Py_Initialize();
    //PySys_SetPath("/home/nhivo/C++");
    pName = PyUnicode_DecodeFSDefault(module);
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, fn);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(1);
	    //char* path = "/home/nhi";
	    pValue = PyUnicode_FromString(path);
	    if (pValue == NULL) return;
	    PyTuple_SetItem(pArgs, 0, pValue);

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                //printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", fn);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", module);
        return;
    }
    if (Py_FinalizeEx() < 0) {
        return;
    }
}

int main(){
	char *ip = "127.0.0.1";
	int port = 8080;

	struct sockaddr_in server_addr;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("> Error in socket");
		exit(1);
	}
	printf("> Client socket created successfully.\n\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);

	int conn = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (conn == -1) {
		perror("> Error in socket");
		exit(1);
	}

	WriteFile(sockfd);
	PlayVideo();

	return 0;
}
