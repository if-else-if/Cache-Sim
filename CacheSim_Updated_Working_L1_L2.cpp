#include<iostream>
#include<vector>
#include<list>
#include<string>
#include<cmath>
#include<stdio.h>
using namespace std;

class memory{
	int tag;
	int dirty_bit;
	int valid;
	int recency;
	int block_offset;
	
	public:
	memory(){
		tag=0;
		dirty_bit=0;
		valid=0;
		recency=0;
		block_offset=0;
	}
	void printmemory(){
		cout<<tag<<"\t"<<dirty_bit<<"\t"<<valid<<"\t"<<recency<<"\t"<<block_offset<<endl;
	}
	void set_tag(int data){ tag =data ;}
	//void set_index(int data){ index= data;}
	void set_valid(int data){ valid= data;}
	void set_recency(int data){ recency=data;}
	void set_block_offset(int data){ block_offset=data;}
	void set_dirty(int data){dirty_bit = data;}
	
	int get_tag(){ return tag;}
	//int get_index(){ return index;}
	int get_dirty_bit(){ return dirty_bit;}
	int get_valid(){ return valid;}
	int get_recency(){ return recency;}
	int get_block_offset(){ return block_offset;}
	
};
class Cache{
	int size;
	int block_size;
	int assoc;
	int sets;
	int num_block_offset_bits;
	int num_index_bits;

	
	//Cache *next;
	
	public:
	vector<vector<memory> > array;
	
	int get_size(){ return size;}
	int get_tag(int row, int col){ return array[row][col].get_tag();}
	int get_block_size(){ return block_size;}
	int get_assoc(){ return assoc;}
	int get_sets(){ return sets;}
	int get_num_block_offset_bits(){ return num_block_offset_bits;}
	int get_num_index_bits(){ return num_index_bits;}
	
	void set_size(int data){ size=data;}
	void set_block_size(int data){ block_size = data;}
	void set_assoc(int data) { assoc = data;}
	void set_num_block_offset_bits(int data){ num_block_offset_bits=floor(log2(data));}
	void set_num_index_bits(int data){ num_index_bits= floor(log2(data));}
	
	
	Cache( int s, int a, int b){
		//cout<<endl<<"===In Parametrized constructor===="<<endl;
		//cout<<int(s)<<endl;
		this->size=s;
		this->assoc= a;
		this->block_size=b;
		this->sets= size/(assoc* block_size);
		array.resize(sets,vector<memory>(assoc));
		for(int i=0;i<sets;i++){
			for(int j=0;j<assoc;j++){
				array[i][j].set_tag(0);
				array[i][j].set_valid(0);
				array[i][j].set_dirty(0);
			}
		}
		this->num_block_offset_bits= floor(log2(block_size));
		this->num_index_bits= floor(log2(sets));
		//Cache *next= NULL;
		/*cout<<"size is "<<size<<endl;
		cout<<"assoc is "<<assoc<<endl;
		cout<<"BS is "<<block_size<<endl<<endl<<endl;*/
		//cout<<"=====Exiting Constructor========"<<endl;
	}
	int get_block_address(int tag, int index){
		int result= (tag<<num_index_bits)| index;
		return result;
	}
	int get_address(int tag, int index, int block_offset){
		int result1= (tag<<num_index_bits)|index;
		int result=  (result1 << num_block_offset_bits) | block_offset;
		return result;
	}
	void print_cache(){
		int a= this->get_sets();
		int b= this->get_assoc();
		int c= this->get_block_size();
		/*cout<<"The cache details are"<<endl;
		cout<<"Sets is"<<"\t"<<a<<endl;
		cout<<"Assoc is"<<"\t"<<b<<endl;
		cout<<"Block size is"<<"\t"<<c<<endl;*/
		cout<<"Set"<<"\t"<<"Tag"<<"\t"<<"Dirty"<<"\t"<<"LRU"<<endl;
		for(int i=0;i<a;i++){
			for(int j=0;j< b;j++)
				if(array[i][j].get_valid())
					cout<<hex<<i<<"\t"<<array[i][j].get_tag()<<"\t"<<array[i][j].get_dirty_bit()<<"\t"<<array[i][j].get_recency()<<"\t";
		}
		
	}
	void set_recency(){
		for(int i=0;i<this->sets;i++){
			for(int j=0;j<this->assoc;j++)
				this->array[i][j].set_recency(j);
		}
		/*cout<<"======IN SET RECENCY==========="<<endl;
		cout<<" Sets is "<<dec<<this->get_sets()<<endl;
		printf("The number of sets is %d\n",get_sets());
		cout<<"Assoc is "<<this->get_assoc()<<endl;
		cout<<"block size is "<<this->get_block_size()<<endl;*/
	}
	int calculate_tag(int address){
		// Tag is the address stripped off block offset and index
		int num_tag_bits= 32- num_index_bits-num_block_offset_bits;
		int tag= (address >> num_index_bits + num_block_offset_bits) & ((1<<num_tag_bits)-1);
		//cout<<" here 2 "<<tag<<endl;
		/*cout<<"======Values in calculate tag====="<<endl;
		cout<<"Sets is "<<sets<<endl;
		cout<<"Assoc is "<<assoc<<endl;
		cout<<"block size is "<<block_size<<endl;*/
		return tag;
	}
	int calculate_index(int address){
		//Function to calculate the index based on the input data
		//cout<<"# Block offset bit is "<<num_block_offset_bits<<endl;
		//cout<<"# index bits is "<<num_index_bits<<endl;
		int index=(address>>num_block_offset_bits)&((1<<num_index_bits)-1);
		//cout<<"index is "<<index<<endl;
		return index;
	}
	int calculate_block_offset(int address){
		int mask= (1<<num_block_offset_bits)-1;
		int block_offset = address & mask;
		return block_offset;
	}
	int calculate_block_address(int address){
		int block_address= (address >>num_block_offset_bits);
		return block_address;
	}
	int IsPresent(int tag,int index){
		/*printf("\n%d\n",sets);
		cout<<"===========In IsPresent============"<<endl;
		cout<<"tag is"<<hex<<tag<<endl;
		cout<<"Index is "<<hex<<index<<endl<<endl;
		cout<<"======Values in IsPresent====="<<endl;
		
		std::cout<<"Sets is "<<sets<<endl;
		cout<<"Assoc is "<<assoc<<endl;
		cout<<"block size is "<<block_size<<endl;*/
		int location=0;
		int flag=0;
		//print_cache();
		for( int i=0;i <assoc; i++){
			if( array[index][i].get_tag()== tag)
				flag=1;
		}
		//print_cache();
		
		cout<<"======Exiting IsPresent========"<<endl;
		if(flag)
			return 1;
		else
			return 0;
	}
	int FindMRU(int index){
		int location=0;
		int flag=0;
		for(int i=0;i<assoc;i++){
			if(array[index][i].get_recency() == 0){
				flag=1;
				location =i;
			}
		}
		if(flag)
			return location;
		else
			return 0;
	}
		
