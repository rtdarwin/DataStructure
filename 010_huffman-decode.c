#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>
#include<string.h>

typedef struct{
	int value;
	int weight;
	int parent, lchild, rchild;
}HTNode, *HuffmanTree;

HuffmanTree rebuild_HT( FILE *HT_file );
int get_HT_root_location( HuffmanTree HT );
void Decode( FILE *coded, HuffmanTree HT, FILE *origin );
void decode( FILE *coded, HuffmanTree HT, FILE *origin );

int main(void){
	FILE *HT_file = fopen( "010_test.huffman-tree", "r" );
	FILE *coded = fopen( "010_test.out", "r" );
	FILE *origin = fopen( "010_test.origin", "w" );

	HuffmanTree HT = rebuild_HT( HT_file );
	decode( coded, HT, origin );

	fclose( HT_file );
	fclose( coded );
	fclose( origin );
	free( HT );
	return 0;
}

//-----------------IMPLEMENTATION------------------

HuffmanTree rebuild_HT( FILE *HT_file ){
	//Count lines
	int count = 0;
	int ch = 0;
	while( (ch=fgetc(HT_file)) != EOF ){
		if( ch == '\n' ){ ++count; }
	}

	//Re HT
	HuffmanTree HT = (HuffmanTree) malloc( sizeof(HTNode) * (count) ); //HT[0] not used.
	rewind( HT_file );
	for( int i = 1; i <= count; i++ ){
		fscanf( HT_file, "%c%d%d%d%d", 
			&(HT[i].value), &(HT[i].weight), &(HT[i].parent), &(HT[i].lchild), &(HT[i].rchild) );
		fgetc( HT_file ); //Dismiss the \n in the line end
	}
	
	return HT;
}

void Decode( FILE *coded, HuffmanTree HT, FILE *origin ){
	int root = get_HT_root_location( HT );

	int cur = root;
	int ch = 0;
	while( (ch=fgetc(coded)) != EOF ){
		if( ch == '0'){ cur = HT[cur].lchild; }
		else { cur = HT[cur].rchild; }
		if( HT[cur].lchild == 0 ){
			fputc( HT[cur].value, origin );
			cur = root; //树指针回退
		}
	}

	rewind( coded );
	rewind( origin );
}

void decode( FILE *coded, HuffmanTree HT, FILE *origin ){
	int32_t total = 0;
	fscanf( coded, "%d"SCNd32, &total );
	
	int bit_unused = 8;
	int decoded_char_count = 0;
	int HT_root = get_HT_root_location( HT );
	int cur_location_in_HT = HT_root;
	unsigned char cur_byte = fgetc( coded );
	while( decoded_char_count < total ){
		if( bit_unused == 0 ){ //Read another byte
			cur_byte = fgetc( coded );
			bit_unused = 8;
		}
		int cur_bit = cur_byte >> --bit_unused;

		if( cur_bit == '0'){ cur_location_in_HT = HT[cur_location_in_HT].lchild; }
		else { cur_location_in_HT = HT[cur_location_in_HT].rchild; }

		if( HT[cur_location_in_HT].lchild == 0 ){
			fputc( HT[cur_location_in_HT].value, origin );
			cur_location_in_HT = HT_root;
			decoded_char_count++;
		}
	}

	rewind( coded );
	rewind( origin );
}

int get_HT_root_location( HuffmanTree HT ){
	//Get root location
	int cur = HT[1].parent;
	while( HT[cur].parent != 0 ){
		cur = HT[cur].parent;
	}
	return cur;
}

