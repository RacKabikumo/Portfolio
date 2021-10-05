#include <iostream>
#include <stdio.h>
#include <stdlib.h> // atoi
#include <string.h>
#include <string>
#include <fstream> // Ū�ɮ� 
#include <vector>
#include <sstream> // 10��8,16 
using namespace std; 
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

static vector<string> source ;
static string fileName = "" ;


class LexicalAnalysis{
	private:
		vector<string> table1 ;
		vector<string> table2 ;
		vector<string> table3 ;
		vector<string> table4 ;
		string table5[100] ;
		string table6[100] ;
		string table7[100] ;
	public:
	
	void Copy( vector<string> &t1, vector<string> &t2, vector<string> &t3, vector<string> &t4, string t5[], string t6[], string t7[] ) {
		t1 = table1 ;
		t2 = table2 ;
		t3 = table3 ;
		t4 = table4 ;
		int i = 0 ;
		for ( i = 0 ; i < 100 ; i++ )
			t5[i] = table5[i] ;
		for ( i = 0 ; i < 100 ; i++ )
			t6[i] = table6[i] ;
		for ( i = 0 ; i < 100 ; i++ )
			t7[i] = table7[i] ;
	} // Copy
	
	void Init() {
		for ( int i = 0 ; i < 100 ; i++ ) {
			table5[i] = "" ;
		} // for
		for ( int j = 0 ; j < 100 ; j++ ) {
			table6[j] = "" ;
		} // for
		for ( int k = 0 ; k < 100 ; k++ ) {
			table7[k] = "" ;
		} // for
	} // Init()
	
	bool InputTable() {
		ReadTable( "1" ) ;
		ReadTable( "2" ) ;
		ReadTable( "3" ) ;
		ReadTable( "4" ) ;
	} // InputTable
	
	void ReadTable( string num ) {
		FILE *file = NULL ; // �ɮ׫��� 
		string name = "table" + num ;
		string filename = name + ".table" ;
		char number = name[name.length()-1] ;
		if ( number == '1' ) {
			table1.clear() ;
		} // if
		else if ( number == '2' ) {
			table2.clear() ;
		} // else if
		else if ( number == '3' ) {
			table3.clear() ;
		} // else if
		else if ( number == '4' ) {
			table4.clear() ;
		} // else if
		file = fopen( filename.c_str(), "r" ) ; 
		// ���}�ɮ� r�N��Ū c_str()�Nstring�����নC�y���Φ����r��`��(�L�k���N���) 
		if ( file == NULL ) {
			cout << endl << "�ɮ׶}�ҥ���!" << endl << endl ; 
		} // if
		else {  
			char instr[10] ;
			int num = fscanf( file, "%s", instr ) ;
			while ( num != -1 ) {
				if ( number == '1' ) {
					table1.push_back( instr ) ;
				} // if
				else if ( number == '2' ) {
					table2.push_back( instr ) ;
				} // else if
				else if ( number == '3' ) {
					table3.push_back( instr ) ;
				} // else if
				else if ( number == '4' ) {
					table4.push_back( instr ) ;
				} // else if
				
				
				num = fscanf( file, "%s", instr ) ;
			} // while
				
		} // else 	
		
		fclose( file ) ;
				
	} // ReadTable()
	
	void WriteFile( string line ) { 
		string name = fileName + "_output.txt" ;
		FILE *out ;
		out = fopen( name.c_str(), "a" ) ;
		if ( out == NULL ) {
			cout << "�ɮ׼g�J����!" << endl << endl ;
		} // if
		else {
			char temp[line.length()+1] ;
			temp[0] = '\0' ;
			strcpy( temp, line.c_str() ) ;
			fprintf( out, "%s", temp ) ;
		} // else

		fclose( out ) ;
	} // WriteFile
	
	bool IsWhiteSpace( string article, int index ) {
		if ( article[index] == ' ' || article[index] == '	' || article[index] == '\n' ) {
			return true ;
		} // if
		else return false ;
	} // IsWhiteSpace
	
	int PositionOfWord( string article, int index ) {
		while ( index < article.size() ) {
			if ( IsWhiteSpace( article, index ) ) {
				index++ ;
			} // if
			else {
				return index ;
			} // else
		} // while
		
		return -1 ; // ��촫��F-->�S�F�� 
	} // PositionOfWord()
	
	bool IsDelimiter( char word ) {
		for ( int i = 0 ; i < table4.size() ; i++ ) {
			if ( table4.at(i)[0] == word ) {
				return true ;
			} // if
		} // for
		
		return false ;
	} // IsDelimiter
	
	string CatchWord( string article, int &index ) {
		string temp = "" ;
		while ( !IsWhiteSpace( article, index ) && !IsDelimiter( article[index] ) && index < article.length() ) {
			temp = temp + article[index] ;
			index++ ;		
		} // while
		
		if ( temp == "" && index < article.length() && IsDelimiter( article[index] ) ) {
			temp = article[index] ; 
			index++ ;
		} // if
		return temp ;
			
	} // CatchWord()
	
	void FindTable4( string temp ) {
		for ( int i = 0 ; i < table4.size() ; i++ ) {
			if ( temp == table4.at(i) ) {
				char num[3] ;
				itoa( i+1, num, 10 ) ;
				string t = num ;
				string word = "(4," + t + ")" ;
				WriteFile( word ) ;
			} // if
		} // for
	} // FindTable4
	
	bool FindTable1( string temp ) { // a 97 A 65
		char name1[temp.length()+1] ;
		for ( int j = 0 ; j < temp.length() ; j++ ) { //�j�g���r��p�g 
			name1[j] = temp[j] ;
			if ( temp[j] >= 'A' && temp[j] <= 'Z' ) {
				name1[j] = temp[j] + 32 ;
			} // if	
		} // for
		name1[temp.length()] = '\0' ;
		string name = name1 ;
		for ( int i = 0 ; i < table1.size() ; i++ ) { // �p�g 
			if ( temp == table1.at(i) ) {
				char num[3] ;
				itoa( i+1, num, 10 ) ;
				string t = num ;
				string word = "(1," + t + ")" ;
				WriteFile( word ) ;
				return true ;
			} // if
			else if ( name == table1.at(i) ) { 
				char number[3] ;
				itoa( i+1, number, 10 ) ;
				string t1 = number ;
				string token = "(1," + t1 + ")" ;
				WriteFile( token ) ;
				return true ;
			} // else if
		} // for
		
		return false ;
	} // FindTable1
	
	bool FindTable2( string temp ) {
		char name1[temp.length()+1] ;
		for ( int j = 0 ; j < temp.length() ; j++ ) { //�p�g���r��j�g  
			name1[j] = temp[j] ;
			if ( temp[j] >= 'a' && temp[j] <= 'z' ) {
				name1[j] = temp[j] - 32 ;
			} // if
		} // for
		name1[temp.length()] = '\0' ;
		string name = name1 ;
		for ( int i = 0 ; i < table2.size() ; i++ ) { // �j�g 
			if ( temp == table2.at(i) ) {
				char num[3] ;
				itoa( i+1, num, 10 ) ;
				string t = num ;
				string word = "(2," + t + ")" ;
				WriteFile( word ) ;
				return true ;
			} // if
			else if ( name == table2.at(i) ) { 
				char number[3] ;
				itoa( i+1, number, 10 ) ;
				string t1 = number ;
				string token = "(2," + t1 + ")" ;
				WriteFile( token ) ;
				return true ;
			} // else if 
		} // for
		
		return false ;
	} // FindTable2
	
	bool FindTable3( string temp ) { // X�����D�|���ѨM!!!!!!!!!!!! 
		char name1[temp.length()+1] ;
		for ( int j = 0 ; j < temp.length() ; j++ ) { //�p�g���r��j�g   
			name1[j] = temp[j] ;
			if ( temp[j] >= 'a' && temp[j] <= 'z' ) {
				name1[j] = temp[j] - 32 ;
			} // if
		} // for
		name1[temp.length()] = '\0' ;
		string name = name1 ;
		for ( int i = 0 ; i < table3.size() ; i++ ) {  
			if ( temp == table3.at(i) ) {
				char num[3] ;
				itoa( i+1, num, 10 ) ;
				string t = num ;
				string word = "(3," + t + ")" ;
				WriteFile( word ) ;
				return true ;
			} // if
			else if ( name == table3.at(i) ) { 
				char number[3] ;
				itoa( i+1, number, 10 ) ;
				string t1 = number ;
				string token = "(3," + t1 + ")" ;
				WriteFile( token ) ;
				return true ;
			} // else if 
		} // for
		
		return false ;
	} // FindTable3
	
	void MakeHash( string temp ) {
		bool out = false ;
		for ( int i = 0 ; i < temp.length() && !out ; i++ ) {
			if ( temp[i] >= '0' && temp[i] <= '9' ) {
				;
			} // if
			else { // ���D�Ʀr��!! symbol!! 
				PutInTable567( temp, table5, "5" ) ;
				out = true ;
			} // else
		} // for
		
		if ( !out ) { // �O�Ʀr 
			PutInTable567( temp, table6, "6" ) ;
		} // if
	} // MakeHash
	
	void CheckTable( string temp, string article, int &index ) {
		if ( temp.length() == 1 && IsDelimiter( temp[0] ) ) { // temp�u���@�Ӧr-->�Ĥ@�Ӧr�Odelimiter 
			FindTable4( temp ) ; 
			if ( temp == "." ) {
				index = article.length() ; // ���ѥ��������n�F! 
			} // if
		} // if
		else {
			if ( !FindTable1( temp ) ) {
				if ( !FindTable2( temp ) ) {
					if ( article[index] != '\'' && !FindTable3( temp ) ) { // ���Otable1 2 3 4 �n�ۤv�إ� 
						MakeHash( temp ) ;
					} // if
					else if ( article[index] == '\'' && ( temp == "X" || temp == "x" || temp == "c" || temp == "C" ) ) { // �i��OC'EOF' X'F1' 
						PutSpec( temp, article, index ) ;
					} // else if
					else if ( article[index] == '\'' ) { // �i��OXsd'155 
						MakeHash( temp ) ;
					} // else
				} // if
			} // if
		} // else
	} // CheckTable
	
	void PutInTable567( string temp, string table[100], string tableNum ) {
		int sum = 0 ;
		for ( int i = 0 ; i < temp.length() ; i++ ) {
			sum = sum + temp[i] ;
		} // for
		bool done = false ;
		int index = sum % 100 ;
		while ( table[index] != "" && !done ) {
			if ( index == 99 ) {
				index = 0 ;
			} // if
			else if ( temp == table[index] ) {
				done = true ;
			} // else if
			else {
				index++ ;
			} // else 
		} // while
		
		table[index] = temp ;
		char number[3] ;
		itoa( index, number, 10 ) ;
		string t1 = number ;
		string token = "(" + tableNum + "," + t1 + ")" ;
		WriteFile( token ) ;
	} // PutInTable567 
	
	string CatchString( string temp, int &index ) {
		string t = "" ;
		string null = "" ;
		for ( int i = index ; i < temp.length() ; i++ ) {
			if ( temp[i] == '\'' ) {
				index = i ;
				return t ;
			} // if
		    else {
				t = t + temp[i] ;
			} // else
		} // for
		
		return null ;
	} // CatchString
	
	string CatchInt( string number, int &index ) {
		string temp = "" ;
		string null = "" ;
		for ( int i = index ; i < number.length() ; i++ ) {
			if ( number[i] == ' ' || number[i] == '\t' ) {
				;
			} // if
			else if ( number[i] == '\'' ) {
				index = i ;
				return temp ;
			} // else if
			else {
				temp = temp + number[i] ;
			} // else
		} // for
		
		return null ;
	} // CatchInt
	