	int FindLRU(int index){
		int max_counter= assoc-1;
		int location=0;
		for(int i=0;i<assoc;i++){
			if(array[index][i].get_recency()== max_counter)
			location=i;
		}
		return location;
	}
			
	void UpdateLRUHit(int index, int col){
		//cout<<"FLAG IS "<<flag<<endl;
		// update recency bits of other blocks as per LRU
		for(int i=0;i<assoc; i++){
			if (array[index][i].get_recency() < array[index][col].get_recency())
				array[index][i].set_recency(array[index][i].get_recency()+1);
		}
		array[index][col].set_recency(0);
	}
	
	void UpdateLRUMiss(int index){
		cout<<"=====Updating LRU MISS========="<<endl;
		int pos= FindLRU(index);
		cout<<"LRU pos is "<<pos<<endl;
		for(int i=0;i<get_assoc();i++)
			if(i!=pos)
				array[index][i].set_recency(array[index][i].get_recency()+1);
		array[index][pos].set_recency(0);
		//print_cache();
	}
	
	int IsVictimBlock(int tag, int index){
		int location = FindLRU(index);
		if(array[index][location].get_dirty_bit())
			return 1;
		else
			return 0;
	}
	
	void AllocateBlock(int tag, int row, int col){
		cout<<"Allocating block "<<hex<<tag<<" in "<<row<<", "<<col<<endl;
		array[row][col].set_valid(1);
		array[row][col].set_tag(tag);
		array[row][col].set_dirty(0);
		//array[row][col].set_recency(0);
	}
	
	int FindEmptyPos(int index){
		//Find empty columns in the set to write to.
		int pos=-1;
		for(int i=0;i<assoc;i++){
			if(array[index][i].get_valid() == 0)
				return i;
		}
		if(pos ==-1)
			return -1;
	}
	
	void SetDirtyBit(int row, int col, int value){
		array[row][col].set_dirty(value);
	}
	int findcol(int tag, int index){
		int col=0;
		for(int i=0;i<get_assoc();i++)
			if(array[index][i].get_tag()== tag)
				col= i;
		return col;
	}
};

class victim{
};

class CPU{
	vector<Cache*>L;
	Cache *l1;
	Cache *l2;
	public:
	
