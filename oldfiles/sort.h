struct Rec{
	char kind[10];
	char name[256];
	off_t size;
	char time[30];
	char link[256];
	Rec* next;
	Rec* pre;
};

void evictNode(Rec** head, Rec* node);
void insertNode(Rec** head, Rec* prevNode, Rec* insertingNode);
void swap (Rec** head, Rec* n1, Rec* n2);
void sort(char* sortby, Rec** list, int num);

void 
evictNode(Rec** head, Rec* node){
  if((*head)->next==NULL&&node==*head){ 
     *head=NULL;
  }
  else if(node==*head){
    *head=node->next;
    (*head)->pre=NULL;
    node->next=NULL;
  }
  else{
    if(node->next!=NULL){
      node->pre->next=node->next;
      node->next->pre=node->pre;
      node->next=NULL;
      node->pre=NULL;
     }
    else{
      node->pre->next=NULL;
      node->pre=NULL;
    }
  }
}

void 
insertNode(Rec** head, Rec* prevNode, Rec* insertingNode){
  if(prevNode==NULL){
    insertingNode->next=*head;
 	 if((*head)!=NULL)
  	 	(*head)->pre=insertingNode;
  	 *head=insertingNode;
  }
  else{
    if(prevNode->next!=NULL){
      insertingNode->next=prevNode->next;
      prevNode->next->pre=insertingNode;
      prevNode->next=insertingNode;
      insertingNode->pre=prevNode;
    }
    else{
      prevNode->next=insertingNode;
      insertingNode->pre=prevNode;
      insertingNode->next=NULL;
    }
  }
}

void 
swap (Rec** head, Rec* n1, Rec* n2) {
  if(n1!=n2){
     Rec* n01=n1->pre;
     Rec* n02=n2->pre;
     if(n1==n2->pre){
       evictNode(head,n1);
       evictNode(head,n2);
       insertNode(head,n01,n2);
       insertNode(head,n2,n1);
     }
     else if(n2==n1->pre){
       evictNode(head,n1);
       evictNode(head,n2);
       insertNode(head,n02,n1);
       insertNode(head,n1,n2);
     }
     else {
       evictNode(head,n1);
       evictNode(head,n2);
       insertNode(head,n01,n2);
       insertNode(head,n02,n1);
     }  
  }
  else return;
}

void
sort(char* sortby, Rec** list, int num){
	int swapn;
	Rec* p0=*list;
	do{
		swapn=0;
		Rec* p1=*list;
		Rec* p2=p1->next;
		if(p1==NULL||p2==NULL)
			return;

		if(!strcmp(sortby,"NA")||!strcmp(sortby,"DA")){
			while(p2){
				if(strcmp(p1->name,p2->name)>0){
					swapn++;
					swap(list,p1,p2);
					p0=p2;
					p2=p1->next;
				}else{
					p1=p2;
					p2=p2->next;
				}
				
			}
	
		}else if(!strcmp(sortby,"ND")||!strcmp(sortby,"DD")){

			while(p2){
				if(strcmp(p1->name,p2->name)<0){
					swapn++;
					swap(list,p1,p2);
					p0=p2;
					p2=p1->next;
					}else{
						p1=p2;
						p2=p2->next;
					}
			}
		
		}else if(!strcmp(sortby,"MA")){

			while(p2){
				if(strcmp(p1->time,p2->time)>0){
					swapn++;
					swap(list,p1,p2);
					p0=p2;
					p2=p1->next;
					}else{
						p1=p2;
						p2=p2->next;
					}
			}
		
		}else if(!strcmp(sortby,"MD")){

			while(p2){
				if(strcmp(p1->time,p2->time)<0){
					swapn++;
					swap(list,p1,p2);
					p0=p2;
					p2=p1->next;
					}else{
						p1=p2;
						p2=p2->next;
					}
			}
		
		}else if(!strcmp(sortby,"SA")){
			while(p2){
				if(p1->size > p2->size){
					swapn++;
					swap(list,p1,p2);
					p0=p2;
					p2=p1->next;
					}else{
						p1=p2;
						p2=p1->next;
					}
			}
		}else if(!strcmp(sortby,"SD")){

			while(p2){
				if(p1->size < p2->size){
					swapn++;
					swap(list,p1,p2);
					p0=p2;
					p2=p1->next;
					}else{
						p1=p2;
						p2=p2->next;
					}
			}
		
		}
		if(swapn==0)
			break;
	
	}while(p0!=*list);
}