	void PutSpec( string temp, string article, int &index ) {
		if ( temp == "X" || temp == "C" || temp == "c" || temp == "x" ) {
			int i = index ;
			string word = CatchWord( article, i ) ;
			int re = i ; // C'��X'�᭱��index��m 
			int position = i ;
			if ( word == "'" ) {
				string number = CatchInt( article, i ) ;
				string eof = CatchString( article, position ) ;
				bool out = false ;
				if ( ( temp == "X" || temp == "x" ) ) { // ��X'F1'���Φ�-->�T�{�O���O16�i��Ʀr! 
					for ( int j = 0 ; j < number.length() && !out ; j++ ) {
						if ( ( number[j] <= 'F' && number[j] >= 'A' ) || ( number[j] <= 'f' && number[j] >= 'a' ) ) {
							;
						} // if
						else if ( number[j] <= '9' && number[j] >= '0' ) {
							;
						} // else if
						else { // �̭��񪺤��O�Ʀr!!! -->���N�Osymbol-->�r�꦳�Ů�n���}��!!! 
							FindTable3( temp ) ; // ��X 
							FindTable4( word ) ; 
							string str = CatchWord( article, re ) ;
							bool label = false ;
							for ( int b = 0 ; b < str.length() ; b++ ) {
								if ( str[b] < '0' || str[b] > '9' ) {
									label = true ;
								} // if
							} // for
							if ( label ) {
								PutInTable567( str, table5, "5" ) ;
							} // if
							else {
								PutInTable567( str, table6, "6" ) ;
							} // else
							index = re ;
							out = true ;
						} // else
					} // for
					
					if ( !out ) { //�O�Ʀr(�a) 
						if ( number != "" ) { // X'F1'�����p 
							FindTable4( word ) ;
							for ( int a = 0 ; a < number.length() ; a++ ) {
								if ( number[a] >= 'a' && number[a] <= 'f' ) {
									number[a] = number[a] - 32 ;
								} // if
							} // for
							PutInTable567( number, table6, "6" ) ;
						} // if
						else { // �u���@��'�����p��X'' 
							string t = CatchWord( article, i ) ; // X'���᪺�r
							if ( t == "'" ) { // X''�����p 
								FindTable4( word ) ;
								FindTable4( t ) ;
							}  // if
							else if ( t == "" ) { // X'�����p 
								FindTable3( temp ) ; // ��X 
								FindTable4( word ) ;
							} // else if
							else { // X'asdf || X'123
								FindTable3( temp ) ; // ��X 
								FindTable4( word ) ;
								bool num = true ;
								for ( int k = 0 ; k < t.length() ; k++ ) { // �P�_�Osymbol�٬O�Ʀr 
									if ( t[k] < '0' || t[k] > '9' ) {
										num = false ;
									} // if
								} // for
								if ( num ) {
									PutInTable567( t, table6, "6" ) ;
								} // if
								else {
									PutInTable567( t, table5, "5" ) ;
								} // else
							} // else
						} // else

						index = i ;
					} // if
				} // if
				else if ( ( temp == "C" || temp == "c" ) ) { 
					if ( eof != "" ) { // ��C'6516as'�����p 
						FindTable4( word ) ;
						PutInTable567( eof, table7, "7" ) ;
					} // if
					else {
						string w = CatchWord( article, position ) ;
						if ( w == "'" ) { // C''�����p 
							FindTable4( word ) ;
							FindTable4( w ) ;
						} // if
						else if ( w == "" ) { // C'�����p 
							PutInTable567( temp, table5, "5" ) ;
							FindTable4( word ) ;
						} // else if
						else { // C'asd156�����p 
							PutInTable567( temp, table5, "5" ) ;
							FindTable4( word ) ;
							bool symbol = false ;
							for ( int j = 0 ; j < w.length() ; j++ ) {
								if ( w[j] < '0' || w[j] > '9' ) {
									symbol = true ;
								} // if
							} // for
							if ( symbol ) {
								PutInTable567( w, table5, "5" ) ;
							} // if
							else {
								PutInTable567( w, table6, "6" ) ;
							} // else
						} // else
					} // else
					
					index = position ;
				} // else if
			} // if
			else {
				PutInTable567( temp, table5, "5" ) ;
			} // else
		} // if
	} // PutSpec
	
	void CutToken() {
		string temp = "" ;
		string article = "" ;
		int line = 0 ;
		while ( line < source.size() ) {
			article = source.at(line) ; // ��line�椺�e
			WriteFile( article ) ;
			int index = 0 ;
			temp = "" ;
			index = PositionOfWord( article, index ) ; // �����L�e�����Ů�
			while ( index != -1 ) { // �����٨S�짹 
				temp = CatchWord( article, index ) ; // index��temp��@�Ӧr 
				// ��X�@�ӧ��㪺�r(�bwhite space��delimiter�e��word) index��whitespace��delimiter����m
				if ( temp != "" ) { // ���F��! 
					CheckTable( temp, article, index ) ;
				} // if
				index = PositionOfWord( article, index ) ; // ��o��U�@�Ӧr����m
				 
			} // while
			
			WriteFile( "\n" ) ;
			line++ ; // ���� 

			
			
		}  // while
		WriteTable567() ;
	} // CutToken()
	
	void WriteTable567() {
		string name = "Table5.table" ;
		int i = 0 ;
		FILE *out ;
		out = fopen( name.c_str(), "w" ) ;
		if ( out == NULL ) {
			cout << "�ɮ׼g�J����!" << endl << endl ;
		} // if
		else {
			i = 0 ;
			while ( i < 100 ) {
				fprintf( out, "%s\n", table5[i].c_str() ) ;
				i++ ;
			} // while
		} // else

		fclose( out ) ;
		
		out = fopen( "Table6.table", "w" ) ;
		if ( out == NULL ) {
			cout << "�ɮ׼g�J����!" << endl << endl ;
		} // if
		else {
			i = 0 ;
			while ( i < 100 ) {
				fprintf( out, "%s\n", table6[i].c_str() ) ;
				i++ ;
			} // while
		} // else

		fclose( out ) ;
		
		out = fopen( "Table7.table", "w" ) ;
		if ( out == NULL ) {
			cout << "�ɮ׼g�J����!" << endl << endl ;
		} // if
		else {
			i = 0 ;
			while ( i < 100 ) {
				fprintf( out, "%s\n", table7[i].c_str() ) ;
				i++ ;
			} // while
		} // else

		fclose( out ) ;
	} // WriteTable567
	
}; // LexicalAnalysis()


bool ReadInputSource() {
	source.clear() ;
	FILE *file = NULL ; // �ɮ׫��� 
	fileName = "" ;
	string name = "" ;
	cout << "�п�J�ɮצW��:\n(ex:�YŪ�J�ɮ׬�input.txt�A�п�Jinput)\n����!:�Y�n���ư���ۦP��input�ɮסA��ƱN�|���ۼg�b�P��output�ɮפU��A�G�Х��R�����e��output��!\n:" ;
	cin >> name ;
	fileName = name ;
	name =  name + ".txt" ;
	file = fopen( name.c_str(), "r" ) ; 
	  // ���}�ɮ� r�N��Ū c_str()�Nstring�����নC�y���Φ����r��`��(�L�k���N���) 
	if ( file == NULL ) {
		cout << endl << "�ɮ׶}�ҥ���!" << endl << endl ; 
		fclose( file ) ;
		return false ;
	} // if
	else { 
		char temp = '\0' ;
		int num = fscanf( file,"%c", &temp );
		while ( num != -1 ) {
			string line = "" ;
			int number = 9999 ;
			while ( temp != '\n' && number != -1 ) {
				line = line + temp ;
				number = fscanf( file,"%c", &temp );
			} // while
			
			if ( temp != '\n' ) {
				line = line + "\n" ;
			} // if
			else {
				line = line + temp ;
			} // else
			source.push_back( line ) ;
			if ( number != -1 ) {
				num = fscanf( file,"%c", &temp ) ;
			} // if
			else {
				num = -1 ;
			} // else
		} // while
					
	} // else 	
	
	fclose( file ) ;
	return true ;			
} // ReadInputSource()


void Lexical_Analysis( LexicalAnalysis &lex ) {
	char act = '\0' ;
	lex.Init() ;
	if ( ReadInputSource() ) {
		lex.InputTable() ;
		lex.CutToken() ;
		cout << endl << "���ntoken�o!" << endl ;
	} // if

} // Lexical_Analysis()

struct Format {
	string opcode ;
	int type ; // format
	int opnum ; // operand Number
	string optype ; // operand�Τ��� 
};

struct Symbol {
	string name ;
	string address ;
};

struct Table {
	string name ;
	int table ;
	int position ;
}; 

struct SIC {
	Table symbol ; 
	Table instr ;
	Table op1 ;
	Table op2 ;
	bool X = false ;
};

struct OutputXE {
	string loc ;
	SIC instruction ; 
	string allInstr ; // ���㪺���O 
	string code ;
	int PC = -1 ;
	bool error = false ;
	bool type4 = false ;
}; 

struct Output {
	string loc ;
	SIC instruction ; // ���n�����Otoken 
	string allInstr ; // ���㪺���O 
	string code ;
	bool error = false ;
};

class SyntaxAnalysis {
	private :
		vector<Output> output ;
		vector<OutputXE> outXE ;
		vector<string> input ;
		vector<string> table1 ;
		vector<string> table2 ;
		vector<string> table3 ;
		vector<string> table4 ;
		vector<Symbol> symtab ;
		int baseReg = 0 ;
		string table5[100] ;
		string table6[100] ;
		string table7[100] ;
		Format form[60] ;
		string literal[100] ; 
	public :
		void ReadFile() {
			string name = fileName + "_output.txt" ;
			FILE *file = NULL ; // �ɮ׫��� 
			file = fopen( name.c_str(), "r" ) ; 
	  		// ���}�ɮ� r�N��Ū c_str()�Nstring�����নC�y���Φ����r��`��(�L�k���N���) 
			if ( file == NULL ) {
				cout << endl << "�ɮ׶}�ҥ���!" << endl << endl ; 
				fclose( file ) ;
			} // if
			else { 
				string temp = "" ;
				char ch ;
				int num = fscanf( file, "%c", &ch );
				while ( num != -1 ) {
					if ( ch == '\n' ) {
						input.push_back(temp) ;
						temp = "" ;
					} // if
					else {
						temp = temp + ch ;
					} // else
		
					if ( num != -1 )
						num = fscanf( file, "%c", &ch );
				} // while
			} // else 	
	
			fclose( file ) ;
			
		} // ReadFile
		
		void ReadFormat() {
			string name = "optable.table" ;
			FILE *file = NULL ; // �ɮ׫��� 
			file = fopen( name.c_str(), "r" ) ; 
	  		// ���}�ɮ� r�N��Ū c_str()�Nstring�����নC�y���Φ����r��`��(�L�k���N���) 
			if ( file == NULL ) {
				cout << endl << "�ɮ׶}�ҥ���!" << endl << endl ; 
				fclose( file ) ;
			} // if
			else { 
				char temp[5] ;
				int num = fscanf( file, "%s", temp ) ;
				int i = 0 ;
				while ( num != -1 && i < 59 ) { 
					form[i].opcode = temp ;
					fscanf( file, "%d", &form[i].type ) ;
					fscanf( file, "%d", &form[i].opnum ) ;
					fscanf( file, "%s", temp ) ;
					form[i].optype = temp ;
					i++ ;
					num = fscanf( file, "%s", temp ) ;
				} // while
			} // else 	
	
			fclose( file ) ;
		} // ReadFormat
		
		void Copy( LexicalAnalysis lex ) {
			lex.Copy( table1, table2, table3, table4, table5, table6, table7 ) ;
		} // Copy
		
		void FindPosition( string format, int &table, int &position ) {
			int i = 3 ; // �Ĥ@�Ӥ@�w�O( �ĤG�ӬOtableNUM �ĤT�ӬO, 
			table = format[1] - '0' ;
			string temp = "" ;
			while ( i < format.length() ) {
				if ( format[i] == ')' ) {
					position = atoi(temp.c_str()) ;
				} // if
				else {
					temp = temp + format[i] ;
				} // else
				i++ ;
			} // while
		} // FindPosition
		
		bool NextIsNothing( int j, int table[], int position[], int num ) { // �T�{�᭱�O���O�S�F��F 
			j++ ;
			if ( j == num ) { // �S�F���! 
				return true ;
			} // if
			else if ( table[j] == 4 && position[j] == 10 ) { // ����(4,10) 
				return true ;
			} // else if
			else { // �٦��F��! 
				return false ;
			} // else
		} // NextIsNothing()
		