	/*CPU(){
		Cache L1;
		Cache L2;
		L.resize(2);
		L.push_back(L1);
		L.push_back(L2);
		l1.Set_recency();
		l2.Set_recency();
	}*/
	CPU(int sizel1,int assocl1,int blocksizel1,int sizel2,int assocl2,int blocksizel2){
		//cout<<" =======Entering CPU Constructor======="<<endl;
		l1= new Cache(sizel1,assocl1,blocksizel1);
		l2= new Cache(sizel2,assocl2,blocksizel2);
		//L.resize(2);
		L.push_back(l1);
		L.push_back(l2);
		l1->set_recency();
		//cout<<" =======L1 Cache is========"<<endl;
		//l1->print_cache();
		//cout<<"Sets is "<<l1.get_sets()<<endl;
		//cout<<"BS is "<<l1.get_block_size()<<endl;
		l2->set_recency();
		//cout<<"=======Exiting CPU constructor==========="<<endl;
		//cout<<endl<<"============L2 array is======="<<endl;
		//l2->print_cache();
	}
	/*~CPU(){
		delete l1;
		delete l2;
		for(int i=0;i<L.size();i++)
			delete L[i];
	}*/
	/*void setL1parameters(int size, int assoc, int block_size){
		int sets= size/(assoc*block_size);
		l1.set_assoc(assoc);
		l1.set_size(size);
		l1.set_block_size(block_size);
		l1.set_num_block_offset_bits(block_size);
		int l1index = size/(assoc*block_size);
		l1.set_num_index_bits(l1index);
	}
	void setL2parameters(int size, int assoc, int block_size){
		int sets= size/(assoc*block_size);
		l2.set_assoc(assoc);
		l2.set_size(size);
		l2.set_block_size(block_size);
		l2.set_num_block_offset_bits(block_size);
		int l1index = size/(assoc*block_size);
		l2.set_num_index_bits(l1index);
	}*/
		
		
		
	void display_cache(){
		cout<<endl<<"========L1 Cache========="<<endl; 
		l1->print_cache();
		cout<<endl<<"==========L2 Cache========="<<endl;
		l2->print_cache();
	}
	void l2read(int tag, int index){
		cout<<"==========L2 read request====="<<endl;
		cout<<"========In L2 Read========="<<endl;
		int x= l2->IsPresent(tag, index);
		if(x){
			cout<<"===L2 read hit====="<<endl;
			int pos=0;
			pos=l2->findcol(tag, index);
			l2->UpdateLRUHit(index,pos);
		}
		else{
			cout<<"=======L2 read misss======="<<endl;
			int pos1=l2->FindLRU(index);
			l2->AllocateBlock(tag, index, pos1);
			l2->UpdateLRUMiss(index);	
		}
	}
	void l2write(int tag, int index){
		cout<<"======In l2 Write====="<<endl;
		cout<<"L2 tag is "<<tag<<endl;
		cout<<"L2 index is "<<index<<endl;
		int x= l2->IsPresent(tag, index);
		if(x){
			int pos= l2->findcol(tag, index);
			l2->SetDirtyBit(index,pos,1);
		}
		else{
			cout<<"======L2 write miss====="<<endl;
			l2read(tag, index);
			int pos1=l2->findcol(tag,index);
			l2->SetDirtyBit(index,pos1,1);
		}
	}
	
