#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<inttypes.h>
#include<string.h>

typedef struct{
	int value;
	int weight;
	int parent, lchild, rchild;
}HTNode, *HuffmanTree;
typedef struct{
	int value;
	unsigned char *code;
}CodeNode, *HuffmanCode;

int statistic( FILE *in, FILE *stttc_file );
HuffmanTree generate_HT( FILE *stttc_file, int total, FILE *HT_file );
void select_two_small( HuffmanTree tree, int n, int *s1, int *s2 );
unsigned char **map_code_table( HuffmanCode HC );
HuffmanCode generate_HC( HuffmanTree tree, int total, FILE *HC_file );
void Code_file( FILE *in, HuffmanCode HC, FILE *out );
void code_file( FILE *in, HuffmanCode HC, FILE *out );

int main(void){
	FILE *in = fopen( "010_test.in", "r" );
	FILE *stttc_file = fopen( "010_test.statistic", "w+" );
	FILE *HT_file = fopen( "010_test.huffman-tree", "w+" );
	FILE *HC_file = fopen( "010_test.huffman-code", "w+" );
	FILE *out = fopen( "010_test.out", "w" );

	int symbol_total = statistic( in, stttc_file );
	HuffmanTree tree = generate_HT( stttc_file, symbol_total, HT_file );
	HuffmanCode code = generate_HC( tree, symbol_total, HC_file );
	code_file( in, code, out );
	
	fclose( in );
	fclose( stttc_file );
	fclose( HT_file );
	fclose( HC_file );
	fclose( out );
	free( tree );
	free( code );

	return 0;
}

//------------------------IMPLEMENTATION--------------------------
int statistic( FILE *in, FILE *stttc_file ){
	//Get statistic
	int count[128] = {0};
	int ch = 0;
	while( (ch=fgetc(in)) != EOF ){
		if( ch!='\n' )
			count[ch]++;
	}
	//Write file
	int total = 0;
	for( int i = 0; i < 128; i++ ){
		if( count[i] ){
			total++;
			fprintf( stttc_file, "%c %d\n", i, count[i] );
		}
	}
	//Return char-count
	rewind( in );
	rewind( stttc_file );
	return total;
}

HuffmanTree generate_HT( FILE *stttc_file, int total, FILE *HT_file ){
	//Get statistic to fill HT[1 ~ total]
	HuffmanTree HT = (HuffmanTree) malloc( sizeof(HTNode) * (2*total) ); //HT[0] not used.
	for( int i = 1; i <= total; i++ ){
		int wt, val, ch;
		fscanf( stttc_file, "%c%d", &val, &wt );
		while( (ch=fgetc(stttc_file)) != '\n' )
			;
		HT[i].value = val;
		HT[i].weight = wt;
		HT[i].parent = HT[i].lchild = HT[i].rchild = 0;
	}
	//Fill the initial HT[total+1 ~ 2*total-1]
	for( int i = total+1; i < 2*total; i++ ){
		HT[i].value = '-';
		HT[i].weight = HT[i].parent = HT[i].lchild = HT[i].rchild = 0;
	}
	//Generate HT
	for( int i = total+1; i < 2*total; i++ ){
		int s1, s2;
		select_two_small( HT, i-1, &s1, &s2 );
		HT[i].weight = HT[s1].weight + HT[s2].weight;
		HT[i].lchild = s1; HT[i].rchild = s2;
		HT[s1].parent = HT[s2].parent = i;
	}
	//Write to HT_file
	for( int i = 1; i < 2*total; i++ ){
		fprintf( HT_file, "%c%4d %4d %4d %4d\n",
			HT[i].value, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild );
	}
	//Return
	rewind( stttc_file );
	rewind( HT_file );
	return HT;
}