		bool TypeMofTable4( int j, int table[], int position[], int num, int type ) { // �Nopnum = 1 optype = m�����Ƶ{���X�g�b�o 
			j++ ;
			if ( type == 1 && j < num && ( table[j] == 6 || table[j] == 5 ) ) { // �i���Ʀr��symbol # 
				j++ ;
				if ( j == num ) { // �S�F���! 
					return true ;
				} // if
				else if ( table[j] == 4 && position[j] == 10 ) { // ����(4,10) 
					return true ;
				} // else if
				else { // �٦��F��! 
					return false ;
				} // else
			} // if
			else { // ��L�F��! �ΨS�F��F -->literal�S�����p 
				if ( type == 3 ) {
					if ( j < num && table[j] == 4 && position[j] == 9 ) { // C'EOF' || X'F1' =
						j++ ;
						if ( j < num && ( table[j] == 6 || table[j] == 7 ) ) {
							return true ;
						} // if
						else {
							return false ;
						} // else
					} // if
					else {
						return false ;
					} // else
				} // if
				else if ( type == 2 && j < num && table[j] == 5 ) { // symbol @
					j++ ;
					if ( j == num ) { // �S�F���! 
						return true ;
					} // if
					else if ( table[j] == 4 && position[j] == 10 ) { // ����(4,10) 
						return true ;
					} // else if
					else { // �٦��F��! 
						return false ;
					} // else
				} // else if
				else {
					return false ;
				} // else	
			} // else
			
		} // TypeMofTable4
		