	void write(int address){
		int l1index=0;
		int l1tag=0;
		int l1block_offset=0;		
		int l2tag=0;
		int l2index=0;
		int l2block_offset=0;
		
		l1tag= l1->calculate_tag(address);
		cout<<"L1 tag is "<<l1tag<<endl;
		l1index=l1->calculate_index(address);
		cout<<"l1 index is "<<l1index<<endl;
		l1block_offset= l1->calculate_block_offset(address);
		
		l2tag=l2->calculate_tag(address);
		cout<<"L2 tag is "<<l2tag<<endl;
		l2index=l2->calculate_index(address);
		cout<<"L2 index is "<<l2index<<endl;
		l2block_offset=l2->calculate_block_offset(address);
		int x1 = l1->IsPresent(l1tag, l1index);

		if(x1){
			//L1 hit
			int l1col= findcol(l1tag, l1index);
			l1->array[l1index][l1col].set_dirty(1);
			l1->UpdateLRUHit(l1tag,l1index);
			return 0;
		}
		else{
			int col1= l1->FindLRU(l1index);
			//try to find the victim block
			if(l1->array[l1index][col1].get_valid()){
				if(l1->array[l1index][col1].get_dirty_bit()){
					//write the block into l2 
					int tagvb= l1->array[index][col].get_tag();
					int addressvb=l1->calculate_block_address(tag_,l1index,0);
					//calculate the L2 tag and address to write the victim block to
					int l2_tag_vb=l2->calculate_tag(addressvb);
					int l2_index_vb= l2->calculate_index(addressvb);

					int x2= l2->IsPresent(l2tag_vb., l2_index_vb);
					if(x2){
						int l2_col_vb=l2->findcol(l2_tag_vb, l2_index_vb);
						l2->array[l2_index_vb][l2_col_vb].set_dirty(1);
						l2->UpdateLRUHit(l2_tag_vb,l2_index_vb);
					}
					else{
						//check if there is a L2 victim
						int l2_col=l2->FindLRU(l2_index_vb);
						if(l2->array[l2index_vb)[l2_col].get_valid()){
							if(l2->array[l2index_vb][l2col].get_dirty_bit()){
								//writebacktomem++
							}
							
						}
					}
					
				}
			}


		}
	}
	void read(int address){
		cout<<endl<<" ===========IN READ=========="<<endl;
		int l1index=0;
		int l1tag=0;
		int l1block_offset=0;		
		int l2tag=0;
		int l2index=0;
		int l2block_offset=0;
		
		l1tag= l1->calculate_tag(address);
		cout<<"L1 tag is "<<l1tag<<endl;
		l1index=l1->calculate_index(address);
		cout<<"l1 index is "<<l1index<<endl;
		l1block_offset= l1->calculate_block_offset(address);
		
		l2tag=l2->calculate_tag(address);
		cout<<"L2 tag is "<<l2tag<<endl;
		l2index=l2->calculate_index(address);
		cout<<"L2 index is "<<l2index<<endl;
		l2block_offset=l2->calculate_block_offset(address);
		
		int x = l1->IsPresent(l1tag, l1index);
		cout<<"X value is "<<x<<endl;
		//l1->print_cache();
		if(x){
			cout<<"========L1 Read hit========="<<endl;
			int loc=0;
			loc= l1->findcol(l1tag, l1index);
			//Update LRU
			l1->UpdateLRUHit(l1index,loc);
			cout<<"HITTT"<<endl;
		}
		else{
			cout<<"=====L1 read miss========="<<endl;
			cout<<"MISSS"<<endl;
			//L1 read miss
			int pos1= l1->FindLRU(l1index);
			//cout<<"Empty Col is "<<pos<<endl;
			// if there are empty cols in the index of l1 array use them
			if(l1->array[l1index][pos1].get_valid()==0){
				cout<<" ==========Empty block found in L1======="<<endl;
				l1->AllocateBlock(l1tag, l1index, pos1);
				//l1.print_cache();
				l1->UpdateLRUMiss(l1index);
				//l1.print_cache();
				l2read(l2tag, l2index);
			}
			//if there are no empty cols in l1 cache, identify the LRU and check if its dirty or not
			//if it is dirty, write it to l2 cache; Allocate the new block in it's place
			else{
				//Find the block which needs to be replaced.
				//int replaceposl1= l1.FindLRU(l1index);
				if(l1->IsVictimBlock(l1tag, l1index)){
					cout<<"=========Victim block exists===="<<endl;
					int pos2= l1->FindLRU(l1index);
					int l1_vb_tag=l1->get_tag(l1index, pos2);
					int Address= l1->get_address(l1_vb_tag,l1index,l1block_offset);
					int l2_vb_tag=l2->calculate_tag(Address);
					int l2_vb_index= l2->calculate_index(Address);
					//write the victim block into the l2 cache.
					l2write(l2_vb_tag, l2_vb_index);
				}
				//performing L2 read here
				l2read(l2tag, l2index);
				//Allocate the block at L1 cache
				l1->AllocateBlock(l1tag,l1index,pos1);
				l1->UpdateLRUMiss(l1index);	
			}
		}			
	}
};
	

int main(){
	//Cache l1(256,1,16);
	//l1.print_cache();
	//Cache l2(32,2,16);
	//l2.print_cache();
	CPU C(512,2,16,4*1024,1,16);
	//C.setL1parameters(256,1,16);
	//C.setL2parameters(8,2,16);
	//C.display_cache();
	C.write(0x4a96);
	C.display_cache();
	cout<<endl<<"====NEXT INSTRUCTION======="<<endl;
	C.read(0x4b94);
	C.display_cache();
	cout<<endl<<"====NEXT INSTRUCTION======="<<endl;
	C.read(0x4c94);
	C.display_cache();
	cout<<endl<<"====NEXT INSTRUCTION======="<<endl;
	C.write(0x4b90);
	C.display_cache();
	cout<<endl<<"====NEXT INSTRUCTION======="<<endl;
	cout<<endl;
	C.read(0xfa97);
	C.display_cache();
	cout<<endl<<"====NEXT INSTRUCTION======="<<endl;
	return 0;	
	
}

