#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
	int value;
	int weight;
	int parent, lchild, rchild;
}HTNode, *HuffmanTree;

HuffmanTree re_HT( FILE *HT_file );
void Decode( FILE *coded, HuffmanTree HT, FILE *original );
void decode( FILE *coded, HuffmanTree HT, FILE *original );

int main(void){
	FILE *HT_file = fopen( "010_test.huffman-tree", "r" );
	FILE *coded = fopen( "010_test.out", "r" );
	FILE *original = fopen( "010_test.original", "w" );

	HuffmanTree HT = re_HT( HT_file );
	Decode( coded, HT, original );

	fclose( HT_file );
	fclose( coded );
//The program will crush when I attemp to close File 'original'
	fclose( original );
	free( HT );
	return 0;
}

//-----------------IMPLEMENTATION------------------

HuffmanTree re_HT( FILE *HT_file ){
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

void Decode( FILE *coded, HuffmanTree HT, FILE *original ){
	//Get root location
	int cur = HT[1].parent;
	while( HT[cur].parent != 0 ){
		cur = HT[cur].parent;
	}
	int root = cur;

	int ch = 0;
	while( (ch=fgetc(coded)) != EOF ){
		if( ch == '0'){ cur = HT[cur].lchild; }
		else { cur = HT[cur].rchild; }
		if( HT[cur].lchild == 0 ){
			fputc( HT[cur].value, original );
			cur = root; //树指针回退
		}
	}
	fputc( '\n', original ); //Add a '\n' at the file end to simulate *nix file

	rewind( coded );
	rewind( original );
}

void decode( FILE *coded, HuffmanTree HT, FILE *original ){
}