HuffmanCode generate_HC( HuffmanTree tree, int total, FILE *HC_file ){
	HuffmanCode HC = (HuffmanCode) malloc( (total+1)*sizeof(CodeNode) ); 
	    //HC[0] not used.
	//new a temporary string to calculate code
	unsigned char *code_string = (unsigned char *) malloc( total*sizeof(unsigned char) ); 
	    //The depth of HuffmanTree is less than total
	code_string[total-1] = '\0';
	for( int i = 1; i <= total; i++ ){
		int begin = total-1;
		int last = i;
		int cur = tree[last].parent;
		for( ; cur != 0; last = cur, cur = tree[cur].parent ){
			if( tree[cur].lchild == last ){
				code_string[--begin] = '0';
			} else if( tree[cur].rchild == last ){
				code_string[--begin] = '1';
			}
		}
		HC[i].value = tree[i].value;
		HC[i].code = (unsigned char*) malloc( (total-begin)*sizeof(unsigned char) );
		strcpy( (char *)HC[i].code, (char *)code_string+begin ); //Becos strcpy requires two char* arguments
	}
	free( code_string );
	//Write to HC_file
	for( int i = 1; i <= total; i++ ){
		fprintf( HC_file, "%c  %s\n", tree[i].value, HC[i].code );
	}
	//Return
	rewind( HC_file );
	return HC;
}
	

void Code_file( FILE *in, HuffmanCode HC, FILE *out ){
	unsigned char **code = map_code_table( HC );
	//Write
	int ch;
	while( (ch=fgetc(in)) != EOF ){
		if( code[ch] != NULL ){
			fprintf( out, "%s", code[ch] );
		}
	}
	fputc( '\n', out ); //Add a '\n' at the end of file to simulate *nix file format
	//Rewind
	rewind( in );
	rewind( out );
	free( code );
}

void code_file( FILE *in, HuffmanCode HC, FILE *out ){
	unsigned char **code = map_code_table( HC );

	int32_t char_total = 0;
	int ch;
	while( (ch=fgetc(in)) != EOF ){
		++char_total;
	}
	rewind( in );

	/* 先写入 out-file 原文件总的字符数
 	 * 再将原文件每个字符对应的编码以bit的方式，8个一组
 	 * 以unsigned char的方式写入 out-file
 	 * 最后，如果最后几个bit组不成byte，直接写入
 	 */
//或因为huffman编码的时候忽略了 '\n' 所以编码到 '\n' 时 code['\n']为NULL所以崩溃了
	fprintf( out, "%"PRId32, char_total );
	unsigned char cur_byte = 0;
	int cur_byte_remaining_length = 8;
	while( (ch=fgetc(in)) != EOF ){
		unsigned char *cur_bit_ptr = code[ch];
		while( *cur_bit_ptr != '\0' ){
			unsigned char cur_bit = *cur_bit_ptr++;
			cur_byte |= ( cur_bit ) << ( --cur_byte_remaining_length );
			if( cur_byte_remaining_length == 0 ){
				fputc( cur_byte, out );
				cur_byte_remaining_length = 8;
				cur_byte = 0;
			}
		}
	}
printf( ">>Write to out-file complete\n" );
	if( cur_byte_remaining_length != 8 ){
		fputc( cur_byte, out );
	}
	fputc( '\n', out ); //Add a '\n' at the end of file to simulate *nix file format
	
	rewind( in );
}

void select_two_small( HuffmanTree tree, int n, int *s1, int *s2 ){
	//Find s1  ( in \theta n )
	int count = 0;
	for( int i = 1; i <= n; i++ ){
		if( tree[i].parent == 0 ){
			if( count == 0 ){ 
				*s1 = i;
				++count;
			} else if( tree[i].weight < tree[*s1].weight ){ *s1 = i; }
		}
	}
	//Find s2  ( in \theta n )
	for( int i = 1; i <= n; i++ ){
		if( tree[i].parent == 0 ){
			if( count == 1 && i != *s1 ){
				*s2 = i;
				++count;
			} else if( tree[i].weight < tree[*s2].weight && i != *s1 ){
				*s2 = i;
			}
		}
	}
}

unsigned char **map_code_table( HuffmanCode HC ){
	unsigned char **code = (unsigned char**)malloc( sizeof(unsigned char*) * 128 );
	int i = 0;
	int j = 1;
	while( i++ < 128 ){
		if( i == HC[j].value ){ code[i] = HC[j++].code; }
	}
	return code;
}

