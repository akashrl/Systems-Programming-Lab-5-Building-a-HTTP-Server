#ifndef SORT
#define SORT
#include "sort.h"
#endif

void writeParentLink(char* filePath,char** htmlHeader);
void writeDir(Rec* pmyrec, char** htmlHeader, int num);
void writeFile(int fd, FILE* readfile, char* documentType, char* protocol);
void writeNotFound(int socket,char* documentType,char* protocol);

void
writeParentLink(char* filePath,char** htmlHeader){
	char* parentPath=(char*)malloc(2*strlen(filePath));
	if(*(filePath+strlen(filePath)-1)!='/'){
		char* p=strrchr(filePath,'/')+1;
		*(filePath+strlen(filePath)-1)='/';
		char pp=*p;
		*p='\0';
		strncpy(parentPath,filePath,strlen(filePath));
		*(parentPath+strlen(filePath))='\0';
		*p=pp; 
	}else{
		*(filePath+strlen(filePath)-1)='\0';
		char* p=strrchr(filePath,'/')+1;
		*(filePath+strlen(filePath)-1)='/';
		char pp=*p;
		*p='\0';
		strncpy(parentPath,filePath,strlen(filePath)); 
		*(parentPath+strlen(filePath))='\0';
		*p=pp;
	}

	char *r=expandPath(parentPath);
	if(r==NULL){
		gotGet=0;
		start_protocol=-1;
		free(docPath);
		free(cwd);
	 	free(filePath);
	 	docPath=NULL;
		cwd=NULL;
		filePath=NULL;
		return;
	}else if(strcmp(parentPath,cwd)==0){
		strcat(parentPath,"htdocs/index.html");
	}else if(strncmp(parentPath, cwd, strlen(parentPath))==0){
		strcat(parentPath,"/htdocs/index.html");
	}

	parentPath=parentPath+strlen(cwd);
	char* parentLink=(char*)malloc(500);
	strcpy(parentLink, "<img src=\"/icons/menu.gif\" alt=\"[DIR]\"> <a href=\"/../");
	strcpy(parentLink+53,parentPath);
	strcpy(parentLink+53+strlen(parentPath),"\">Parent Directory</a>\n");
	*(parentLink+77+strlen(host)+strlen(parentPath))='\0';
	if(strlen(parentLink)>(length-strlen(*htmlHeader))){
		*htmlHeader=(char*)realloc(*htmlHeader,length+strlen(parentLink));
		length=length+strlen(parentLink);
	}
	strcpy(*htmlHeader+strlen(*htmlHeader),parentLink);
}

void
writeDir(Rec* pmyrec, char** htmlHeader, int num){
	Rec* rp=pmyrec;
	for(int i=0; rp &&i<num;i++){
		char* fileInfo=(char*)malloc(10000);
		if(strcmp(rp->kind,"Directory")==0){
			strcpy(fileInfo,"<img src=\"/icons/menu.gif\" alt=\"[DIR]\"> <a href=\"");
			strcpy(fileInfo+49,rp->link);
			strcpy(fileInfo+49+strlen(rp->link),"/\">");
			strcpy(fileInfo+52+strlen(rp->link),rp->name);
			strcpy(fileInfo+52+strlen(rp->link)+strlen(rp->name),"</a>\t\t");
			strcpy(fileInfo+58+strlen(rp->link)+strlen(rp->name), rp->time);
			strcpy(fileInfo+58+strlen(rp->link)+strlen(rp->name)+strlen(rp->time), "     ");
			char s[10];
			sprintf(s, "%jd", rp->size);
			*(s+strlen(s))='\0';
			strcpy(fileInfo+63+strlen(rp->link)+strlen(rp->name)+strlen(rp->time),s);
			strcpy(fileInfo+63+strlen(rp->link)+strlen(rp->name)+strlen(rp->time)+strlen(s), "  ");
			strcpy(fileInfo+65+strlen(rp->link)+strlen(rp->name)+strlen(rp->time)+strlen(s), "Directory\n");
			*(fileInfo+75+strlen(rp->link)+strlen(rp->name)+strlen(rp->time)+strlen(s))='\0';
		}else{
			strcpy(fileInfo,"<img src=\"/icons/binary.gif\" alt=\"[DIR]\"> <a href=\"");
			strcpy(fileInfo+51,rp->name);
			strcpy(fileInfo+51+strlen(rp->link),"\">");
		 	strcpy(fileInfo+53+strlen(rp->link), rp->name);
			strcpy(fileInfo+53+strlen(rp->link)+strlen(rp->name),"</a>\t\t");
			strcpy(fileInfo+59+strlen(rp->link)+strlen(rp->name), rp->time);
			strcpy(fileInfo+59+strlen(rp->link)+strlen(rp->name)+strlen(rp->time), "     ");
			char s[10];
			sprintf(s, "%jd", rp->size);
			*(s+strlen(s))='\0';
			strcpy(fileInfo+64+strlen(rp->link)+strlen(rp->name)+strlen(rp->time),s);
			strcpy(fileInfo+64+strlen(rp->link)+strlen(rp->name)+strlen(rp->time)+strlen(s), "  ");
			strcpy(fileInfo+66+strlen(rp->link)+strlen(rp->name)+strlen(rp->time)+strlen(s), "File\n");
			*(fileInfo+71+strlen(rp->link)+strlen(rp->name)+strlen(rp->time)+strlen(s))='\0';
			printf("fileInfo:%s",fileInfo);
		}
		if(strlen(fileInfo)>(length-strlen(*htmlHeader))){
			*htmlHeader=(char*)realloc(*htmlHeader,length+strlen(fileInfo));
			length=length+strlen(fileInfo);			
		}
		strcpy(*htmlHeader+strlen(*htmlHeader),fileInfo);
		free(fileInfo);
		rp=rp->next;
	}
	
}

