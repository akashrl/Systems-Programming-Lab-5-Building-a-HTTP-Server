struct Mod{
	void* lib;
	char* name;
	Mod *next;
	Mod *prev;
};

int num;
Mod *head;


void initMod(){
	num=0;
}

void addMod(void* lib, char* name){
	if(num==0){
		head=new Mod();
		head->lib=lib;
		head->name=strdup(name);
		head->next=NULL;
		head->prev=NULL;
	}else{
		Mod *tempNode=head;
		Mod *temp;
		while(tempNode){
			temp=tempNode;
			tempNode=tempNode->next;
		}
		tempNode=new Mod();
		tempNode->lib=lib;
		tempNode->name=strdup(name);
		tempNode->next=NULL;
		tempNode->prev=temp;
		temp->next=tempNode;
	}
}

void* inMod(char* name){
	if(head==NULL)
		return NULL;
	
	Mod *tempNode=head;
	while(tempNode&&tempNode->next&&strcmp(tempNode->name,name)){
		tempNode=tempNode->next;
	}
	if(tempNode->next==NULL){
		if(!strcmp(tempNode->name,name)){
			return tempNode->lib;
		}
	}else{
		return tempNode->lib;
	}
	
	return NULL;
}