		bool CheckTypeSeq( int table[], int position[], int num ) { // �ˬdtable���ǹ�� 
			int i = 0 ;
			int j = 0 ;
			int index = 0 ;
			int opnum = 0 ;
			string optype = "" ;
			Format temp ;
			while ( i < num ) {
				if ( table[i] == 1 ) { // ���O 
					index = position[i] - 1 ; // table1��position�q1�}�l 
					temp = form[index] ;
					opnum = temp.opnum ;
					optype = temp.optype ;
					j = i + 1 ;
					if ( opnum == 0 ) { // �Loperand 
						if ( j == num ) { // �����O�����̫�@��(���M�N�O������!) 
							return true ;
						} // if
						else if ( table[j] == 4 && position[j] == 10 ) { // ����(4,10) 
							return true ;
						} // else if
					} // if
					else if ( j == num ) { // �U�@�����ӭn���F��! opnum != 0
						return false ;
					} // else if
					else if ( opnum == 1 ) { // ���ӥu���@��operand 
						if ( optype == "r" ) { // ��reg	
							if ( table[j] == 3 ) { // �Oreg! 
								return NextIsNothing( j, table, position, num ) ;
							} // if
							else { // �᭱���Oreg! 
								return false ;
							} // else
						} // if
						else if ( optype == "m" ) { // �᭱��symbol �� # �� @ �� literal �� address 
							if ( table[j] == 5 ) { // �᭱��symbol 
								j++ ;
								if ( j < num && ( table[j] == 4 && position[j] == 1 ) ) { // ,X �����p-->index 
									j++ ;
									if ( j < num && ( table[j] == 3 && position[j] == 2 ) ) { // X
										return NextIsNothing( j, table, position, num ) ;
									} // if
									else { // ���OX!! 
										return false ;
									} // else
								} // if
								else {
									return NextIsNothing( j-1, table, position, num ) ;
								} // else
							} // if
							else if ( table[j] == 4 && position[j] == 12 ) { // #
								return TypeMofTable4( j, table, position, num, 1 ) ;
							} // else if
							else if ( table[j] == 4 && position[j] == 13 ) { // @
								return TypeMofTable4( j, table, position, num, 2 ) ;
							} // else if
							else if ( table[j] == 4 && position[j] == 11 ) { // =
								return TypeMofTable4( j, table, position, num, 3 ) ;
							} // else if
							else if ( table[j] == 6 ) { // address
								return NextIsNothing( j, table, position, num ) ;
							} // else if
							else { // �᭱��������! 
								return false ;
							} // else
						} // else if
						else { // �u�ѤUn�� 
							if ( table[j] == 6 ) { // �������Ʀr(�a) 
								return true ;
							} // if
							else { // ���O�Ʀr! 
								return false ;
							} // else
						} // else
					} // else if
					else { // �̦h���operand!  
						if ( optype == "rr" ) { // �Ҭ�reg 
							if ( table[j] == 3 ) { // �Ĥ@�ӬOreg 
								j++ ;
								if ( j == num ) { // �u���@��operand 
									return false ;
								} // if
								else if ( table[j] == 4 && position[j] == 1 ) { // �U�@�ӬO, 
									j++ ;
									if ( j == num ) { // �����٦� 
										return false ;
									} // if
									else if ( table[j] == 3 ) { // ��2��reg�F�T�{�᭱�٦��S�� 
										return NextIsNothing( j, table, position, num ) ;
									} // else if
									else { // �������Oreg 
										return false ;
									} // else
								} // else if
								else { // �������O,
									return false ;
								} // else
							} // if
							else { // ���Oreg! 
								return false ;
							} // else
						} // if
						else { // �u��rn��type 
							if ( table[j] == 3 ) { // �Ĥ@�ӬOreg 
								j++ ;
								if ( j == num ) { // �u���@��! 
									return false ;
								} // if
								else if ( table[j] == 4 && position[j] == 1 ) { // ,
									j++ ;
									if ( j == num ) {
										return false ;
									} // if
									else if ( table[j] == 6 ) { // reg,n
										return NextIsNothing( j, table, position, num ) ;
									} // else if
									else { // �������O�Ʀr! 
										return false ;
									} // else
								} // else if
								else { // �������O, 
									return false ;
								} // else
							} // if
						} // else
					} // else
				} // if
				else if ( table[i] == 2 ) { // �����O! 
					j = i + 1 ;
					if ( position[i] == 1 ) { // START
						if ( j == num ) {
							return false ;
						} // if
						else if ( table[j] == 6 ) {
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else { // �������O�Ʀr 
							return false ;
						} // else
					} // if
					else if ( position[i] == 2 ) { // END
						if ( j == num ) { // �᭱�i�H�S�F�� 
							return true ;
						} // if
						else if ( table[j] == 5 ) {
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else { // �������Osymbol 
							return false ;
						} // else
					} // else if
					else if ( position[i] == 9 ) { // LTORG
						return NextIsNothing( i, table, position, num ) ;
					} // else if
					else if ( position[i] == 8 ) { // BASE �᭱�i�H��symbol��decNum 
						if ( j == num ) { 
							return false ;
						} // if
						else if ( table[j] == 5 ) {
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else if ( table[j] == 6 ) {
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else {
							return false ;
						} // else
					}  // else if
					else if ( position[i] == 5 || position[i] == 6 ) { // RESW RESB
						if ( j == num ) {
							return false ;
						} // if
						else if ( table[j] == 6 ) {
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else {
							return false ;
						} // else
					} // else if
					else if ( position[i] == 3 || position[i] == 4 ) { // WORD BYTE
						if ( j == num ) {
							return false ;
						} // if
						else if ( table[j] == 6 ) { // decNum
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else if ( table[j] == 4 && position[j] == 9 ) { // X'F1' or C'EOF'
							j++ ;
							if ( j == num ) {
								return false ;
							} // if
							else if ( table[j] == 6 || table[j] == 7 ) {
								j++ ;
								if ( j == num ) { // ��' 
									return false ;
								} // if
								else if ( table[j] == 4 && position[j] == 9 ) { // �ĤG��' 
									return NextIsNothing( j, table, position, num ) ;
								} // else if
								else { // �᭱�O��L�F�� 
									return false ;
								} // else
							} // else if
							else {
								return false ;
							} // else
						} // else if
						else {
							return false ;
						} // else
					} // else if
					else if ( position[i] == 7 ) { // EQU �᭱�����򳣦��i�� �����L 
						if ( table[j] == 6 ) { // �᭱���Ʀr(dec) 
							return NextIsNothing( j, table, position, num ) ;
						} // if
						else if ( table[j] == 5 ) { // symbol���i�঳�|�h�B��(QQ)
							for ( int k = j+1 ; k < num ; k++ ) {
								if ( table[k] == 4 ) {
									if ( position[k] > 5 && position[k] < 2 ) { // �D + - * /
										return false ;
									} // if
								} // if
							} // for
							return true ;
						} // else
						else if ( table[j] == 4 && position[j] == 4 ) { // *
							return NextIsNothing( j, table, position, num ) ;
						} // else if
						else {
							return false ;
						} // else 
					} // else if
				} // else if
				else if ( table[i] == 4 && position[i] == 10 ) { // ���� 
					if ( i == 0 ) {
						return true ;
					}  // if
					else {
						return false ;
					} // else
				} // else if
				i++ ;
			} // while
			
			return false ;
		} // CheckTypeSeq
		
		bool CheckError( string code, string format, int table[], int position[] ) {
			string temp = "" ;
			int i = 0 ;
			int num = 0 ; // �⦳�X��token 
			while ( i < format.length() && num < 10 ) {
				if ( format[i] != ')' ) {
					temp = temp + format[i] ;
				} // if
				else {
					temp = temp + format[i] ;
					FindPosition( temp, table[num], position[num] ) ;// �n���s�F�裫���������� �n�ݶ��ǭ� 
					num++ ;
					temp = "" ;
				} // else
				i++ ;
				
			} // while
			table[num] = -1 ;
			position[num] = -1 ;
			return !CheckTypeSeq( table, position, num ) ; // �ˬdtable���ǹ�� 
		} // CheckError 
		
		string BigEnglish( string temp ) {
			for ( int i = 0 ; i < temp.length() ; i++ ) {
				if ( temp[i] <= 'z' && temp[i] >= 'a' ) {
					temp[i] = temp[i] - 32 ; // �p�g��j�g 
				} // if
			} // for
			
			return temp ;
		} // BigEnglish
		
		void SeparateWord( string code, int table[], int position[], Output &o ) {
			o.allInstr = code ; // ��ӫ��O! 
			int i = 0 ;
			int num = position[0] ;
			if ( table[i] == 5 ) { // �w�qsymbol! 
				o.instruction.symbol.name = BigEnglish( table5[num] ) ; // �Ĥ@�Ӧp�G�Osymbol�N�n�w�qsymbol�A���O�p�G���O�}�Y�N�O���O 
				o.instruction.symbol.table = 5 ;
				o.instruction.symbol.position = num ;
				i++ ;
			} // if
			num = position[i] ;
			int t1to4Num = num - 1 ; // �]��table1~4���O�q1�}�l�� �ҥHŪ�쪺�Ʀr�n-1 
			int opnum = form[t1to4Num].opnum ; // form�O�̷�table1�����ҥH�]�n-1 
			if ( table[i] == 1 ) {
				o.instruction.instr.name = BigEnglish( table1.at(t1to4Num) ) ;
				o.instruction.instr.table = 1 ;
				o.instruction.instr.position = t1to4Num ;
				if ( opnum != 0 ) { // ��OP 
					i++ ;
					num = position[i] ;
					t1to4Num = num - 1 ;
					if ( table[i] == 3 ) { // op1�Oreg 
						o.instruction.op1.name = table3.at(t1to4Num) ;
						o.instruction.op1.table = 3 ;
						o.instruction.op1.position = t1to4Num ;
					} // if
					else if ( table[i] == 5 ) { // op1�Osymbol 
						o.instruction.op1.name = BigEnglish( table5[num] ) ;
						o.instruction.op1.table = 5 ;
						o.instruction.op1.position = num ;
					} // else if
					else if ( table[i] == 6 ) { // �����O�Ʀr 
						o.instruction.op1.name = table6[num] ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					}  // else if
					else if ( ( table[i] == 4 && ( 11 <= num ) && ( num <= 13 ) ) ) { // = # @�����p 
						i++ ;
						num = position[i] ;
						if ( num == 12 ) { // #
							if ( table[i] == 5 ) {
								o.instruction.op1.name = "#" + BigEnglish( table5[num] ) ;
								o.instruction.op1.table = 5 ;
								o.instruction.op1.position = num ;
							} // if
							else if ( table[i] == 6 ) {
								o.instruction.op1.name = "#" + BigEnglish( table6[num] ) ;
								o.instruction.op1.table = 6 ;
								o.instruction.op1.position = num ;
							} // else if	
						} // if
						else if ( num == 13 ) { // @ �u��symbol 
							if ( table[i] == 5 ) {
								o.instruction.op1.name = "@" + BigEnglish( table5[num] ) ;
								o.instruction.op1.table = 5 ;
								o.instruction.op1.position = num ;
							} // if
						} // else if
						else { // =
							if ( table[i] == 4 && position[i] == 9 ) { // C'' X''
								i++ ;
								num = position[i] ;
								if ( table[i] == 6 ) { // X''
									o.instruction.op1.name = "X'" + BigEnglish( table6[num] ) + "'" ;
									o.instruction.op1.table = 6 ;
									o.instruction.op1.position = num ;
								} // if
								else { // C''
									o.instruction.op1.name = "C'" + table7[num] + "'" ; // ������j�g ���٭n��ASCII 
									o.instruction.op1.table = 7 ;
									o.instruction.op1.position = num ;
								} // else
							} // if
						} // else
					} // else if
					// op1��n�P!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					i++ ;
					if ( opnum == 1 ) {
						if ( table[i] == 4 && position[i] == 1 ) { // symbol�ᦳ,-->�w�g�T�{��k�S���N��O,X�����p 
							o.instruction.X = true ; // ��X!!!!!! 
							o.instruction.op2.name = "X" ;
							o.instruction.op2.table = 3 ;
							o.instruction.op2.position = 2 ; // X reg
						} // if
					}  // if
					if ( opnum == 2 ) { // ��op2!!! 
						if ( table[i] == 4 && position[i] == 1 ) { // symbol�ᦳ,
							i++ ;
							if ( table[i] == 3 ) { // op2�Oreg 
								o.instruction.op2.name = table3.at(position[i]-1) ; // ���s��m��+1 t1~4 
								o.instruction.op2.table = 3 ;
								o.instruction.op2.position = position[i]-1 ;
							} // if
							else { // op2�O�Ʀr 
								o.instruction.op2.name = table6[position[i]] ;
								o.instruction.op2.table = 6 ;
								o.instruction.op2.position = position[i] ;
							} // else
						} // if
					}  // if
				} // if
			} // if
			else if ( table[i] == 2 ) { // �w�qsymbol �᭱�@�w�Otable1 or 2 ���D�O���� 
				o.instruction.instr.name = table2.at(t1to4Num) ;
				o.instruction.instr.table = 2 ;
				o.instruction.instr.position = t1to4Num ;
				i++ ;
				if ( num == 3 || num == 4 ) { // WORD BYTE
					num = position[i] ;
					if ( table[i] == 6 ) { //�᭱�����O�Ʀr 
						o.instruction.op1.name = table6[num] ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					} // if
					else if ( table[i] == 4 && num == 9 ) { // C'EOF' X'F1'
						i++ ;
						num = position[i] ;
						t1to4Num = num - 1 ;
						if ( table[i] == 6 ) { // X'F1'
							o.instruction.op1.name = "X'" + BigEnglish( table6[num] ) + "'" ;
							o.instruction.op1.table = 6 ;
							o.instruction.op1.position = num ;
						} // if
						else if ( table[i] == 7 ) { // C'EOF'
							o.instruction.op1.name = "C'" + table7[num] + "'" ;
							o.instruction.op1.table = 7 ;
							o.instruction.op1.position = num ;
						} // else if
					} // else if
				} // if
				else if ( num == 1 ) { // START�᭱�@�w���Ʀr 
					num = position[i] ;
					o.instruction.op1.name = table6[num] ;
					o.instruction.op1.table = 6 ;
					o.instruction.op1.position = num ;
				} // else if
				else if ( num == 8 ) { // BASE�᭱��symbol��decnum 
					num = position[i] ;
					if ( table[i] == 5 ) {
						o.instruction.op1.name = BigEnglish( table5[num] ) ;
						o.instruction.op1.table = 5 ;
						o.instruction.op1.position = num ;
					} // if
					else {
						o.instruction.op1.name = BigEnglish( table6[num] ) ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					} // else
				} // else if
				else if ( num == 5 || num == 6 ) { // RESW RESB �᭱�u���Ʀr 
					num = position[i] ;
					o.instruction.op1.name = table6[num] ;
					o.instruction.op1.table = 6 ;
					o.instruction.op1.position = num ;
				} // else if
				else if ( num == 7 ) { // EQU �᭱�i��O�@��_�Ǫ��F��@@ ���������s! 
					string temp = "" ;
					if ( table[i] == 6 ) {
						num = position[i] ;
						o.instruction.op1.name = table6[num] ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					} // if
					else if ( table[i] == 4 && position[i] == 4 ) { // *
						o.instruction.op1.name = "*" ;
						o.instruction.op1.table = 4 ;
						o.instruction.op1.position = 4 ;
					} // else if
					else { // label �i�঳�|�h�B�� 
						for ( int j = 0 ; j < code.length() ; j++ ) {
							if ( code[j] != '	' ) {
								temp = temp + code[j] ;
							} // if
							else {
								if ( temp == "EQU" || temp == "equ" ) { // �Lliteral 
									temp = "" ;
									temp.append( code, j+1, code.length()-j ) ; // �qj+1�}�l���Xlen-j�Ӧr���temp-->�ѤU���r 
								} // if
								else {
									temp = "" ;
								} // else
							} // else
						} // for
					
						o.instruction.op1.name = BigEnglish( temp ) ;
					} // else
					
				} // else if
				// LTORG�᭱�S�F��@@ END ������ 
			} // else if
			else if ( table[i] == 4 && num == 10 ) { // ����! 
				;
			} // else if
		} // SeparateWord()
		
		int LocStrtoNum( string strLoc ) {
			stringstream ss ;
			int loc = 0 ;
			ss << hex << strLoc ; // �N�r��H16�i��Ū�J 
			ss >> loc ; // ��X��10�i�� (100=>256) 
			ss.clear() ; // �n�O�o�M��!
			return loc ;
		} // LocStrtoNum
		
		string LocNumtoStr( int loc ) {
			stringstream ss ;
			string strLoc = "" ;
			ss << hex << loc ; // �Nloc�ন16�i��
			ss >> strLoc ; // ��X���r�� 
			ss.clear() ;
			strLoc = BigEnglish( strLoc ) ;
			return strLoc ;
		} // LocStrtoNum
		
		string FillStr( string temp, int num ) {
			while ( temp.length() < num ) {
				temp = "0" + temp ;
			} // while
			
			return temp ;
		} // FillStr
		
		void TranslatePass1() { // Pass1�o! literal!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			string strLoc = "0000" ; // �r�ꪺlocation 
			int loc = 0 ; // �p��Ϊ�loc
			int i = 0 ;
			stringstream ss ; // �ഫ�i�� 
			int move = 0 ; // BYTE WORD�� 
			string temp = "" ; // C''||EQU�� 
			int num = 0 ; // C''�� 
			while ( i < output.size() ) {
				Symbol s ;
				Output o = output.at(i) ;
				SIC ins = o.instruction ;
				if ( ins.instr.name == "START" ) { // �]�w�{���_�l��m! ����½�����X 
					strLoc = ins.op1.name ; // �{���_�l��m
					strLoc = BigEnglish( strLoc ) ; // �쥻LocNumtoStr�w�g����j�g �i�O�o�̬O�@�}�l �S�Ψ� �ҥH�n�� 
					loc = LocStrtoNum( strLoc ) ;  
					strLoc = FillStr( strLoc, 4 ) ; // �񺡺�4�� 
					output.at(i).loc = strLoc ;
					if ( ins.symbol.name != "" ) {
						s.name = ins.symbol.name ; // �Nsymbol�s�Jsymboltable 
						s.address = strLoc ;
						for ( int k = 0 ; k < symtab.size() ; k++ ) {
							if ( symtab.at(k).name == ins.symbol.name ) {
								output.at(i).error = true ;
								output.at(i).code = "Duplicated Definition!" ;
							} // if
						} // for
						if ( !output.at(i).error ) { // �S���D�����O �N�i�Hpush 
							symtab.push_back( s ) ;
						} // if
					} // if
				} // if
				else {
					if ( ins.instr.name != "" ) { // ���O���ѩΪ̪Ū� 
						if ( ins.instr.name != "END" && ins.instr.name != "BASE" ) { // �S����  �]��BASE�]���κ�LOC 
							if ( !output.at(i).error ) { // �S�������O�~�n�B�z 
								if ( ins.instr.name == "RESB" ) {
									output.at(i).loc = FillStr( strLoc, 4 ) ;
									ss << dec << ins.op1.name ; // �r����Ʀr 
									ss >> move ;
									ss.clear() ;
									loc = loc + move ;
									strLoc = LocNumtoStr( loc ) ;
								} // if
								else if ( ins.instr.name == "RESW" ) {
									output.at(i).loc = FillStr( strLoc, 4 ) ;
									ss << dec << ins.op1.name ; // �r����Ʀr 
									ss >> move ;
									ss.clear() ;
									move = move * 3 ; // word��3byte
									loc = loc + move ;
									strLoc = LocNumtoStr( loc ) ;
								} // else if
								else if ( ins.instr.name == "BYTE" || ins.instr.name == "WORD" ) {
									output.at(i).loc = FillStr( strLoc, 4 ) ;
									if ( ins.op1.name[0] == 'C' ) { // C'EOF' || X'F1' 
										temp = "" ;
										for ( int j = 2 ; j < ins.op1.name.length() ; j++ ) {
											if ( ins.op1.name[j] != '\'' ) {
												num = ins.op1.name[j] ;
												output.at(i).code = output.at(i).code + LocNumtoStr( num ) ;
											} // if
											else {
												break ;
											} // else
										} // for
										move = output.at(i).code.length() ;
										output.at(i).code = BigEnglish( output.at(i).code ) ;
										if ( ins.instr.name == "WORD" ) { // �n��6�ӫ��O 
											output.at(i).code = FillStr( output.at(i).code, 6 ) ;
											move = 6 ;
										} // if
										else if ( move % 2 != 0 ) { // code���׬��_�� 
											output.at(i).code = "0" + output.at(i).code ;
											move++ ;
										} // else if
										move = move / 2 ;
										loc = loc + move ;
										strLoc = LocNumtoStr( loc ) ;
									} // if
									else if ( ins.op1.name[0] == 'X' ) {
										temp = "" ;
										for ( int j = 2 ; j < ins.op1.name.length() ; j++ ) {
											if ( ins.op1.name[j] != '\'' ) {
												output.at(i).code = output.at(i).code + ins.op1.name[j] ;
											} // if
											else {
												break ;
											} // else
										} // for
										output.at(i).code = BigEnglish( output.at(i).code ) ;
										move = output.at(i).code.length() ;
										if ( ins.instr.name == "WORD" ) { // �n��6�ӫ��O 
											output.at(i).code = FillStr( output.at(i).code, 6 ) ;
											move = 6 ;
										} // if
										else if ( move % 2 != 0 ) { // code���׬��_�� 
											output.at(i).code = "0" + output.at(i).code ;
											move++ ;
										} // else if
										move = move / 2 ;
										loc = loc + move ;
										strLoc = LocNumtoStr( loc ) ;
									} // else if
									else { // dec num
										output.at(i).loc = FillStr( strLoc, 4 ) ;
										ss << dec << ins.op1.name ; // �r����Ʀr 
										ss >> num ; 
										ss.clear() ;
										ss << hex << num ; // �N�Ʀr�H16�i��Ū�J�ন�r�� 
										ss >> output.at(i).code ;
										ss.clear() ;
										output.at(i).code = BigEnglish( output.at(i).code ) ;
										move = output.at(i).code.length() ;
										if ( ins.instr.name == "WORD" ) { // �n��6�ӫ��O 
											output.at(i).code = FillStr( output.at(i).code, 6 ) ;
											move = 6 ;
										} // if
										else if ( move % 2 != 0 ) { // code���׬��_�� 
											output.at(i).code = "0" + output.at(i).code ;
											move++ ;
										} // else if
										move = move / 2 ;
										loc = loc + move ;
										strLoc = LocNumtoStr( loc ) ;
									} // else
								} // else if
								else if ( ins.instr.name == "EQU" ) { // ���v�T�U�@�D���O����m 
									if ( ins.op1.table == 6 ) {
										ss << dec << ins.op1.name ;
										ss >> move ;
										ss.clear() ;
										temp = "" ;
										ss << hex << move ;
										ss >> temp ;
										ss.clear() ;
										temp = BigEnglish( temp ) ;
										output.at(i).loc = FillStr( temp, 4 ) ;
									} // if
									else if ( ins.op1.table == 4 ) { // * �����O���`�⪺address 
										output.at(i).loc = FillStr( strLoc, 4 ) ;
										// �U�@�D���OLOC�@�� 
									} // else if
									else { // label�o! 
										temp = ins.op1.name ;
										string t = "" ; // �Ȧssymbol 
										int sum = 0 ; // �Ƴ̫�Loc
										char c = '\0' ; 
										for ( int j = 0 ; j < temp.length() ; j++ ) {
											if ( temp[j] != '+' || temp[j] != '*' || temp[j] != '-' || temp[j] != '/' ) {
												t = t + temp[j] ;
											} // if
											else {
												ss << hex << FindSymLoc( t ) ;
												ss >> move ;
												ss.clear() ;
												if ( sum == 0 ) {
													sum = move ;
												} // if
												else if ( c == '-' ) {
													sum = sum - move ;
												} // else if
												else if ( c == '+' ) {
													sum = sum + move ;
												} // else if
												else if ( c == '*' ) {
													sum = sum * move ;
												} // else if
												else if ( c == '/' ) {
													sum = sum / move ;
												} // else if
												c = temp[j] ;
											} // else
										} // for
										if ( sum == 0 ) { // ���i��N�@��label 
											ss << hex << FindSymLoc( t ) ;
											ss >> move ;
											ss.clear() ;
											sum = move ;
										} // if
										else if ( c == '-' ) { // �̫�@��symbol�S�p��� 
											sum = sum - move ;
										} // else if
										else if ( c == '+' ) {
											sum = sum + move ;
										} // else if
										else if ( c == '*' ) {
											sum = sum * move ;
										} // else if
										else if ( c == '/' ) {
											sum = sum / move ;
										} // else if
										ss << hex << sum ;
										ss >> temp ;
										ss.clear() ;
										temp = BigEnglish( temp ) ;
										output.at(i).loc = FillStr( temp, 4 ) ;
									} // else
								} // else if
								else {
									output.at(i).loc = FillStr( strLoc, 4 ) ;
									loc = loc + 3 ; // �T�w+3 
									strLoc = LocNumtoStr( loc ) ;
								} // else
							} // if
						} // if
						if ( ins.symbol.name != "" ) { // ���n�w�qsymbol 
							s.name = ins.symbol.name ; // �Nsymbol�s�Jsymboltable 
							s.address = output.at(i).loc ;
							for ( int k = 0 ; k < symtab.size() ; k++ ) {
								if ( symtab.at(k).name == ins.symbol.name ) {
									output.at(i).error = true ;
									output.at(i).code = "Duplicated Definition!" ;
								} // if
							} // for
							if ( !output.at(i).error ) { // �S���D�����O �N�i�Hpush 
								symtab.push_back( s ) ;
							} // if
						} // if
						// END������ 
					} // if
					// ���ѩΪŪ��]������! 
				} // else
				i++ ;
			} // while
		} // TranslatePass1
		
		void TranslatePass2() {
			stringstream ss ;
			string addr = "" ; // ½�����X�ݭn��binary�h�ഫ 
			string temp = "" ;
			int x = 0 ;
			for ( int i = 0 ; i < output.size() ; i++ ) {
				Output o = output.at(i) ;
				SIC instr = o.instruction ;
				if ( o.code == "" && instr.instr.name != "" ) { // ��k���T&�٨S�⪺&�D���� 
					if ( instr.instr.table == 1 ) { // �n�doptable�n�Otable1 
						if ( instr.op1.name == "" ) { // �S��operand 
							output.at(i).code = form[instr.instr.position].opcode + "0000" ; 
							output.at(i).allInstr = output.at(i).allInstr + "\t" ;
						} // if
						else if ( instr.op1.table == 5 ) { // ��symtab 
							addr = FindSymLoc( instr.op1.name ) ;
							if ( addr != "" ) { // �䤣��loc-->�S�w�q��symbol 
								if ( instr.X ) {
									temp.append( addr, 0, 1 ) ; // �̰��줸���X�� �n+8(X=1) 
									ss << hex << temp ;
									ss >> x ;
									ss.clear() ;
									x = x + 8 ;
									ss << hex << x ;
									ss >> temp ;
									addr[0] = temp[0] ;
									ss.clear() ;
								} // if
								output.at(i).code = form[instr.instr.position].opcode + addr ;
							} // if
							else { // �B�zUndefine Symbol-->��LOC�|�����D(����S���a) 
								for ( int j = 0 ; j < symtab.size() ; j++ ) {
									if ( symtab.at(j).name == o.instruction.symbol.name ) {
										symtab.erase( symtab.begin() + j ) ;
										break ;
									} // if
								} // for
								output.at(i).loc = "" ;
								output.at(i).code = "Undefine Symbol!" ;
								output.at(i).error = true ;
							} // else
						} // else if
					} // if
					
				} // if
			} // for
		} // TranslatePass2
		
		string FindSymLoc( string temp ) {
			for ( int i = 0 ; i < symtab.size() ; i++ ) {
				if ( symtab.at(i).name == temp ) {
					return symtab.at(i).address ;
				} // if
			} // for
			string t = "" ;
			return t ;
		} // FindSymtab
		
		void OutputSIC() {
			string name = fileName + "_SICMachineCode.txt" ;
			FILE *file = NULL ; // �ɮ׫��� 
			file = fopen( name.c_str(), "w" ) ; 
	  		// ���}�ɮ� r�N��Ū c_str()�Nstring�����নC�y���Φ����r��`��(�L�k���N���) 
			if ( file == NULL ) {
				cout << endl << "�ɮ׶}�ҥ���!" << endl << endl ; 
				fclose( file ) ;
			} // if
			else { 
				int k = 0 ;
				fprintf( file, "Line	Loc	Source statement		Object code\n" ) ;
				for ( int j = 0 ; j < output.size() ; j++ ) {
					Output o = output.at(j) ;
					if ( o.allInstr != "" ) {
						k = k + 5 ;	
						fprintf( file, "%d	%s	%s	", k, o.loc.c_str(), o.allInstr.c_str() ) ;
						//cout << k << "\t" << o.loc << "\t" << o.allInstr << "\t" ; 
						if ( !o.instruction.X ) {
							fprintf( file, "\t" ) ;
							//cout << "\t" ;
						} // if
						fprintf( file, "%s\n", o.code.c_str() ) ;
						//cout << o.code << endl ;
					} // if
					else {
						fprintf( file, "\n" ) ;
					} // else
					
				} // for
			} // else 	
	
			fclose( file ) ;
		} // OutputSIC
		
		void TurnMachineCodeSIC() { // ������X SIC
			int line = 0 ;
			int table[10] = {0} ;
			int position[10] = {0} ;
			while ( line < input.size() ) {
				string code = input.at(line) ; // ���X��line�檺�F��
				line++ ;
				Output o ;
				while ( ( code == "" || code == "\n" ) && line < input.size()-1 ) {
					if ( line % 2 == 0 ) { // index�q0�}�l 
						output.push_back( o ) ; // �Ū��]�n�L!
					} // if
					code = input.at(line) ; // ���X��line�檺�F��
					line++ ;
				} // while
				string format = input.at(line) ; // ���X��line�檺�F��
				if ( format != "" ) { // ���O�Ū��~�P�_ 
					if ( !CheckError( code, format, table, position ) ) { // �ˬd��k���S����-->�Lerror 
						SeparateWord( code, table, position, o ) ; // ���Φn�r�F! 
					} // if
					else { // ��Xerror!!!!!!!!!!! 
						o.error = true ;
						o.allInstr = code ;
						o.code = "Syntax Error!" ;
					} // else
				} // if 
				output.push_back( o ) ; // �Ū��]�n�L!
				line++ ;
			} // while
			
			TranslatePass1() ; // ��pass1���� ��loc ��symbol�� 
			TranslatePass2() ; 
			OutputSIC() ;
		} // TurnMachineCodeSIC
		
		
		void DoHash( string temp, string type ) {
			int i = 0 ;
			int sum = 0 ;
			string allName = "=" + type + "'" + temp + "'" ;
			bool done = false ;
			while ( i < temp.length() ) {	
				sum = sum + temp[i] ;
				i++ ;
			} // while
			
			sum = sum % 100 ;
			while ( literal[sum] != "" ) {
				if ( literal[sum] == allName ) {
					done = true ;
					break ;
				} // if
				else {
					sum++ ;
					if ( sum == 100 ) {
						sum = 0 ;
					} // if
				} // else
				
			} // while
			if ( !done ) {
				literal[sum] = allName ;
			} // if
			
		} // DoHash
		
		void SeparateWordXE( string code, int table[], int position[], OutputXE &o ) {
			o.allInstr = code ; // ��ӫ��O! 
			int i = 0 ;
			int num = position[0] ;
			if ( table[i] == 5 ) { // �w�qsymbol! 
				o.instruction.symbol.name = BigEnglish( table5[num] ) ; // �Ĥ@�Ӧp�G�Osymbol�N�n�w�qsymbol�A���O�p�G���O�}�Y�N�O���O 
				o.instruction.symbol.table = 5 ;
				o.instruction.symbol.position = num ;
				i++ ;
			} // if
			if ( table[i] == 4 && position[i] == 2 ) { // + type4!
				o.type4 = true ;
				i++ ;
			} // if
			num = position[i] ;
			int t1to4Num = num - 1 ; // �]��table1~4���O�q1�}�l�� �ҥHŪ�쪺�Ʀr�n-1 
			int opnum = form[t1to4Num].opnum ; // form�O�̷�table1�����ҥH�]�n-1 
			if ( table[i] == 1 ) {
				o.instruction.instr.name = BigEnglish( table1.at(t1to4Num) ) ;
				o.instruction.instr.table = 1 ;
				o.instruction.instr.position = t1to4Num ;
				if ( opnum != 0 ) { // ��OP 
					i++ ;
					num = position[i] ;
					t1to4Num = num - 1 ;
					if ( table[i] == 3 ) { // op1�Oreg 
						o.instruction.op1.name = table3.at(t1to4Num) ;
						o.instruction.op1.table = 3 ;
						o.instruction.op1.position = t1to4Num ;
					} // if
					else if ( table[i] == 5 ) { // op1�Osymbol 
						o.instruction.op1.name = BigEnglish( table5[num] ) ;
						o.instruction.op1.table = 5 ;
						o.instruction.op1.position = num ;
					} // else if
					else if ( table[i] == 6 ) { // �����O�Ʀr 
						o.instruction.op1.name = table6[num] ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					}  // else if
					else if ( ( table[i] == 4 && ( 11 <= num ) && ( num <= 13 ) ) ) { // = # @�����p 
						i++ ;
						if ( num == 12 ) { // #
							num = position[i] ;
							if ( table[i] == 5 ) {
								o.instruction.op1.name = "#" + BigEnglish( table5[num] ) ;
								o.instruction.op1.table = 5 ;
								o.instruction.op1.position = num ;
							} // if
							else if ( table[i] == 6 ) {
								o.instruction.op1.name = "#" + BigEnglish( table6[num] ) ;
								o.instruction.op1.table = 6 ;
								o.instruction.op1.position = num ;
							} // else if	
						} // if
						else if ( num == 13 ) { // @ �u��symbol 
							num = position[i] ;
							if ( table[i] == 5 ) {
								o.instruction.op1.name = "@" + BigEnglish( table5[num] ) ;
								o.instruction.op1.table = 5 ;
								o.instruction.op1.position = num ;
							} // if
						} // else if
						else { // =
							if ( table[i] == 4 && position[i] == 9 ) { // C'' X''
								i++ ;
								num = position[i] ;
								int d = 0 ;
								if ( table[i] == 6 ) { // X''
									for ( d = 0 ; d < o.allInstr.length() ; d++ ) {
										if ( o.allInstr[d] != ' ' && o.allInstr[d] != '	' && o.allInstr[d] != '+' ) {
											break ;
										} // if
									} // for
									if ( o.allInstr[d] >= 'a' && o.allInstr[d] <= 'z' ) {
										
										for ( int k = 0 ; k < 100 ; k++ ) {
											int m = 0 ;
											while ( table6[k][m] >= 'A' && table6[k][m] <= 'Z' ) {
												table6[k][m] = table6[k][m] + 32 ;
											} // if
										} // for
										o.instruction.op1.name = "=x'" + table6[num] + "'" ;
									} // if
									else {
										o.instruction.op1.name = "=X'" + table6[num] + "'" ;
									} // else
									o.instruction.op1.table = 6 ;
									o.instruction.op1.position = num ;
								} // if
								else { // C''
									for ( d = 0 ; d < o.allInstr.length() ; d++ ) {
										if ( o.allInstr[d] != ' ' && o.allInstr[d] != '	' && o.allInstr[d] != '+' ) {
											break ;
										} // if
									} // for
									if ( o.allInstr[d] >= 'a' && o.allInstr[d] <= 'z' ) {
										for ( int k = 0 ; k < 100 ; k++ ) {
											int m = 0 ;
											while ( table6[k][m] >= 'A' && table6[k][m] <= 'Z' ) {
												table6[k][m] = table6[k][m] + 32 ;
											} // if
										} // for
										o.instruction.op1.name = "=c'" + table7[num] + "'" ;
									} // if
									else {
										o.instruction.op1.name = "=C'" + table7[num] + "'" ; 
									} // else
								 	// ������j�g ���٭n��ASCII 
									o.instruction.op1.table = 7 ;
									o.instruction.op1.position = num ;
								} // else
							} // if
						} // else
					} // else if
					// op1��n�P!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					i++ ;
					if ( opnum == 1 ) {
						if ( table[i] == 4 && position[i] == 1 ) { // symbol�ᦳ,-->�w�g�T�{��k�S���N��O,X�����p 
							o.instruction.X = true ; // ��X!!!!!! 
							o.instruction.op2.name = "X" ;
							o.instruction.op2.table = 3 ;
							o.instruction.op2.position = 2 ; // X reg
						} // if
					}  // if
					if ( opnum == 2 ) { // ��op2!!! 
						if ( table[i] == 4 && position[i] == 1 ) { // symbol�ᦳ,
							i++ ;
							if ( table[i] == 3 ) { // op2�Oreg 
								o.instruction.op2.name = table3.at(position[i]-1) ; // ���s��m��+1 t1~4 
								o.instruction.op2.table = 3 ;
								o.instruction.op2.position = position[i]-1 ;
							} // if
							else { // op2�O�Ʀr 
								o.instruction.op2.name = table6[position[i]] ;
								o.instruction.op2.table = 6 ;
								o.instruction.op2.position = position[i] ;
							} // else
						} // if
					}  // if
				} // if
			} // if
			else if ( table[i] == 2 ) { // �w�qsymbol �᭱�@�w�Otable1 or 2 ���D�O���� 
				o.instruction.instr.name = table2.at(t1to4Num) ;
				o.instruction.instr.table = 2 ;
				o.instruction.instr.position = t1to4Num ;
				i++ ;
				if ( num == 3 || num == 4 ) { // WORD BYTE
					num = position[i] ;
					if ( table[i] == 6 ) { //�᭱�����O�Ʀr 
						o.instruction.op1.name = table6[num] ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					} // if
					else if ( table[i] == 4 && num == 9 ) { // C'EOF' X'F1'
						i++ ;
						num = position[i] ;
						t1to4Num = num - 1 ;
						if ( table[i] == 6 ) { // X'F1'
							o.instruction.op1.name = "X'" + BigEnglish( table6[num] ) + "'" ;
							o.instruction.op1.table = 6 ;
							o.instruction.op1.position = num ;
						} // if
						else if ( table[i] == 7 ) { // C'EOF'
							o.instruction.op1.name = "C'" + table7[num] + "'" ;
							o.instruction.op1.table = 7 ;
							o.instruction.op1.position = num ;
						} // else if
					} // else if
				} // if
				else if ( num == 1 ) { // START�᭱�@�w���Ʀr 
					num = position[i] ;
					o.instruction.op1.name = table6[num] ;
					o.instruction.op1.table = 6 ;
					o.instruction.op1.position = num ;
				} // else if
				else if ( num == 8 ) { // BASE�᭱��symbol��decnum 
					num = position[i] ;
					if ( table[i] == 5 ) {
						o.instruction.op1.name = BigEnglish( table5[num] ) ;
						o.instruction.op1.table = 5 ;
						o.instruction.op1.position = num ;
					} // if
					else {
						o.instruction.op1.name = BigEnglish( table6[num] ) ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					} // else
				} // else if
				else if ( num == 5 || num == 6 ) { // RESW RESB �᭱�u���Ʀr 
					num = position[i] ;
					o.instruction.op1.name = table6[num] ;
					o.instruction.op1.table = 6 ;
					o.instruction.op1.position = num ;
				} // else if
				else if ( num == 7 ) { // EQU �᭱�i��O�@��_�Ǫ��F��@@ ���������s! 
					string temp = "" ;
					if ( table[i] == 6 ) {
						num = position[i] ;
						o.instruction.op1.name = table6[num] ;
						o.instruction.op1.table = 6 ;
						o.instruction.op1.position = num ;
					} // if
					else if ( table[i] == 4 && position[i] == 4 ) { // *
						o.instruction.op1.name = "*" ;
						o.instruction.op1.table = 4 ;
						o.instruction.op1.position = 4 ;
					} // else if
					else { // label �i�঳�|�h�B�� 
						for ( int j = i ; table[j] != -1 ; j++ ) {
							num = position[j] ;
							if ( table[j] == 5 ) {
								temp = temp + table5[num] ;
							} // if
							else if ( table[j] == 4 ) {
								temp = temp + table4.at(num-1) ;
							} // else if
						} // for
						o.instruction.op1.name = BigEnglish( temp ) ;
						o.instruction.op1.table = 5 ;
					} // else
					
				} // else if
				// LTORG�᭱�S�F��@@ END ������ 
			} // else if
			else if ( table[i] == 4 && num == 10 ) { // ����! 
				;
			} // else if
		} // SeparateWordXE()
		
		bool All16Num( string temp ) { // �Φb����address�Msymbol SIC�Laddress 
			for ( int j = 0 ; j < temp.length() ; j++ ) { // �T�{�Oaddress�٬Osymbol 
				if ( temp[j] >= '0' && temp[j] <= '9' ) { // �p�G�O�Ʀr���ܴN�S�� 
					;
				} // if
				else if ( temp[j] >= 'A' && temp[j] <= 'F' ) { // opname���O�j�g�� 
					;
				} // else if
				else {
					return false ;
				} // else
			} // for
			return true ;
		} // All16Num
		
		void TranslatePass1XE() { // SICXE Pass1�o! literal!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			string strLoc = "0000" ; // �r�ꪺlocation 
			int loc = 0 ; // �p��Ϊ�loc
			int i = 0 ;
			stringstream ss ; // �ഫ�i�� 
			int move = 0 ; // BYTE WORD�� 
			string temp = "" ; // C''||EQU END LTORG�� 
			string code = "" ; // END LTORG�� 
			string t1 = "" ;
			int num = 0 ; // C''�� 
			while ( i < outXE.size() ) {
				Symbol s ;
				OutputXE o = outXE.at(i) ;
				SIC ins = o.instruction ;
				if ( ins.instr.name == "START" ) { // �]�w�{���_�l��m! ����½�����X 
					strLoc = ins.op1.name ; // �{���_�l��m
					loc = LocStrtoNum( strLoc ) ; 
					baseReg = loc ; // �sbaseReg��m 
					strLoc = BigEnglish( strLoc ) ; // �^����j�g��
					strLoc = FillStr( strLoc, 4 ) ; // �񺡺�4�� 
					outXE.at(i).loc = strLoc ;
					outXE.at(i).PC = loc ; // �n�sPC! 
					if ( ins.symbol.name != "" ) {
						s.name = ins.symbol.name ; // �Nsymbol�s�Jsymboltable 
						s.address = strLoc ;
						for ( int k = 0 ; k < symtab.size() ; k++ ) {
							if ( symtab.at(k).name == ins.symbol.name ) {
								outXE.at(i).error = true ;
								outXE.at(i).code = "Duplicated Definition!" ;
								break ;
							} // if
						} // for
						if ( !outXE.at(i).error ) { // �S���D�����O �N�i�Hpush 
							symtab.push_back( s ) ;
						} // if
					} // if
				} // if
				else {
					if ( ins.instr.name != "" ) { // ���O���ѩΪ̪Ū� 
						if ( ins.instr.name != "END" && ins.instr.name != "BASE" && ins.instr.name != "LTORG" ) { // �S����  �]��BASE�]���κ�LOC 
							if ( !outXE.at(i).error ) { // �S�������O�~�n�B�z 
								if ( ins.instr.name == "RESB" ) {
									outXE.at(i).loc = FillStr( strLoc, 4 ) ;
									ss << dec << ins.op1.name ; // �r����Ʀr 
									ss >> move ;
									ss.clear() ;
									loc = loc + move ;
									strLoc = LocNumtoStr( loc ) ;
								} // if
								else if ( ins.instr.name == "RESW" ) {
									outXE.at(i).loc = FillStr( strLoc, 4 ) ;
									ss << dec << ins.op1.name ; // �r����Ʀr 
									ss >> move ;
									ss.clear() ;
									move = move * 3 ; // word��3byte
									loc = loc + move ;
									strLoc = LocNumtoStr( loc ) ;
									outXE.at(i).PC = loc ; // �n�sPC!
								} // else if
								else if ( ins.instr.name == "BYTE" || ins.instr.name == "WORD" ) {
									outXE.at(i).loc = FillStr( strLoc, 4 ) ;
									if ( ins.op1.name[0] == 'C' ) { // C'EOF' || X'F1' 
										temp = "" ;
										for ( int j = 2 ; j < ins.op1.name.length() ; j++ ) {
											if ( ins.op1.name[j] != '\'' ) {
												num = ins.op1.name[j] ;
												outXE.at(i).code = outXE.at(i).code + LocNumtoStr( num ) ;
											} // if
											else {
												break ;
											} // else
										} // for
										move = outXE.at(i).code.length() ;
										outXE.at(i).code = BigEnglish( outXE.at(i).code ) ;
										if ( ins.instr.name == "WORD" ) { // �n��6�ӫ��O 
											outXE.at(i).code = FillStr( outXE.at(i).code, 6 ) ;
											move = 6 ;
										} // if
										else if ( move % 2 != 0 ) { // code���׬��_�� 
											outXE.at(i).code = "0" + outXE.at(i).code ;
											move++ ;
										} // else if
										move = move / 2 ;
										loc = loc + move ;
										strLoc = LocNumtoStr( loc ) ;
									} // if
									else if ( ins.op1.name[0] == 'X' ) {
										temp = "" ;
										for ( int j = 2 ; j < ins.op1.name.length() ; j++ ) {
											if ( ins.op1.name[j] != '\'' ) {
												outXE.at(i).code = outXE.at(i).code + ins.op1.name[j] ;
											} // if
											else {
												break ;
											} // else
										} // for
										outXE.at(i).code = BigEnglish( outXE.at(i).code ) ;
										move = outXE.at(i).code.length() ;
										if ( ins.instr.name == "WORD" ) { // �n��6�ӫ��O 
											outXE.at(i).code = FillStr( outXE.at(i).code, 6 ) ;
											move = 6 ;
										} // if
										else if ( move % 2 != 0 ) { // code���׬��_�� 
											outXE.at(i).code = "0" + outXE.at(i).code ;
											move++ ;
										} // else if
										move = move / 2 ;
										loc = loc + move ;
										strLoc = LocNumtoStr( loc ) ;
									} // else if
									else { // dec num
										outXE.at(i).loc = FillStr( strLoc, 4 ) ;
										ss << dec << ins.op1.name ; // �r����Ʀr 
										ss >> num ; 
										ss.clear() ;
										ss << hex << num ; // �N�Ʀr�H16�i��Ū�J�ন�r�� 
										ss >> outXE.at(i).code ;
										ss.clear() ;
										outXE.at(i).code = BigEnglish( outXE.at(i).code ) ;
										move = outXE.at(i).code.length() ;
										if ( ins.instr.name == "WORD" ) { // �n��6�ӫ��O 
											outXE.at(i).code = FillStr( outXE.at(i).code, 6 ) ;
											move = 6 ;
										} // if
										else if ( move % 2 != 0 ) { // code���׬��_�� 
											outXE.at(i).code = "0" + outXE.at(i).code ;
											move++ ;
										} // else if
										move = move / 2 ;
										loc = loc + move ;
										strLoc = LocNumtoStr( loc ) ;
									} // else
									outXE.at(i).PC = loc ; // �n�sPC!
								} // else if
								else if ( ins.instr.name == "EQU" ) { // ���v�T�U�@�D���O����m 
									if ( ins.op1.table == 6 ) {
										ss << dec << ins.op1.name ;
										ss >> move ;
										ss.clear() ;
										temp = "" ;
										ss << hex << move ;
										ss >> temp ;
										ss.clear() ;
										temp = BigEnglish( temp ) ;
										outXE.at(i).loc = FillStr( temp, 4 ) ;
										if ( i > 0 ) { // ���O�Ĥ@�� ���e�@�ӤH��PC�ݭn�� 
											outXE.at(i-1).PC = move ; // �n�sPC!
										} // if
										outXE.at(i).PC = loc ; // �n�sPC!
									} // if
									else if ( ins.op1.table == 4 ) { // * �����O���`�⪺address 
										outXE.at(i).loc = FillStr( strLoc, 4 ) ;
										outXE.at(i).PC = loc ; // �n�sPC!
										// �U�@�D���OLOC�@�� 
									} // else if
									else { // label�o! 
										temp = ins.op1.name ;
										string t = "" ; // �Ȧssymbol 
										int sum = 0 ; // �Ƴ̫�Loc
										char c = '\0' ; 
										for ( int j = 0 ; j < temp.length() ; j++ ) {
											if ( temp[j] != '+' && temp[j] != '*' && temp[j] != '-' && temp[j] != '/' ) {
												t = t + temp[j] ;
											} // if
											else {
												ss << hex << FindSymLoc( t ) ;
												ss >> move ;
												ss.clear() ;
												t = "" ;
												if ( sum == 0 ) {
													sum = move ;
												} // if
												else if ( c == '-' ) {
													sum = sum - move ;
												} // else if
												else if ( c == '+' ) {
													sum = sum + move ;
												} // else if
												else if ( c == '*' ) {
													sum = sum * move ;
												} // else if
												else if ( c == '/' ) {
													sum = sum / move ;
												} // else if
												c = temp[j] ;
											} // else
										} // for
										ss << hex << FindSymLoc( t ) ;
										ss >> move ;
										ss.clear() ;
										if ( sum == 0 ) { // ���i��N�@��label 
											sum = move ;
										} // if
										else if ( c == '-' ) { // �̫�@��symbol�S�p��� 
											sum = sum - move ;
										} // else if
										else if ( c == '+' ) {
											sum = sum + move ;
										} // else if
										else if ( c == '*' ) {
											sum = sum * move ;
										} // else if
										else if ( c == '/' ) {
											sum = sum / move ;
										} // else if
										ss << hex << sum ;
										ss >> temp ;
										ss.clear() ;
										temp = BigEnglish( temp ) ;
										outXE.at(i).loc = FillStr( temp, 4 ) ;
										if ( i > 0 ) { // �p�G���e�@�ӤH �L��PC�n�� 
											outXE.at(i-1).PC = sum ; // �n�sPC!
										} // if
										outXE.at(i).PC = loc ; // �n�sPC!
									} // else
								} // else if
								else if ( ins.instr.table == 1 ) { // ���O!
									num = ins.instr.position ;
									int type = form[num].type ;
									outXE.at(i).loc = FillStr( strLoc, 4 ) ;
									if ( type == 1 ) { // �u��opcode 1byte 
										loc = loc + 1 ; // 1byte 
										strLoc = LocNumtoStr( loc ) ;
									}  // if
									else if ( type == 2 ) { // ��opcode r1 r2 --> �@2byte 
										loc = loc + 2 ; // 4byte 
										strLoc = LocNumtoStr( loc ) ;
									} // else if
									else { // �u�ѤUtype3 or 4 --> 3:3byte, 4:4byte 
										if ( o.type4 ) {
											loc = loc + 4 ; // 8byte 
											strLoc = LocNumtoStr( loc ) ;
										} // if
										else {
											loc = loc + 3 ; // 6byte 
											strLoc = LocNumtoStr( loc ) ;
										} // else
									} // else
									outXE.at(i).PC = loc ; // �n�sPC!
								} // else
							} // if
						} // if
						else if ( ins.instr.name == "LTORG" || ins.instr.name == "END" ) { // ���ΦsPC
							// literal ����@���� �n�ݨ�LTORG��~�񤧫e�ݨ쪺 ���ѨM 
							for ( int j = 0 ; j < 100 ; j++ ) {
								if ( literal[j] != "" ) { // �n�������O�o! 
									OutputXE lit ;
									lit.allInstr = literal[j] + "	BYTE	" ;
									temp = literal[j].substr( 3, literal[j].length()-4 ) ; // EOF || F1 
									if ( literal[j][1] == 'C' || literal[j][1] == 'c' ) {
										for ( int n = 0 ; n < temp.length() ; n++ ) {
											num = temp[n] ;
											lit.code = lit.code + LocNumtoStr( num ) ; ;
										} // for
									} // if
									else if ( literal[j][1] == 'X' || literal[j][1] == 'x' ) {
										lit.code = temp ;
									} // else if
									lit.code = BigEnglish( lit.code ) ;
									move = lit.code.length() ;
									if ( move % 2 != 0 ) { // code���׬��_�� 
										lit.code = "0" + lit.code ;
										move++ ;
									} // if
									lit.loc = BigEnglish( strLoc ) ;
									lit.loc = FillStr( lit.loc, 4 ) ;
									lit.instruction.instr.name = "BYTE" ;
									lit.instruction.instr.table = 2 ;
									lit.instruction.instr.position = 3 ;
									lit.instruction.symbol.name = literal[j] ;
									lit.instruction.symbol.table = 5 ;
									Symbol s ;
									s.name = BigEnglish( literal[j] ) ;
									s.address = lit.loc ;
									symtab.push_back( s ) ;
									move = move / 2 ;
									loc = loc + move ; // code����/2 
									strLoc = LocNumtoStr( loc ) ;
									strLoc = FillStr( strLoc, 4 ) ;
									i++ ; // �|���b�e���G+1 
									outXE.insert( ( outXE.begin() + i ), lit ) ;
									outXE.at(i).PC = loc ; // �n�sPC!
									literal[j] = "" ; // �M��table 
								}  // if
							} // for
						} // else if
						// BASE ������(pass2�~��baseReg) 
						if ( ins.symbol.name != "" ) { // ���n�w�qsymbol 
							s.name = ins.symbol.name ; // �Nsymbol�s�Jsymboltable 
							s.address = outXE.at(i).loc ;
							for ( int k = 0 ; k < symtab.size() ; k++ ) {
								if ( symtab.at(k).name == ins.symbol.name ) {
									outXE.at(i).error = true ;
									outXE.at(i).code = "Duplicated Definition!" ;
									break ;
								} // if
							} // for
							if ( !outXE.at(i).error ) { // �S���D�����O �N�i�Hpush 
								symtab.push_back( s ) ;
							} // if
						} // if
					} // if
					// ���ѩΪŪ��]������! 
				} // else
				if ( ins.op1.name[0] == '=' ) { // literal �n��Jliteral table
					temp = ins.op1.name.substr( 3, ins.op1.name.length() - 4 ) ; // ��EOF||F1 
					string type = "" ;
					type = ins.op1.name.substr( 1, 1 ) ;
					if ( ins.op1.table == 6 ) { // X'F1'
						DoHash( temp, type ) ;
					}  // if
					else {
						DoHash( temp, type ) ;
					} // else
				} // if
				i++ ;
			} // while
		} // TranslatePass1XE
		
		string XEType2( OutputXE o, SIC instr, string opcode ) { // opcode r1{,r2}
			stringstream ss ;
			string op1name = instr.op1.name ;
			string op2name = instr.op2.name ;
			string code = opcode ;
			string temp = "" ;
			int num = instr.op1.position ; // reg��code���L�btable����m
			if ( instr.op1.table == 3 ) {
				ss << dec << num ;
				ss >> temp ;
				ss.clear() ;
			} // if
			else if ( instr.op1.table == 6 ) {
				temp = instr.op1.name ;
			}  // else if
			code = code + temp ; // �top2 
			if ( op2name == "" ) { // �u��1��op 	
				code = code + "0" ;
			} // if
			else {
				num = instr.op2.position ;
				ss << dec << num ;
				ss >> temp ;
				ss.clear() ;
				code = code + temp ;
			} // else
			code = BigEnglish( code ) ;
			return code ;
		} // XEType2
		
		void TranslatePass2XE() {
			stringstream ss ;
			string addr = "" ; // ½�����X�ݭn��binary�h�ഫ 
			string temp = "" ;
			string opcode = "" ;
			int codeNum = 0 ;
			int disp = 0 ;
			int pc = -1 ;
			int index = 0 ;
			string code = "" ;
			for ( int i = 0 ; i < outXE.size() ; i++ ) {
				OutputXE o = outXE.at(i) ;
				SIC instr = o.instruction ;
				if ( o.code == "" && instr.instr.name != "" ) { // ��k���T&�٨S�⪺&�D���� 
					if ( instr.instr.table == 1 ) { // �n�doptable�n�Otable1
						index = instr.instr.position ; 
						opcode = form[index].opcode ;
						if ( form[index].type == 1 ) { // type1-->�S��operand
							outXE.at(i).code = opcode ; // ���θ�0�� 
							outXE.at(i).allInstr = outXE.at(i).allInstr + "\t" ;
						} // if
						else if ( form[index].type == 2 ) { // type2 
							outXE.at(i).code = XEType2( o, instr, opcode ) ;
						} // else if
						else if ( form[index].type == 3 ) { // type3 4 -->op�Ƭ�0��1(m) 
							string op1name = o.instruction.op1.name ;
							pc = o.PC ;
							bool imm = false ;
							bool fin = false ;
							code = opcode ;
							codeNum = LocStrtoNum( code ) ; // �N16�i�쪺�r���ର10�i�쪺�Ʀr 
							if ( op1name == "" ) { // type3 4 ��opnum = 0 
								codeNum = codeNum + 3 ; // �S��op-->�D#�D@
								code = LocNumtoStr( codeNum ) ; 
								code = FillStr( code, 2 ) ;
								if ( !o.type4 ) { // type3
									outXE.at(i).code = code + "0000" ; // �᭱�Sop �ҥH�������O0 
								} // if
								else { // type4
									outXE.at(i).code = opcode + "100000" ; // �᭱�Sop �ҥH1(xbpe)-->0001
								} // else
							} // if
							else if ( op1name[0] == '#' || op1name[0] == '@' ) { 
								// # opcode + 1 �᭱�i��Osymbol��decnum 
								// @ opcode + 2 �᭱�i��Osymbol  
								if ( op1name[0] == '#' ) {
									codeNum++ ; // # opcode+1-->opcode��n�F
								} // if
								else { // @
									codeNum = codeNum + 2 ;
								} // else
								code = LocNumtoStr( codeNum ) ; // �j�g�]��n�F
								code = FillStr( code, 2 ) ;
								temp = op1name.substr( 1, op1name.length()-1 ) ; // ��#�᭱���F�� 
								if ( instr.op1.table == 5 ) {
									temp = FindSymLoc( temp ) ;
									if ( temp == "" ) { // �䤣��loc-->�S�w�q��symbol �B�zUndefine Symbol-->��LOC�|�����D(����S���a)	
										for ( int j = 0 ; j < symtab.size() ; j++ ) {
											if ( symtab.at(j).name == o.instruction.symbol.name ) {
												symtab.erase( symtab.begin() + j ) ;
												break ;
											} // if
										} // for
										outXE.at(i).loc = "" ;
										outXE.at(i).code = "Undefine Symbol!" ;
										outXE.at(i).error = true ;
										fin = true ;
									} // if
									else {
										codeNum = LocStrtoNum( temp ) ; // symbol address(int)
									} // else
								} // if
								else { // decnum
									ss << dec << temp ;
									ss >> codeNum ;
									ss.clear() ;
									imm = true ;
								} // else
								if ( !fin && !o.type4 ) { // type3 ��Z�� 
									if ( imm ) { // #3 ������������ 
										temp = LocNumtoStr( codeNum ) ;
										code = code + FillStr( temp, 4 ) ; // disp 3bytes + xbpe(1byte)
									} // if
									else { // symbol�� 
										disp = codeNum - pc ; // �ت��a-PC
										if ( disp >= -2047 && disp <= 2047 ) { // �W�X�o�ӽd��n��baseReg 
												addr = LocNumtoStr( disp ) ; // disp �j�g����
											addr = FillStr( addr, 3 ) ; // �����Y�����n��0(�t���ܰ�) 
											addr = addr.substr( addr.length()-3, 3 ) ; // �u��3��(12bit)
											code = code + "2" + addr ; // code(opcode)+2(xbpe-->0010)+addr(disp) 
										}  // if
										else { // ��base�� 
											disp = codeNum - baseReg ; // �ت�-baseReg
											addr = LocNumtoStr( disp ) ; // disp �j�g���� 
											addr = FillStr( addr, 3 ) ; // �����Y�����n��0(�t���ܰ�) 
											addr = addr.substr( addr.length()-3, 3 ) ; // �u��3��(12bit)
											code = code + "4" + addr ; // code(opcode)+4(xbpe-->0100)+addr(disp) 
										} // else
									} // else
									outXE.at(i).code = code ;
								} // if
								else if ( !fin ) { // type4 ������symbol address 
									addr = LocNumtoStr( codeNum ) ; // address(str)
									addr = FillStr( addr, 5 ) ; // loc�u��4bytes-->address�n5bytes
									code = code + "1" + addr ; // code(opcode) 1(xbpe)0001 addr(addr)
									outXE.at(i).code = code ; 
								} // else if
							} // else if
							else if ( op1name[0] == '=' ) { // literal type3 4
								codeNum = codeNum + 3 ;
								code = LocNumtoStr( codeNum ) ; // opcode½�n�F �j�g��!	
								code = FillStr( code, 2 ) ;
								addr = FindSymLoc( BigEnglish(op1name) ) ; // literal address 
								codeNum = LocStrtoNum( addr ) ; // literal address(int)
								if ( !o.type4 ) { // type3
									disp = codeNum - pc ; // �ت��a-PC
									if ( disp >= -2047 && disp <= 2047 ) { // �W�X�o�ӽd��n��baseReg 
										addr = LocNumtoStr( disp ) ; // disp �j�g����
										addr = FillStr( addr, 3 ) ; // �����Y�����n��0(�t���ܰ�) 
										addr = addr.substr( addr.length()-3, 3 ) ; // �u��3��(12bit)
										code = code + "2" + addr ; // code(opcode)+2(xbpe-->0010)+addr(disp) 
									}  // if
									else { // ��base�� 
										disp = codeNum - baseReg ; // �ت�-baseReg
										addr = LocNumtoStr( disp ) ; // disp �j�g���� 
										addr = FillStr( addr, 3 ) ; // �����Y�����n��0(�t���ܰ�) 
										addr = addr.substr( addr.length()-3, 3 ) ; // �u��3��(12bit)
										code = code + "4" + addr ; // code(opcode)+4(xbpe-->0100)+addr(disp) 
									} // else
									outXE.at(i).code = code ;
								} // if
								else {
									addr = FillStr( addr, 5 ) ; // �n��5��bytes! 
									code = code + "1" + addr ; // code(opcode) 1(xbpe)0001 addr(addr)
									outXE.at(i).code = code ; 
								} // else
							} // else if
							else { // ���O#�]���O@�]���Oliteral opcode + 3 
								codeNum = codeNum + 3 ;
								code = LocNumtoStr( codeNum ) ; // opcode½�n�F �j�g��!	
								code = FillStr( code, 2 ) ;	
								if ( All16Num( op1name ) ) { // address
									//codeNum = LocStrtoNum( op1name ) ; // address(int) 
									if ( !o.type4 ) {
										addr = FillStr( op1name, 4 ) ;
										outXE.at(i).code = code + addr ;
										fin = true ;
									} // if
									else {
										addr = FillStr( op1name, 6 ) ;
										outXE.at(i).code = code + addr ;
										fin = true ;
									} // else
								} // if
								else { // symbol
									addr = FindSymLoc( op1name ) ; // symbol address(str)
									if ( addr != "" ) { // �����! 
										codeNum = LocStrtoNum( addr ) ; // Symbol address(int)
									} // if
									else { // �B�zUndefine Symbol-->��LOC�|�����D(����S���a) 
										for ( int j = 0 ; j < symtab.size() ; j++ ) {
											if ( symtab.at(j).name == o.instruction.symbol.name ) {
												symtab.erase( symtab.begin() + j ) ;
												break ;
											} // if
										} // for
										outXE.at(i).loc = "" ;
										outXE.at(i).code = "Undefine Symbol!" ;
										outXE.at(i).error = true ;
										fin = true ;
									} // else
								} // else
								if ( !fin && !o.type4 ) { // �p�G�S�����w�qsymbol type3
									disp = codeNum - pc ;
									if ( disp >= -2047 && disp <= 2047 ) { // �W�X�o�ӽd��n��baseReg 
										addr = LocNumtoStr( disp ) ; // disp �j�g����
										addr = FillStr( addr, 3 ) ; // �����Y�����n��0(�t���ܰ�) 
										addr = addr.substr( addr.length()-3, 3 ) ; // �u��3��(12bit)
										if ( instr.X ) {
											code = code + "A" + addr ; // code(opcode)+A(xbpe-->1010)+addr(disp) 
										} // if
										else {
											code = code + "2" + addr ; // code(opcode)+2(xbpe-->0010)+addr(disp) 
										} // else
										
									}  // if
									else { // ��base�� 
										disp = codeNum - baseReg ; // �ت�-baseReg
										addr = LocNumtoStr( disp ) ; // disp �j�g���� 
										addr = FillStr( addr, 3 ) ; // �����Y�����n��0(�t���ܰ�) 
										addr = addr.substr( addr.length()-3, 3 ) ; // �u��3��(12bit)
										if ( instr.X ) {
											code = code + "C" + addr ; // code(opcode)+C(xbpe-->1100)+addr(disp) 
										} // if
										else {
											code = code + "4" + addr ; // code(opcode)+2(xbpe-->0010)+addr(disp) 
										} // else
									} // else
									outXE.at(i).code = code ;
								} // if
								else if ( !fin ) { // type4
									addr = LocNumtoStr( codeNum ) ; // address(str)
									addr = FillStr( addr, 5 ) ; // loc�u��4bytes-->address�n5bytes
									if ( instr.X ) {
										code = code + "9" + addr ; // code(opcode) 9(xbpe)1001 addr(addr)
									} // if
									else {
										code = code + "1" + addr ; // code(opcode) 1(xbpe)0001 addr(addr)
									} // else
									outXE.at(i).code = code ; 
								} // else if
							} // else
						} // else if
					} // if
					else if ( instr.instr.table == 2 ) { // �ڭn��sbaseReg 
						if ( instr.instr.name == "BASE" ) { // ��baseReg 
							if ( instr.op1.table == 6 ) { // 10�i��Ʀr������ 
								ss << dec << instr.op1.name ;
								ss >> dec >> baseReg ;
								ss.clear() ;
							} // if
							else { // symbol
								temp = FindSymLoc( instr.op1.name ) ;
								if ( temp != "" ) {  // ���F! 
									ss << hex << temp ;
									ss >> baseReg ;
									ss.clear() ;
								} // if
								else { // �S���symbol-->LOC�����D���
									for ( int j = 0 ; j < symtab.size() ; j++ ) {
										if ( symtab.at(j).name == o.instruction.symbol.name ) {
											symtab.erase( symtab.begin() + j ) ;
											break ;
										} // if
									} // for
									outXE.at(i).loc = "" ;
									outXE.at(i).code = "Undefine Symbol!" ;
									outXE.at(i).error = true ;
								} // else
							} // else
						} // if
					} // else if
				} // if
			} // for
		} // TranslatePass2XE
		
		void TurnMachineCodeSICXE() { // ������X SIC
			int line = 0 ;
			int table[10] = {0} ;
			int position[10] = {0} ;
			while ( line < input.size() ) {
				string code = input.at(line) ; // ���X��line�檺�F��
				line++ ;
				OutputXE o ;
				while ( ( code == "" || code == "\n" ) && line < input.size()-1 ) {
					if ( line % 2 == 0 ) { // index�q0�}�l 
						outXE.push_back( o ) ; // �Ū��]�n�L!
					} // if
					code = input.at(line) ; // ���X��line�檺�F��
					line++ ;
				} // while
				string format = input.at(line) ; // ���X��line�檺�F��
				if ( format != "" ) { // ���O�Ū��~�P�_ 
					if ( !CheckError( code, format, table, position ) ) { // �ˬd��k���S����-->�Lerror 
						SeparateWordXE( code, table, position, o ) ; // ���Φn�r�F! 
					} // if
					else { // ��Xerror!!!!!!!!!!! 
						o.error = true ;
						o.allInstr = code ;
						o.code = "Syntax Error!" ;
					} // else
				} // if 
				outXE.push_back( o ) ; // �Ū��]�n�L!
				line++ ;
			} // while
			TranslatePass1XE() ; // ��pass1���� ��loc ��symbol�� 
			TranslatePass2XE() ; 
			OutputSICXE() ;
		} // TurnMachineCodeSICXE
		
		void OutputSICXE() {
			string name = fileName + "_SICXEMachineCode.txt" ;
			FILE *file = NULL ; // �ɮ׫��� 
			file = fopen( name.c_str(), "w" ) ; 
	  		// ���}�ɮ� r�N��Ū c_str()�Nstring�����নC�y���Φ����r��`��(�L�k���N���) 
			if ( file == NULL ) {
				cout << endl << "�ɮ׶}�ҥ���!" << endl << endl ; 
				fclose( file ) ;
			} // if
			else { 
				int k = 0 ;
				fprintf( file, "Line  Location  Source code                              Object code\n" ) ;
				for ( int j = 0 ; j < outXE.size() ; j++ ) {
					OutputXE o = outXE.at(j) ;
					if ( o.allInstr != "" ) {
						k = k + 5 ;	
						fprintf( file, "%d	%s	", k, o.loc.c_str() ) ;
						if ( o.instruction.symbol.name == "" && o.allInstr[0] != '\t' ) { // �Ssymbol�S�Stab 
							fprintf( file, "\t" ) ;
						} // if
						fprintf( file, "%s\t\t", o.allInstr.c_str() ) ;
						if ( !o.instruction.X ) {
							fprintf( file, "\t" ) ;
						} // if
						fprintf( file, "%s\n", o.code.c_str() ) ;
					} // if
					else {
						fprintf( file, "\n" ) ;
					} // else
					
				} // for
			} // else 	
	
			fclose( file ) ;
		} // OutputSICXE
};

void DoSIC() {
	LexicalAnalysis lex ;
	Lexical_Analysis(lex) ; // ���ntoken �ӥB��table567 
	SyntaxAnalysis syn ;
	syn.ReadFile() ;
	syn.ReadFormat() ;
	syn.Copy(lex) ;
	syn.TurnMachineCodeSIC() ;
} // DoSIC

void DoSICXE() {
	LexicalAnalysis lex ;
	Lexical_Analysis(lex) ; // ���ntoken �ӥB��table567 
	SyntaxAnalysis syn ;
	syn.ReadFile() ;
	syn.ReadFormat() ;
	syn.Copy(lex) ;
	syn.TurnMachineCodeSICXE() ;
} // DoSICXE

int main() {
	char command = '\0' ;
	bool exit = false ;
	while ( !exit ) {
		cout << "�п�ܭn���檺�{���y��(��J��L���O�Y����):\n1.SIC\n2.SICXE\n:" ;
		cin >> command ;
		if ( command == '1' ) {
			DoSIC() ;
		} // if
		else if ( command == '2' ) {
			DoSICXE() ;
		} // else if
		else {
			exit = true ;
		} // else
	} // while
	
	
} // main()



