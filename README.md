# Lab-5-Building-a-HTTP-Server

The iterative HTTP server implements the following algorithm:
1. Open Passive Socket.
2. Do Forever
3. Accept new TCP connection
4. Read request from TCP connection and parse it.
5. Frame the appropriate response header depending on whether the URL requested is found on the server or not.
6. Write the response header to TCP connection.
7. Write requested document document (initially you will respond with index.html that is found in htdocs/index.html)  to TCP connection.
8. Close TCP connection

*Primary file that with the implementation is myhttpd.cpp.

Features added:
1. HTTP Authentication (prompts for a username and password
2. Three concurrency which is passed as an argument:
    1. -f Create a new process for each request
    2. -t : Create a new thread for each reques
    3. -p:  Pool of threads
3. Capacity to browse directories.  If the <Document Requested> in the request is a directory, the HTTP server will return an HTML document with hyperlinks to the contents of the directory. Also, it can recursively browse subdirectories contained in this directory.
4. CGI Bin. cgi-bin implementation should:
    1. Fork child process
    2. Set the environment variable REQUEST_METHOD=GET
    3. Set the environment variable QUERY_STRING=(arguments after ?)
    4. Redirect output of child process to slave socket.
    5. Print the following header:
    6. HTTP/1.1 200 Document follows crlf  Server: Server-Type crlf
5. Loadable Modules. Loadable modules are implemented to be able to extend the server. When the name of a cgi-bin script ends with .so, instead of calling exec for this  file the server will load that module into memory using dlopen(), if it has not been previously loaded. Then the server will transfer the control to this module by first looking up the function extern "C" httprun(int ssock, char * query_string) in that module using dlsym() and then calling httprun() passing the slave socket and the query string as parameters. httprun() will write the response to the ssock slave socket using the parameters in querystring.
6. Statistics and Log pages. Contains the following:
    1. The names of the student who wrote the project
    2. The time the server has been up
    3. The number of requests since the server started
    4. The minimum service time and the URL request that took this time.
    5. The maximum service time and the URL request that took this time.