void
writeFile(int fd, FILE* readfile,char* documentType, char* protocol){

	char* dex;
	unsigned long lSize;
	fseek (readfile , 0L , SEEK_END);
	lSize = ftell (readfile);
  	rewind(readfile);
	char*  buffer = (char*) malloc (sizeof(char)*lSize);
	fread (buffer,1,lSize,readfile);	
	
	if(lSize>(1000-(strlen(protocol)+strlen(documentType)+120))){
		dex=(char*)malloc((1000+lSize)*sizeof(char));

	} 	else{
  		dex=(char*)malloc(1000*sizeof(char));
  	}
  	memcpy(dex,protocol,strlen(protocol));
  	memcpy(dex+strlen(protocol)," 200 Document follows\r\nServer: CS 252 lab5\r\nContent-type: ",58);
  	memcpy(dex+strlen(protocol)+58, documentType, strlen(documentType));
  	if(strcmp(documentType,"image/gif")==0){	
  		memcpy(dex+strlen(protocol)+strlen(documentType)+58, "Content-Transfer-Encoding: binary\r\nContent-Length: ", 51);
  		char* s=(char*)calloc(16,0);
  		sprintf(s,"%lu",lSize);
  		memcpy(dex+strlen(protocol)+strlen(documentType)+109,s,strlen(s));
  		memcpy(dex+strlen(protocol)+strlen(documentType)+strlen(s)+109,"\r\n\r\n",4);
  		memcpy(dex+strlen(protocol)+strlen(documentType)+strlen(s)+113,buffer,lSize);
		write(fd,dex,strlen(protocol)+strlen(documentType)+strlen(s)+113+lSize);

 	}else{
  		memcpy(dex+strlen(protocol)+strlen(documentType)+58,"\r\n\r\n",4);
  		memcpy(dex+strlen(protocol)+strlen(documentType)+62,buffer,lSize);
		*(dex+strlen(protocol)+strlen(documentType)+62+lSize)='\0';
		write(fd,dex,strlen(dex));
	}
  	free(dex);
	free (buffer);
	buffer=NULL;
  	dex=NULL;
}

void
writeNotFound(int fd, char* documentType, char* protocol){
	char * notFound=(char*)calloc(256*sizeof(char),0);
	memcpy(notFound,protocol,strlen(protocol));
	memcpy(notFound+strlen(notFound)," 404 File Not Found\r\nServer: CS 252 lab5\r\nContent-type: ",58);
	memcpy(notFound+strlen(notFound),documentType,strlen(documentType));
	memcpy(notFound+strlen(notFound),"\r\n\r\n",4);
	memcpy(notFound+strlen(notFound),"File Not Found",14);
	write(fd,notFound,strlen(notFound));
	notFound=NULL;
}